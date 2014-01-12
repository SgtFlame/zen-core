//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Core Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C) 2008 - 2009 Matthew Alan Gray
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
#ifndef ZEN_SCRIPTING_I_SCRIPTABLE_TYPE_HPP_INCLUDED
#define ZEN_SCRIPTING_I_SCRIPTABLE_TYPE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Core/Scripting/I_ScriptModule.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Scripting {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_ObjectReference;

/// @brief Implement this in classes that are scriptable
/// @todo Rename this to I_Scriptable
class SCRIPTING_DLL_LINK I_ScriptableType
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<I_ScriptModule>     pScriptModule_type;
    /// @}

    /// @name I_ScriptableType interface
    /// @{
public:
    /// Get the name that the Script system uses for this class
    /// The base framework interfaces implement this, but if you
    /// want to create a derived class, override this method.
    virtual const std::string& getScriptTypeName() = 0;

    /// @brief Get the script object associated with this object
    /// @return The script object associated with this object, but possibly NULL if this object
    ///         was created before a script engine was registered.
    virtual I_ObjectReference* getScriptObject() = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ScriptableType();
public:
    virtual ~I_ScriptableType();
    /// @}

};  // interface I_ScriptableType

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Scripting
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_SCRIPTING_I_SCRIPTABLE_TYPE_HPP_INCLUDED
