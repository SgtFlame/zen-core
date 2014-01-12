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
#ifndef ZEN_EVENT_I_EVENT_SERVICE_HPP_INCLUDED
#define ZEN_EVENT_I_EVENT_SERVICE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_self_ref.hpp>

#include <Zen/Core/Scripting/I_ScriptableService.hpp>
#include <Zen/Core/Scripting/ObjectReference.hpp>

#include <boost/noncopyable.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Event {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_EventQueue;
class I_ActionMap;
class I_Event;

/// Event Service.
/// This is the top level service for handling event queues and action maps.
class EVENT_DLL_LINK I_EventService
:   public Scripting::I_ScriptableService
,   public Memory::managed_self_ref<I_EventService>
,   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<I_EventService>                 pScriptObject_type;
    typedef Scripting::ObjectReference<I_EventService>          ScriptObjectReference_type;
    typedef ScriptObjectReference_type                          ScriptWrapper_type;
    /// @}

    /// @name I_EventService interface
    /// @{
public:
    /// Create an event by name.
    virtual I_Event& createEvent(const std::string& _name) = 0;

    /// Get an event by name.
    virtual I_Event& getEvent(const std::string& _name) = 0;

    /// Get an event queue by name.
    /// There are two special event queues. "default" and "script".
    /// The default event queue is used if I_Event::connect() has a NULL
    /// for the event queue.  The script event queue is used as the default
    /// event queue for scripted actions.  If either of these
    /// event queues are created by either of these use cases, make
    /// sure you process events on those queues, otherwise the queues will
    /// fill up and consume memory.
    virtual I_EventQueue& getEventQueue(const std::string& _queueName) = 0;

    /// Get an action map by name.
    virtual I_ActionMap& getActionMap(const std::string& _actionMapName) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_EventService();
    virtual ~I_EventService();
    /// @}

};  // interface I_EventService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Event
namespace Memory {
    // I_ScriptEngine is managed by I_ScriptEngineFactory
    template<>
    struct is_managed_by_factory<Event::I_EventService> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_EVENT_I_EVENT_SERVICE_HPP_INCLUDED
