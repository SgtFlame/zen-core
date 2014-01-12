//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Core Framework
//
// Copyright (C) 2001 - 2010 Tony Richards
//
//  This software is provided 'as-is', without any express or implied
//  warranty.  In no event will the authors be held liable for any damages
//  arising from the use of this software.
//
//  Permission is granted to anyone to use this software for any purpose,
//  including commercial applications, and to alter it and redistribute it
//  freely, subject to the following restrictions:
//
//  1. The origin of this software must not be misrepresented; you must not
//     claim that you wrote the original software. If you use this software
//     in a product, an acknowledgment in the product documentation would be
//     appreciated but is not required.
//  2. Altered source versions must be plainly marked as such, and must not be
//     misrepresented as being the original software.
//  3. This notice may not be removed or altered from any source distribution.
//
//  Tony Richards trichards@indiezen.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

// Force compilation of templates
#include "../Event.hpp"
#include "../Connection.hpp"

#include "../future_return_value.hpp"

#include "Event_impl.hpp"
#include "Connection_impl.hpp"
#include "ScriptAction.hpp"

#include "EventQueue.hpp"
#include "EventService.hpp"

#include <Zen/Core/Scripting/forward_declarations.hpp>
#include <Zen/Core/Scripting/I_ScriptType.hpp>

#include <boost/bind.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Event {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Event_impl::Event_impl(EventService& _service)
:   m_service(_service)
,   m_connections()
,   m_pMutex(Threading::MutexFactory::create())
,   m_pScriptObject(NULL)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Event_impl::~Event_impl()
{
    /// When a connection is disconnected, it is removed from
    /// the m_connections, so we cannot be iterating through the
    /// list and disconnecting at the same time.  To solve this
    /// problem, we splice the m_connections list onto tempConnections
    /// and use tempConnections for iterating instead of m_connections.
    container_type tempConnections;

    // This section must be synchronized
    {
        Threading::CriticalSection guard(m_pMutex);

        // splice() is an efficient way to remove items from one list
        // and put it in another list without churning the memory allocator.
        tempConnections.splice(tempConnections.begin(), m_connections);
    }

    // Iterate through the original list of connections and disconnect them.
    if( !tempConnections.empty() )
    {
        for(container_type::iterator iter = tempConnections.begin();
             iter != tempConnections.end() ;
             iter++ )
        {
            (*iter)->disconnect();
        }
    }

    // And finally, destroy the mutex.
    Threading::MutexFactory::destroy(m_pMutex);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static std::string sm_typeName("Event");
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
Event_impl::getScriptTypeName()
{
    return sm_typeName;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Scripting::I_ObjectReference*
Event_impl::getScriptObject()
{
    if (m_pScriptObject == NULL)
    {
        m_pScriptObject = new ScriptWrapper_type(getScriptModule(), 
            getScriptModule()->getScriptType(getScriptTypeName()),
            this
            );
    }

    return m_pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Event_impl::pScriptModule_type
Event_impl::getScriptModule()
{
    return m_service.getScriptModule();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_Connection&
Event_impl::connect(pAction_type _pAction, I_EventQueue* _pQueue)
{
    pConnection_type pConnection = new Connection_type(this, _pAction);
    _pAction->addConnection(*pConnection);

    Threading::CriticalSection guard(m_pMutex);

    m_connections.push_back(pConnection);
    if (_pQueue)
    {
        EventQueue* pQueue = dynamic_cast<EventQueue*>(_pQueue);

        if (pQueue)
        {
            m_queues.insert(pQueue);
        }
    }
    else
    {
        // Dispatch to the default queue.
        m_queues.insert(dynamic_cast<EventQueue*>(&m_service.getEventQueue("default")));
    }

    return *pConnection;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Event_impl::disconnect(Connection_impl* _pConnection)
{
    // TODO Keep a reference count for the queues and
    // when the last connection is disconnected for a
    // given queue, remove it from the set.

    Threading::CriticalSection guard(m_pMutex);

    // TODO Optimize. This requires a linear lookup
    m_connections.remove(_pConnection);

    /// This is the only place where a connection is deleted.
    delete _pConnection;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Event_impl::fireEvent(boost::any _argument)
{
    Threading::CriticalSection guard(m_pMutex);

    for(Queues_type::iterator iter = m_queues.begin(); iter != m_queues.end(); iter++)
    {
        (*iter)->queueEvent(this, _argument);
    }    
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Event_impl::dispatch(boost::any _argument)
{
    typedef std::list<pAction_type> actions_type;
    actions_type actions;

    // Get a list of actions.  We don't want to 
    // stay blocked too long and an action might
    // take a long time, so we create a list of
    // actions while guarded and then dispatch them
    // when unguarded.
    {
        Threading::CriticalSection guard(m_pMutex);

        for(container_type::iterator iter = m_connections.begin(); iter != m_connections.end(); iter++)
        {
            actions.push_back((*iter)->getAction());
        }
    }

    // Iterate through the list of actions and dispatch them.
    for(actions_type::iterator iter = actions.begin(); iter != actions.end(); iter++)
    {
        (*iter)->dispatch(_argument);
    }

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_Connection&
Event_impl::connectScript(boost::any _scriptObject, boost::any _scriptFunction)
{
    ScriptAction* pRawAction = new ScriptAction(m_service.getScriptObject()->getModule(),
        _scriptObject, _scriptFunction);

    pAction_type pAction(pRawAction, boost::bind(&Event_impl::destroyScriptAction, this, _1));

    return connect(pAction, &m_service.getEventQueue("script"));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Event_impl::destroyScriptAction(pAction_type::weak_ptr_type _pAction)
{
    ScriptAction* pAction = dynamic_cast<ScriptAction*>(_pAction.get());

    delete pAction;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Event
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
