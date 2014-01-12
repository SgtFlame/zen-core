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
#ifndef ZEN_EVENT_EVENT_QUEUE_HPP_INCLUDED
#define ZEN_EVENT_EVENT_QUEUE_HPP_INCLUDED

#include "../I_EventQueue.hpp"

#include <Zen/Core/Threading/SynchronizedQueue.hpp>

#include <set>
#include <map>
#include <deque>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Threading {
        class I_Mutex;
    }   // namespace Threading
namespace Event {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class EventService;
class I_Action;
class Event_impl;

class EventQueue
:   public I_EventQueue
{
    /// @name Types
    /// @{
public:
    typedef Scripting::I_ScriptModule::pScriptType_type     pScriptType_type;
    typedef std::pair<Event_impl*, boost::any>              Event_type;
    typedef Threading::SynchronizedQueue<Event_type>        EventQueue_type;

    typedef Memory::managed_ptr<Scripting::I_ScriptModule>  pScriptModule_type;
    /// @}

    /// @name I_EventQueue implementation
    /// @{
public:
    virtual bool dispatchOneEvent();
    virtual void dispatchAllEvents(bool _wait);
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual const std::string& getScriptTypeName();
    virtual Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name EventQueue implementation
    /// @{
public:
    /// Queue an event for dispatching.
    void queueEvent(Event_impl* _pEvent, boost::any _argument);

    pScriptModule_type getScriptModule();
    /// @}

    /// @name 'Structors
    /// @{
public:
             EventQueue(EventService& _parent);
    virtual ~EventQueue();
    /// @}

    /// @name Member variables
    /// @{
private:
    EventService&                   m_parent;

    Scripting::I_ObjectReference*   m_pScriptObject;

    EventQueue_type                 m_eventQueue;

    // @}

};  // class EventQueue

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Event
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_EVENT_EVENT_QUEUE_HPP_INCLUDED
