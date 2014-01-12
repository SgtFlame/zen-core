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
#ifndef ZEN_SCRIPTING_I_OBJECT_REFERENCE_HPP_INCLUDED
#define ZEN_SCRIPTING_I_OBJECT_REFERENCE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Scripting {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_ScriptObject;
class I_ScriptModule;
class I_ScriptType;

/// Helper for mapping C++ objects to the I_ScriptObject
class SCRIPTING_DLL_LINK I_ObjectReference
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_ObjectReference>         pObjectReference_type;
    typedef Zen::Memory::managed_weak_ptr<I_ObjectReference>    wpObjectReference_type;
    typedef Zen::Event::Event<wpObjectReference_type>           objectReferenceEvent_type;

    typedef Zen::Memory::managed_ptr<I_ScriptObject>            pScriptObject_type;
    typedef Zen::Memory::managed_ptr<I_ScriptModule>            pScriptModule_type;
    typedef Zen::Memory::managed_ptr<I_ScriptType>              pScriptType_type;
    /// @}

    /// @name I_ObjectReference interface
    /// @{
public:
    virtual pScriptModule_type getModule() = 0;
    virtual pScriptType_type getType() = 0;

    virtual pScriptObject_type getScriptObject() = 0;
    virtual void setScriptObject(pScriptObject_type _pObject) = 0;

    virtual void setScriptUserData(void* _pUserData) = 0;
    virtual void* getScriptUserData() const = 0;
    /// @}

    /// @name Events
    /// @{
public:
    objectReferenceEvent_type   onDestroyEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ObjectReference();
    virtual ~I_ObjectReference();
    /// @}
};  // interface I_ObjectReference

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Scripting
namespace Memory {
    // I_ObjectReference is managed by I_ObjectHeap
    template<>
    struct is_managed_by_factory<Scripting::I_ObjectReference> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_SCRIPTING_OBJECT_HEAP_HPP_INCLUDED
