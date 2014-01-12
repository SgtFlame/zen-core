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
#include "ActionMap.hpp"
#include "EventService.hpp"
#include "ScriptAction.hpp"
#include "Action.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Core/Threading/I_Mutex.hpp>
#include <Zen/Core/Threading/MutexFactory.hpp>
#include <Zen/Core/Threading/CriticalSection.hpp>

#include <Zen/Core/Scripting/I_ScriptType.hpp>

#include <Zen/Core/Event/I_Action.hpp>
#include <Zen/Core/Event/I_Connection.hpp>

#include <boost/bind.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Event {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ActionMap::ActionMap(EventService& _parent)
:   m_parent(_parent)
,   m_pActionMapMutex(Threading::MutexFactory::create())
,   m_pScriptObject(NULL)
,   m_actionMap()
//,   m_pActionScriptType(NULL)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ActionMap::~ActionMap()
{
    Threading::MutexFactory::destroy(m_pActionMapMutex);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static std::string sm_typeName("ActionMap");
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
ActionMap::getScriptTypeName()
{
    return sm_typeName;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_Action&
ActionMap::createAction(const std::string& _name, ActionFunction_type _function)
{
    Threading::CriticalSection guard(m_pActionMapMutex);
    // TODO Guard?

    Action* pRawAction = new Action(getScriptObject()->getModule(), _function);

    pAction_type pAction(pRawAction, boost::bind(&ActionMap::destroyStandardAction, this, _1));
    m_actionMap[_name] = pAction;

    return *pAction;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ActionMap::destroyAction(const std::string& _name)
{
    class ConnectionVisitor
    :   public I_Action::I_ConnectionVisitor
    {
    public:
        virtual void begin() {}

        virtual void visit(I_Connection& _connection)
        {
            _connection.disconnect();
        }

        virtual void end() {}

    };  // class ConnectionVisitor

    ActionMap_type::iterator iter = m_actionMap.find(_name);
    if (iter != m_actionMap.end())
    {
        ConnectionVisitor visitor;
        iter->second->getConnections(visitor);
        m_actionMap.erase(iter);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_Action&
ActionMap::operator [](const std::string& _name)
{
    return getAction(_name);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Scripting::I_ObjectReference*
ActionMap::getScriptObject()
{
    if (m_pScriptObject == NULL)
    {
        m_pScriptObject = new ScriptWrapper_type(
            m_parent.getScriptModule(),
            m_parent.getScriptModule()->getScriptType(getScriptTypeName()),
            this
        );
    }

    return m_pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_Action&
ActionMap::createScriptAction(const std::string& _name, boost::any _function, boost::any _object)
{
    Threading::CriticalSection guard(m_pActionMapMutex);

    ScriptAction* pRawScriptAction = new ScriptAction(getScriptObject()->getModule(), _object, _function);

    pAction_type pScriptAction(pRawScriptAction, boost::bind(&ActionMap::destroyScriptAction, this, _1));
    m_actionMap[_name] = pScriptAction;

    return *pScriptAction;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ActionMap::destroyScriptAction(pAction_type::weak_ptr_type _wpAction)
{
    ScriptAction* pAction = dynamic_cast<ScriptAction*>(_wpAction.get());

    delete pAction;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ActionMap::destroyStandardAction(pAction_type::weak_ptr_type _wpAction)
{
    Action* pAction = dynamic_cast<Action*>(_wpAction.get());

    delete pAction;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_Action&
ActionMap::getAction(const std::string& _name)
{
    ActionMap_type::iterator iter = m_actionMap.find(_name);
    if( iter != m_actionMap.end() )
    {
        return *iter->second;
    }
    // TODO Throw exception?
    //return pAction_type();
    throw Zen::Utility::runtime_exception("ActionMap::getAction() : Action is not in map.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
ActionMap::actionExists(const std::string& _name)
{
    return m_actionMap.find(_name) != m_actionMap.end();
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Event
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
