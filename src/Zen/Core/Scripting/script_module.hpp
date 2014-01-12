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
#ifndef ZEN_SCRIPTING_SCRIPT_MODULE_HPP_INCLUDED
#define ZEN_SCRIPTING_SCRIPT_MODULE_HPP_INCLUDED

#include <Zen/Core/Scripting/forward_declarations.hpp>

#include <Zen/Core/Scripting/I_ScriptEngine.hpp>
#include <Zen/Core/Scripting/I_ScriptModule.hpp>

#include <boost/noncopyable.hpp>

#include <string>
#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Scripting {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/// Helper class for defining script modules.
/// This class is exposed publicly so that you can derive additional
/// implementations from it or so you can use it as an example if
/// you want to implement it a different way.
class script_module
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_ScriptEngine>                    pScriptEngine_type;
    typedef Zen::Memory::managed_ptr<I_ScriptModule>                    pScriptModule_type;

    typedef std::map<std::string, script_type_interface*>               ScriptTypes;
    /// @}

    /// @name script_module implementation
    /// @{
public:
    /// Add a script type to this module.
    /// A script type is a "type", "table" or "class" depending on the terminology
    /// used by the underlying scripting engine.
    template<typename ScriptableClass_type>
    script_type<ScriptableClass_type>&
    addType(const std::string& _typeName, const std::string& _typeDoc);

    template<typename ScriptableClass_type>
    derived_script_type<ScriptableClass_type>&
    addDerivedType(script_type_interface& _baseType, const std::string& _typeName, const std::string& _typeDoc);

    /// Activate this module.
    /// When a module is activated, all of the classes within the module are
    /// finalized and registered.  You cannot add more classes to the module and
    /// you cannot add more methods to existing classes after this method is called.
    void activate();

    /// Get the underlying I_ScriptModule class.
    pScriptModule_type getScriptModule();

    script_type_interface& getScriptType(const std::string& _typeName);
    /// @}

    /// @name 'Structors
    /// @{
public:
             script_module(pScriptEngine_type _pScriptEngine, const std::string& _name, const std::string& _documentation = "");
    virtual ~script_module();
    /// @}

private:
    pScriptEngine_type          m_pScriptEngine;
    std::string                 m_name;
    std::string                 m_documentation;

    /// true if this module has been activated.
    /// Should this be part of I_ScriptModule instead?
    bool                        m_activated;

    /// Script module that this script_module wraps.
    pScriptModule_type          m_pModule;

    ScriptTypes                 m_scriptTypes;
};  // class script_module

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Scripting
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_SCRIPTING_SCRIPT_MODULE_HPP_INCLUDED
