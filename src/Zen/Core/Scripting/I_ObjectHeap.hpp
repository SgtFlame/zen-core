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
#ifndef ZEN_SCRIPTING_I_OBJECT_HEAP_HPP_INCLUDED
#define ZEN_SCRIPTING_I_OBJECT_HEAP_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Scripting {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_ObjectReference;

/// @note TR - I don't think we're actively using this anymore.
///		It was originally part of the implementation of the Java -> C++
///		JNI layer, but we have not been maintaining that and it doesn't
///		even exist in this repository (it's in the old "framework" repo).
class SCRIPTING_DLL_LINK I_ObjectHeap
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_ObjectHeap>          pObjectHeap_type;
    typedef Zen::Memory::managed_weak_ptr<I_ObjectHeap>     wpObjectHeap_type;
    typedef Zen::Event::Event<wpObjectHeap_type>            objectHeapEvent_type;

    typedef Zen::Memory::managed_ptr<I_ObjectReference>     pObjectReference_type;
    /// @}

    /// @name I_ObjectHeap interface
    /// @{
public:
    /// Create a new object based on the object reference.
    /// This really doesn't do anything except maintain the
    /// reference count of the C++ wrapped object.
    virtual void createObject(pObjectReference_type _pObject) = 0;

    /// Get the global reference object from the heap that is the same
    /// as the reference passed as a parameter.
    virtual pObjectReference_type getGlobalReference(I_ObjectReference& _object) = 0;
    /// @}

    /// @name Events
    /// @{
public:
    objectHeapEvent_type   onDestroyEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ObjectHeap();
    virtual ~I_ObjectHeap();
    /// @}

};  // interface I_ObjectHeap

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Scripting
namespace Memory {
    // I_ObjectHeap is managed by I_ScriptEngine
    template<>
    struct is_managed_by_factory<Scripting::I_ObjectHeap> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_SCRIPTING_I_OBJECT_HEAP_HPP_INCLUDED
