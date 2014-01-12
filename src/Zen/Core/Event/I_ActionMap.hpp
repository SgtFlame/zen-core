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
#ifndef ZEN_EVENT_I_ACTION_MAP_HPP_INCLUDED
#define ZEN_EVENT_I_ACTION_MAP_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Core/Scripting/I_ScriptableType.hpp>
#include <Zen/Core/Scripting/ObjectReference.hpp>

#include <boost/any.hpp>
#include <boost/function.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Event {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_Action;

/// @brief Action interface.
class EVENT_DLL_LINK I_ActionMap
:   public Scripting::I_ScriptableType
{
    /// @name Types
    /// @{
public:
    typedef boost::function<void(boost::any&)>                          ActionFunction_type;

    typedef Zen::Memory::managed_ptr<I_Action>                          pAction_type;
    typedef Zen::Memory::managed_weak_ptr<I_Action>                     wpAction_type;
    /// @}

    /// @name I_ActionMap interface
    /// @{
public:
    /// Create an action by name and bind it to a function that gets envoked when
    /// the action occurs.
    virtual I_Action& createAction(const std::string& _name, ActionFunction_type _function) = 0;

    /// Destroy an action and clear all of its existing connections.
    virtual void destroyAction(const std::string& _name) = 0;

    /// Indicates if an action exists or not.
    /// @return true if the action exists, otherwise returns false.
    virtual bool actionExists(const std::string& _name) = 0;

	/// Get an action by name.
    virtual I_Action& getAction(const std::string& _name) = 0;

    /// Get an action by name.
    /// This operator overload is a convenience operator that
    /// simply calls getAction(_name).
    virtual I_Action& operator[](const std::string& _name) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ActionMap();
    virtual ~I_ActionMap();
    /// @}

};  // interface I_ActionMap


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Event
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_EVENT_I_ACTION_MAP_HPP_INCLUDED
