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
#ifndef ZEN_SCRIPTING_SCRIPT_TYPE_HPP_INTERFACE_INCLUDED
#define ZEN_SCRIPTING_SCRIPT_TYPE_HPP_INTERFACE_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <string>
#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Scripting {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_ScriptMethod;
class I_ScriptModule;
class script_module;

/// Script Type interface.
/// This class is used as the base interface for script_type.
/// The primary purpose is to expose the common activate() method
/// and allow script_module to contain a collection of script_type.
class SCRIPTING_DLL_LINK script_type_interface
{
    /// @name Types
    /// @{
public:
    typedef std::map<std::string, I_ScriptMethod*>          Methods_type;
    typedef Zen::Memory::managed_ptr<I_ScriptModule>        pScriptModule_type;
    /// @}

    /// @name script_type_interface interface
    /// @{
private:
    friend class script_module;

    /// Activate this script type.
    /// When a script_type is activated, it registers itself with
    /// the script engine.
    /// This method is called by script_module::activate().
    virtual void activate() = 0;

    /// Create the global objects.
    virtual void createGlobals() = 0;

public:
    /// Get the script_module<> for this type.
    virtual script_module& getModule() = 0;

    /// Get the methods that have been added so far.
    virtual Methods_type& getMethods() = 0;
    /// @}

    /// @name 'Structors
    /// @{
public:
             script_type_interface();
    virtual ~script_type_interface();
    /// @}

};  // interface script_type_interface

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Scripting
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_SCRIPTING_SCRIPT_TYPE_HPP_INTERFACE_INCLUDED
