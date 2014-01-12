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
#ifndef ZEN_SCRIPTING_I_SCRIPT_ENGINE_HPP_INCLUDED
#define ZEN_SCRIPTING_I_SCRIPT_ENGINE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>

#include <boost/any.hpp>

#include <vector>
#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Scripting {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_ObjectHeap;
class I_ScriptModule;

class SCRIPTING_DLL_LINK I_ScriptEngine
{
    /// @name Types
    /// @{
public:
    typedef std::string                                     index_type;
    typedef Zen::Memory::managed_ptr<I_ScriptEngine>        pScriptEngine_type;
    typedef Zen::Memory::managed_weak_ptr<I_ScriptEngine>   wpScriptEngine_type;
    typedef Zen::Event::Event<wpScriptEngine_type>          scriptEngineEvent_type;

    typedef Zen::Memory::managed_ptr<I_ScriptModule>        pScriptModule_type;
    /// @}

    /// @name I_ScriptEngine interface
    /// @{
public:
    virtual I_ObjectHeap& heap() = 0;

    /// Execute a script
    /// @return true if the script was sucesfully executed, otherwise false
    virtual bool executeScript(const std::string& _fileName) = 0;

    /// Execute a method on an object
    /// @param _object An object passed from script
    /// @param _method A method passed from script
    /// @param _parms Parameters that will be passed to the method
    virtual void executeMethod(boost::any& _object, boost::any& _method, std::vector<boost::any>& _parms) = 0;

    /// Create a script module
    virtual pScriptModule_type createScriptModule(const std::string& _moduleName, const std::string& _docString) = 0;
    /// @}

    /// @name Static methods
    /// @{
    static const std::string& getNamespace();
    static const std::string& getExtensionPointName();
    /// @}

    /// @name Events
    /// @{
public:
    scriptEngineEvent_type  onDestroyEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ScriptEngine();
    virtual ~I_ScriptEngine();
    /// @}

};  // interface I_ScriptEngine

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Scripting
namespace Memory {
    // I_ScriptEngine is managed by I_ScriptEngineFactory
    template<>
    struct is_managed_by_factory<Scripting::I_ScriptEngine> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_SCRIPTING_I_SCRIPT_ENGINE_HPP_INCLUDED
