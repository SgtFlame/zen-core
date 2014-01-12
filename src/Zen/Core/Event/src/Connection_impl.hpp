//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Core Framework
//
// Copyright (C) 2001 - 2010 Tony Richards
// Copyright (C) 2008        Matthew Alan Gray
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
#ifndef ZEN_EVENT_MODELING_CONNECTION_IMPL_HPP_INCLUDED
#define ZEN_EVENT_MODELING_CONNECTION_IMPL_HPP_INCLUDED

#include <Zen/Core/Scripting/I_ScriptModule.hpp>

#include <Zen/Core/Event/I_Connection.hpp>
#include <Zen/Core/Event/I_Action.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Event {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class Event_impl;

/// Connection implementation.
/// This is the non-template implementation that's used with 
/// I_EventService.
class Connection_impl
:   public I_Connection
{
    /// @name Types
    /// @{
public:
    typedef boost::any                              Parm1_type;
    typedef Zen::Memory::managed_ptr<I_Action>      pAction_type;
    typedef Event_impl*                             pEvent_type;
    typedef Memory::managed_ptr<Scripting::I_ScriptModule> pScriptModule_type;
    /// @}

    /// @name I_Connection implementation
    /// @{
public:
    virtual void disconnect();
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual const std::string& getScriptTypeName();
    virtual Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name Connection_impl implementation
    /// @{
public:
    void operator()(Parm1_type _parm);
    void block();
    void unblock();
    pScriptModule_type getScriptModule();
    pAction_type getAction();
    /// @}

    /// @name 'Structors
    /// @{
public:
    Connection_impl(pEvent_type _pEvent, pAction_type _pAction);

protected:
    friend class Event_impl;

    /// This can only be called by Event_impl:: disconnect()
    virtual ~Connection_impl();
    /// @}

    /// @name Member variables
    /// @{
private:
    pEvent_type     m_pEvent;
    pAction_type    m_pAction;
    bool            m_isBlocked;
    bool            m_isDetached;

    Scripting::I_ObjectReference*       m_pScriptObject;
    /// @}

};  // class Connection

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Event
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_EVENT_MODELING_CONNECTION_HPP_INCLUDED
