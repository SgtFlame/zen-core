//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Core Framework
//
// Copyright (C) 2001 - 2010 Tony Richards
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
#ifndef ZEN_SCRIPTING_I_SCRIPT_MODULE_HPP_INCLUDED
#define ZEN_SCRIPTING_I_SCRIPT_MODULE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Memory/managed_self_ref.hpp>

#include <Zen/Core/Event/Event.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Scripting {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_ScriptEngine;
class I_ScriptType;
class I_ObjectReference;

/// A module within the script engine.
/// Some script engines use this as a namespace.
/// 
/// Not all script engines support modules or namespaces
/// so just ignore this layer in those cases, but you still need to create
/// the modules nonetheless.
class SCRIPTING_DLL_LINK I_ScriptModule
:   public Memory::managed_self_ref<I_ScriptModule>
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_ScriptModule>        pScriptModule_type;
    typedef Zen::Memory::managed_weak_ptr<I_ScriptModule>   wpScriptModule_type;
    typedef Zen::Event::Event<wpScriptModule_type>          scriptModuleEvent_type;

    typedef Zen::Memory::managed_ptr<I_ScriptType>          pScriptType_type;
    typedef Zen::Memory::managed_ptr<I_ObjectReference>     pObjectReference_type;

    typedef Zen::Memory::managed_ptr<I_ScriptEngine>        pScriptEngine_type;
    /// @}

    /// @name I_ScriptModule interface
    /// @{
public:
    /// Create a type or class.
    /// @param _typeName name of the type as exposed to the scripting language.  Some languages
    ///         prepend the module name (such as Python).
    /// @param _docString documentation string that is used by the scripting language if
    ///         the language supports it.
    /// @param _rawSize Size of the structure that contains the underlying C / C++ struct or object.
    virtual pScriptType_type createScriptType(const std::string& _typeName, const std::string& _docString, unsigned long _rawSize) = 0;

    /// Get a type or class by name.
    /// The type must have already been created by calling I_ScriptModule::createScriptType().
    virtual pScriptType_type getScriptType(const std::string& _typeName) = 0;

    /// Activate the module.  No more methods, types or attributes can be added
    /// or modified once the module has been activated.
    virtual void activate() = 0;

    /// Create a new script object and binds it to a C++ object.
    /// @param _name Name of the object
    /// @param _pType Type of the object
    /// @param _pObject C++ version of the object
    virtual void createObject(pScriptType_type _pType, I_ObjectReference* _pObject) = 0;

    /// Create a new global script object and binds it to a C++ object.
    /// @param _name Name of the object
    /// @param _pType Type of the object
    /// @param _pObject C++ version of the object
    virtual void createGlobalObject(const std::string& _name, pScriptType_type _pType, I_ObjectReference* _pObject) = 0;

    virtual pScriptEngine_type getScriptEngine() = 0;
    /// @}

    /// @name Events
    /// @{
public:
    scriptModuleEvent_type  onDestroyEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ScriptModule();
    virtual ~I_ScriptModule();
    /// @}

};  // interface I_ScriptModule

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Scripting
namespace Memory {
    // I_ScriptModule is managed by I_ScriptEngine
    template<>
    struct is_managed_by_factory<Scripting::I_ScriptModule> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_SCRIPTING_I_SCRIPT_MODULE_HPP_INCLUDED
