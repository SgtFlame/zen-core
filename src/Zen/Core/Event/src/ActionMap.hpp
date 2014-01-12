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
#ifndef ZEN_EVENT_ACTION_MAP_HPP_INCLUDED
#define ZEN_EVENT_ACTION_MAP_HPP_INCLUDED

#include "../I_ActionMap.hpp"

#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Threading {
        class I_Mutex;
    }   // namespace Threading
namespace Event {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class EventService;

class ActionMap
:   public I_ActionMap
{
    /// @name Types
    /// @{
public:
    typedef ActionMap*                                      pScriptObject_type;
    typedef Scripting::ObjectReference<ActionMap>           ScriptObjectReference_type;
    typedef ScriptObjectReference_type                      ScriptWrapper_type;

    typedef Scripting::I_ScriptModule::pScriptType_type     pScriptType_type;
    typedef std::map<std::string, pAction_type>             ActionMap_type;
    /// @}

    /// @name I_ActionMap implementation
    /// @{
public:
    virtual I_Action& createAction(const std::string& _name, ActionFunction_type _function);
    virtual void destroyAction(const std::string& _name);
    virtual bool actionExists(const std::string& _name);
    virtual I_Action& getAction(const std::string& _name);
    virtual I_Action& operator[](const std::string& _name);
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual const std::string& getScriptTypeName();
    virtual Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name ActionMap implementation
    /// @{
public:
    /// Script specific create action.
    /// The order of parameters here is correct.  _function first, then _object.
    /// Need to stay consistent with boost::bind() and if anything is optional, the object
    /// is optional, not the function.
    I_Action& createScriptAction(const std::string& _name, boost::any _function, boost::any _object);

    void destroyScriptAction(pAction_type::weak_ptr_type _wpAction);

    void destroyStandardAction(pAction_type::weak_ptr_type _wpAction);
    /// @}

    /// @name Event handlers
    /// @{
public:
    void onDestroyAction(wpAction_type _pAction);
    /// @}

    /// @name 'Structors
    /// @{
public:
             ActionMap(EventService& _parent);
    virtual ~ActionMap();
    /// @}

    /// @name Member Variables
    /// @{
private:
    EventService&                           m_parent;

    Threading::I_Mutex*                     m_pActionMapMutex;

    Scripting::I_ObjectReference*           m_pScriptObject;

    /// Primary action map
    ActionMap_type                          m_actionMap;

    //pScriptType_type                        m_pActionScriptType;

    /// @}

};  // class ActionMap

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Event
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_EVENT_ACTION_MAP_HPP_INCLUDED
