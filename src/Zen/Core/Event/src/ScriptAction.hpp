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
#ifndef ZEN_EVENT_SCRIPT_ACTION_HPP_INCLUDED
#define ZEN_EVENT_SCRIPT_ACTION_HPP_INCLUDED

#include "../I_Action.hpp"
#include "../I_ActionMap.hpp"

#include <list>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Event {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class ActionMap;

/// Action that is handled by script.
class ScriptAction
:   public I_Action
{
    /// @name Types
    /// @{
public:
    typedef I_ActionMap::ActionFunction_type                    ActionFunction_type;
    typedef Zen::Memory::managed_ptr<Scripting::I_ScriptModule> pScriptModule_type;
    /// @}

    /// @name I_Action implementation
    /// @{
public:
    virtual void dispatch(boost::any& _parameter);
    virtual void addConnection(I_Connection& _connection);
    virtual void removeConnection(I_Connection& _connection);
    virtual void getConnections(I_ConnectionVisitor& _visitor);
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class Event_impl;
    friend class ActionMap;
             ScriptAction(pScriptModule_type _pScriptModule, boost::any _scriptObject, boost::any _scriptFunction);
    virtual ~ScriptAction();
    /// @}

    /// @name Member Variables
    /// @{
private:
    pScriptModule_type                  m_pScriptModule;
    Scripting::I_ObjectReference*       m_pScriptObject;
    boost::any                          m_scriptObject;
    boost::any                          m_scriptFunction;

    /// TODO Use an intrusive list here?
    typedef std::list<I_Connection*>    Connections_type;
    Connections_type                    m_connections;
    /// @}

};  // class ScriptAction

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Event
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_EVENT_SCRIPT_ACTION_HPP_INCLUDED
