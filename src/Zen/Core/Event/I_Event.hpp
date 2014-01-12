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
#ifndef ZEN_EVENT_I_EVENT_HPP_INCLUDED
#define ZEN_EVENT_I_EVENT_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Scripting/I_ScriptableService.hpp>
#include <Zen/Core/Scripting/ObjectReference.hpp>

#include <boost/any.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Event {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_Connection;
class I_Action;
class I_EventQueue;

/// @brief Event interface
class EVENT_DLL_LINK I_Event
:   public Scripting::I_ScriptableType
{
    /// @name Types
    /// @{
public:
    typedef I_Event*                                pScriptObject_type;
    typedef Scripting::ObjectReference<I_Event>     ScriptObjectReference_type;
    typedef Scripting::ObjectReference<I_Event>     ScriptWrapper_type;
    typedef Memory::managed_ptr<I_Action>           pAction_type;
    /// @}

    /// @name I_Event interface
    /// @{
public:
    /// Connect an action to an event.
    /// @param _pAction Action that will be invoked when the event is fired.
    /// @param _queue The event queue that will be used to dispatch the action.
    ///         If this parameter is NULL, the default event queue is used.
    /// @return I_Connection that represents the connected event and action.
    ///         Use this to disconnect the connection.
    virtual I_Connection& connect(pAction_type _pAction, I_EventQueue* _pQueue = NULL) = 0;

    /// Fire an event.
    /// This does not necessarily dispatch immediately if
    /// the event is connected to an asynchronous event queue.
    /// @param _argument must be copyable / assignable and 
    ///     if the lifetime of the value is in question then
    ///     it should be a reference counted pointer
    ///     such as a managed_ptr or shared_ptr.
    virtual void fireEvent(boost::any _argument) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_Event();
    virtual ~I_Event();
    /// @}

};  // interface I_Event

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Event
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_EVENT_I_EVENT_HPP_INCLUDED
