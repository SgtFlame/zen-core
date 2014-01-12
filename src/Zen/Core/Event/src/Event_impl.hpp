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
#ifndef ZEN_EVENT_EVENT_IMPL_HPP_INCLUDED
#define ZEN_EVENT_EVENT_IMPL_HPP_INCLUDED

#include "../I_Event.hpp"

#include <set>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Event {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class EventQueue;
class EventService;
class Connection_impl;

/// @todo rename to Event when Event gets renamed to event<>
class EVENT_DLL_LINK Event_impl
:   public I_Event
{
    /// @name Types
    /// @{
public:
    typedef Threading::I_Mutex*                         pMutex_type;
    typedef boost::any                                  Parm1_type;
    typedef Zen::Memory::managed_ptr<I_Action>          pAction_type;
    typedef Connection_impl                             Connection_type;
    typedef Connection_type*                            pConnection_type;
    typedef std::list<pConnection_type>	                container_type;
    typedef Zen::Memory::managed_ptr<Scripting::I_ScriptModule> pScriptModule_type;
    typedef std::set<EventQueue*>                       Queues_type;
    /// @}

    /// @name I_Event implementation
    /// @{
public:
    virtual I_Connection& connect(pAction_type _pAction, I_EventQueue* _pQueue = NULL);
    virtual void fireEvent(boost::any _argument);
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual const std::string& getScriptTypeName();
    virtual Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name Event implementation
    /// @{
public:
    /// Connect a script action.
    I_Connection& connectScript(boost::any _scriptClass, boost::any _scriptFunction);

    pScriptModule_type getScriptModule();
protected:
    friend class Connection_impl;
    /// Should only be called by the Connection_impl
    void disconnect(Connection_impl* _pConnection);

protected:
    friend class EventQueue;
    /// Dispatch this event.
    /// This is called by the EventQueue.
    void dispatch(boost::any _argument);

    void destroyScriptAction(pAction_type::weak_ptr_type _pAction);
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class EventService;
             Event_impl(EventService& _service);
    virtual ~Event_impl();
    /// @}

    /// @name Member Variables
    /// @{
private:
    /// Service that owns this event.
    EventService&                       m_service;

    /// Connections for this event.
    container_type	                m_connections;

    Queues_type                         m_queues;

    /// Mutex to guard m_connections and m_queues
    pMutex_type                         m_pMutex;

    /// Script Object wrapper.
    Scripting::I_ObjectReference*       m_pScriptObject;
    /// @}

};  // class Event

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Event
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_EVENT_EVENT_IMPL_HPP_INCLUDED
