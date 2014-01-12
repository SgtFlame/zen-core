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
#ifndef ZEN_SCRIPTING_OBJECT_REFERENCE_HPP_INCLUDED
#define ZEN_SCRIPTING_OBJECT_REFERENCE_HPP_INCLUDED

#include <Zen/Core/Scripting/I_ObjectReference.hpp>
#include <Zen/Core/Scripting/I_ScriptObject.hpp>
#include <Zen/Core/Scripting/I_ScriptModule.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Scripting {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_ScriptType;

// TR This is the old way. See http://www.indiezen.org/wiki/ticket/90 for more info
//template<typename Object_type, typename pObject_type = Zen::Memory::managed_ptr<typename Object_type> >

/// Script wrapper for a C++ object.
/// @see http://www.indiezen.org/wiki/ticket/90
/// @todo Rename this to ScriptWrapper and change ScriptObjectReference_type to 
///     ScriptWrapper_type.
template<typename Object_type>
class ObjectReference
:   public I_ObjectReference
{
    /// @name Types
    /// @{
public:
    /// Type of pointer to the C++ object.  This might be a raw pointer or
    /// it may be a managed pointer.
    /// @note If you get an error "pScriptObject_type if not a member of <class>
    ///     then you need to declare a pScriptObject_type typedef that is the
    ///     type of pointer normally used for the object.  This lets the 
    ///     Scripting framework identify the type of pointer.  Normally this
    ///     is either a raw pointer or a managed pointer.
    typedef typename Object_type::pScriptObject_type            object_ptr_type;
    typedef Zen::Memory::managed_ptr<I_ScriptModule>            pScriptModule_type;
    typedef Zen::Memory::managed_ptr<I_ScriptType>              pScriptType_type;
    /// @}

    /// @name I_ObjectReference implementation
    /// @{
public:
    virtual pScriptModule_type getModule();
    virtual pScriptType_type getType();

    virtual pScriptObject_type getScriptObject();
    virtual void setScriptObject(pScriptObject_type _pObject);

    virtual void setScriptUserData(void* _pUserData);
    virtual void* getScriptUserData() const;
    /// @}

    /// @name ObjectReference implementation
    /// @{
public:
    object_ptr_type getObject();

    /// @brief Get the raw object.
    /// 
    /// This is provided as a convienience and should be used with care.
    /// The raw object lifetime is only guaranteed to be as long as
    /// this ObjectReference's lifetime, so don't keep a dangling 
    /// reference to this returned pointer.  Use getObject() if you
    /// need a managed pointer.
    Object_type* getRawObject();
    /// @}

    /// @name 'Structors
    /// @{
public:
             ObjectReference(pScriptModule_type _pModule, pScriptType_type _pType, object_ptr_type _pObject);
             ObjectReference(pScriptModule_type _pModule, pScriptType_type _pType, object_ptr_type _pObject, const std::string& _name);
    virtual ~ObjectReference();
    /// @}

    /// @name Member Variables
    /// @{
private:
    pScriptModule_type  m_pModule;
    pScriptType_type    m_pType;
    pScriptObject_type  m_pScriptObject;
    object_ptr_type     m_pObject;

    void*               m_pUserData;
    /// @}

};  // class ObjectReference

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename Object_type>
ObjectReference<Object_type>::ObjectReference(pScriptModule_type _pModule, pScriptType_type _pType, object_ptr_type _pObject)
:   m_pModule(_pModule)
,   m_pType(_pType)
,   m_pObject(_pObject)
,   m_pUserData(NULL)
{
    m_pModule->createObject(m_pType, this);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename Object_type>
ObjectReference<Object_type>::ObjectReference(pScriptModule_type _pModule, pScriptType_type _pType, object_ptr_type _pObject, const std::string& _name)
:   m_pModule(_pModule)
,   m_pType(_pType)
,   m_pObject(_pObject)
,   m_pUserData(NULL)
{
    m_pModule->createGlobalObject(_name, m_pType, this);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename Object_type>
ObjectReference<Object_type>::~ObjectReference()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename Object_type>
inline
Zen::Memory::managed_ptr<I_ScriptObject>
ObjectReference<Object_type>::getScriptObject()
{
    return m_pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename Object_type>
inline
void
ObjectReference<Object_type>::setScriptObject(pScriptObject_type _pScriptObject)
{
    m_pScriptObject = _pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename Object_type>
inline
void
ObjectReference<Object_type>::setScriptUserData(void* _pUserData)
{
    m_pUserData = _pUserData;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename Object_type>
inline
void*
ObjectReference<Object_type>::getScriptUserData() const
{
    return m_pUserData;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename Object_type>
inline
Zen::Memory::managed_ptr<I_ScriptModule>
ObjectReference<Object_type>::getModule()
{
    return m_pModule;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename Object_type>
inline
Zen::Memory::managed_ptr<I_ScriptType>
ObjectReference<Object_type>::getType()
{
    return m_pType;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename Object_type>
inline
typename ObjectReference<Object_type>::object_ptr_type
ObjectReference<Object_type>::getObject()
{
    return m_pObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename Object_type>
inline
Object_type*
ObjectReference<Object_type>::getRawObject()
{
    return m_pObject.get();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Scripting
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_SCRIPTING_OBJECT_REFERENCE_HPP_INCLUDED
