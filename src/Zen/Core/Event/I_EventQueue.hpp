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
#ifndef ZEN_EVENT_I_EVENT_QUEUE_HPP_INCLUDED
#define ZEN_EVENT_I_EVENT_QUEUE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Core/Scripting/I_ScriptableType.hpp>
#include <Zen/Core/Scripting/I_ScriptableService.hpp>
#include <Zen/Core/Scripting/ObjectReference.hpp>

#include <boost/any.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Event {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_Action;

class EVENT_DLL_LINK I_EventQueue
:   public Scripting::I_ScriptableType
{
    /// @name Types
    /// @{
public:
    typedef I_EventQueue*                                               pScriptObject_type;
    typedef Scripting::ObjectReference<I_EventQueue>                    ScriptObjectReference_type;
    typedef ScriptObjectReference_type                                  ScriptWrapper_type;

    typedef Zen::Memory::managed_ptr<I_Action>                          pAction_type;
    /// @}

    /// @name I_EventQueue interface
    /// @{
public:
    /// Dispatch the next event in this queue.
    /// Call this from the thread that needs to dispatch this
    /// event queue.
    /// @return False if no events are remaining.
    virtual bool dispatchOneEvent() = 0;

    /// Dispatch all of the events that have been queued.
    /// Call this from the thread that needs to dispatch this
    /// event queue.
    /// @param _wait True if this method should block until at
    ///     least one event is dispatched.  If this parameter
    ///     is false, you must always call it from the same thread.
    virtual void dispatchAllEvents(bool _wait) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_EventQueue();
    virtual ~I_EventQueue();
    /// @}

};  // interface I_EventQueue

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Event
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_EVENT_I_EVENT_QUEUE_HPP_INCLUDED
