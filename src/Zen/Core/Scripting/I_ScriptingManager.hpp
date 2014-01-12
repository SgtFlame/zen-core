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
#ifndef ZEN_SCRIPTING_I_SCRIPTING_MANAGER_HPP_INCLUDED
#define ZEN_SCRIPTING_I_SCRIPTING_MANAGER_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Scripting {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_ScriptEngine;
class I_ScriptModule;

class SCRIPTING_DLL_LINK I_ScriptingManager
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_ScriptEngine>    pScriptEngine_type;
    typedef Zen::Memory::managed_ptr<I_ScriptModule>    pScriptModule_type;
    /// @}

    /// @name I_ScriptingManager interface
    /// @{
public:
    /// Create a script engine by name
    /// @param _name The name of a script engine.  Currently supported names
    ///             include "python" and "lua" but it's possible that other
    ///             scripting plugins can be created.
    /// @return NULL if the engine isn't found, otherwise the engine is returned
    ///             The engine must be destroyed by calling destroyScriptEngine
    virtual pScriptEngine_type createScriptEngine(const std::string& _name) = 0;
    /// @}

    /// @name Static Methods
    /// @{
public:
    static I_ScriptingManager& getSingleton();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ScriptingManager();
    virtual ~I_ScriptingManager();
    /// @}

};  // interface I_ScriptingManager

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Scripting
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_SCRIPTING_I_SCRIPTING_MANAGER_HPP_INCLUDED
