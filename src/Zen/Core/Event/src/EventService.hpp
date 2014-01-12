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
#ifndef ZEN_EVENT_EVENT_SERVICE_HPP_INCLUDED
#define ZEN_EVENT_EVENT_SERVICE_HPP_INCLUDED

#include "../I_EventService.hpp"

#include <Zen/Core/Scripting/script_module.hpp>

#include <map>
#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Event {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class ActionMap;
class EventQueue;
class Event_impl;

/// Event Service implementation.
/// This class implements the basic event service.  Since I_EventService is not
/// exposed as an extension point, this is likely to be the only implementation.
class EventService
:   public I_EventService
{
    /// @name Types
    /// @{
public:
    typedef std::map<std::string, EventQueue*>          EventQueues_type;
    typedef std::map<std::string, ActionMap*>           ActionMaps_type;
    typedef std::map<std::string, Event_impl*>          Events_type;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual const std::string& getScriptTypeName();
    virtual Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name I_ScriptableService implementation
    /// @{
public:
    virtual void registerScriptEngine(pScriptEngine_type _pScriptEngine);
    /// @}

    /// @name I_EventService implementation
    /// @{
public:
    virtual I_Event& createEvent(const std::string& _name);
    virtual I_Event& getEvent(const std::string& _name);
    virtual I_EventQueue& getEventQueue(const std::string& _queueName);
    virtual I_ActionMap& getActionMap(const std::string& _actionMapName);
    /// @}

    /// @name EventService implementation
    /// @{
public:
    pScriptModule_type getScriptModule();
    const std::string& getName() const;
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class EventManager;
             EventService(const std::string& _name);
    virtual ~EventService();
    /// @}

    /// @name Member Variables
    /// @{
private:
    std::string                         m_name;
    pScriptEngine_type                  m_pScriptEngine;
    Zen::Scripting::script_module*      m_pScriptModule;
    Scripting::I_ObjectReference*       m_pScriptObject;

    EventQueues_type                    m_eventQueues;
    ActionMaps_type                     m_actionMaps;

    /// Collection of all registered events.
    Events_type                         m_events;

    /// Mutex for guarding m_events
    Threading::I_Mutex*                 m_pEventsMutex;

    /// Mutex for guarding m_eventQueues
    Threading::I_Mutex*                 m_pEventQueuesMutex;

    /// Mutex for guarding m_actionMaps
    Threading::I_Mutex*                 m_pActionMapsMutex;

    /// @}

};  // class EventService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Event
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_EVENT_EVENT_SERVICE_HPP_INCLUDED
