//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Core Framework
//
// Copyright (C) 2001 - 2010 Tony Richards
// Copyright (C) 2008 - 2010 Matthew Alan Gray
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
//  Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "EventService.hpp"
#include "EventQueue.hpp"
#include "ActionMap.hpp"
#include "ScriptAction.hpp"
#include "Event_impl.hpp"
#include "Connection_impl.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Core/Scripting/I_ScriptType.hpp>

#include <Zen/Core/Scripting/script_module.hpp>
#include <Zen/Core/Scripting/script_type.hpp>

#include <Zen/Core/Scripting/script_module_impl.hpp>
#include <Zen/Core/Scripting/script_type_impl.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Event {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
EventService::EventService(const std::string& _name)
:   m_name(_name)
,   m_pScriptObject(NULL)
,   m_pScriptEngine()
,   m_pScriptModule(NULL)
,   m_pEventsMutex(Threading::MutexFactory::create())
,   m_pEventQueuesMutex(Threading::MutexFactory::create())
,   m_pActionMapsMutex(Threading::MutexFactory::create())
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
EventService::~EventService()
{
    {
        ActionMaps_type::iterator iter = m_actionMaps.begin();
        while( iter != m_actionMaps.end() )
        {
            delete(iter->second);
            iter++;
        }
    }

    {
        Threading::CriticalSection guard(m_pEventQueuesMutex);

        EventQueues_type::iterator iter = m_eventQueues.begin();
        while( iter != m_eventQueues.end() )
        {
            delete(iter->second);
            iter++;
        }
    }

    Threading::MutexFactory::destroy(m_pEventsMutex);
    Threading::MutexFactory::destroy(m_pEventQueuesMutex);
    Threading::MutexFactory::destroy(m_pActionMapsMutex);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static std::string sm_typeName("EventService");
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
EventService::getScriptTypeName()
{
    return sm_typeName;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Scripting::I_ObjectReference*
EventService::getScriptObject()
{
    if (m_pScriptObject == NULL)
    {
        m_pScriptObject = new ScriptWrapper_type(getScriptModule(),
            getScriptModule()->getScriptType(getScriptTypeName()),
            this->getSelfReference().lock()
            );
    }

    return m_pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
EventService::pScriptModule_type
EventService::getScriptModule()
{
    return m_pScriptModule->getScriptModule();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
EventService::registerScriptEngine(pScriptEngine_type _pScriptEngine)
{
    // Only do this once.
    assert(m_pScriptModule == NULL);

    m_pScriptEngine = _pScriptEngine;

    static Zen::Scripting::script_module module(_pScriptEngine, "Event");

    // Keep a reference to the module.
    m_pScriptModule = &module;

    module.addType<I_EventService>(getScriptTypeName(), "Event Service")
        .addMethod("createEvent", &I_EventService::createEvent)
        .addMethod("getEvent", &I_EventService::getEvent)
        .addMethod("getEventQueue", &I_EventService::getEventQueue)
        .addMethod("getActionMap", &I_EventService::getActionMap)
        .createGlobalObject(m_name, this)
    ;

    // EventQueue
    module.addType<EventQueue>("EventQueue", "Event Queue")
        .addMethod("dispatchOneEvent", &EventQueue::dispatchOneEvent)
        .addMethod("dispatchAllEvents", &EventQueue::dispatchAllEvents)
    ;

    // ActionMap
    // Expose ActionMap instead of I_ActionMap because
    // we need to hook up createScriptAction().
    module.addType<ActionMap>("ActionMap", "Action Map")
        .addMethod("createAction", &ActionMap::createScriptAction)
        .addMethod("getAction", &ActionMap::getAction)
    ;

    // ScriptAction
    module.addType<I_Action>("Action", "Scriptable Action")
        // TR - I don't think this should be exposed
        //.addMethod("dispatch", &I_Action::dispatch)
    ;

    // Expose Event_impl instead of I_Event
    // because we need to hook up connectScript
    module.addType<Event_impl>("Event", "Event")
        .addMethod("connect", &Event_impl::connectScript)
        .addMethod("fireEvent", &Event_impl::fireEvent)
    ;

    module.addType<I_Connection>("Connection", "Connection between an Action and an Event")
        .addMethod("disconnect", &I_Connection::disconnect)
    ;

    module.activate();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_Event&
EventService::createEvent(const std::string& _name)
{
    // Since getEvent() will create the event if
    // it doesn't exist, just call it.
    return getEvent(_name);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_Event&
EventService::getEvent(const std::string& _name)
{
    Threading::CriticalSection guard(m_pEventsMutex);

    // TODO Make sure the event has been created.
    Events_type::iterator iter = m_events.find(_name);

    if (iter == m_events.end())
    {
        Event_impl* pEvent = new Event_impl(*this);

        m_events[_name] = pEvent;

        return *pEvent;
    }
    else
    {
        return *iter->second;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_EventQueue&
EventService::getEventQueue(const std::string& _queueName)
{
    Threading::CriticalSection guard(m_pEventQueuesMutex);

    EventQueues_type::iterator iter = m_eventQueues.find(_queueName);
    if (iter != m_eventQueues.end())
    {
        EventQueue* const pEventQueue = iter->second;
        assert(pEventQueue != NULL);
        return *pEventQueue;
    }

    EventQueue* pEventQueue = new EventQueue(*this);

    assert(pEventQueue != NULL);

    m_eventQueues[_queueName] = pEventQueue;

    return *pEventQueue;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_ActionMap&
EventService::getActionMap(const std::string& _actionMapName)
{
    Threading::CriticalSection guard(m_pActionMapsMutex);

    ActionMaps_type::iterator iter = m_actionMaps.find(_actionMapName);
    if( iter != m_actionMaps.end() )
    {
        return *iter->second;
    }

    ActionMap* pActionMap = new ActionMap(*this);
    m_actionMaps[_actionMapName] = pActionMap;

    return *pActionMap;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
EventService::getName() const
{
    return m_name;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Event
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
