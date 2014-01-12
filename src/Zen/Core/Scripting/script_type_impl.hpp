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
#ifndef ZEN_SCRIPTING_SCRIPT_TYPE_IMPL_HPP_INCLUDED
#define ZEN_SCRIPTING_SCRIPT_TYPE_IMPL_HPP_INCLUDED

#include <Zen/Core/Scripting/forward_declarations.hpp>
#include <Zen/Core/Scripting/script_dispatch_helper.hpp>
#include <Zen/Core/Scripting/script_method.hpp>

#include <boost/typeof/typeof.hpp>


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Scripting {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

template<typename ScriptableClass_type>
inline
script_type<ScriptableClass_type>::script_type(script_module& _module, const std::string& _typeName, const std::string& _documentation)
:   m_pModule(&_module)
,   m_pScriptModule()
,   m_typeName(_typeName)
,   m_documentation(_documentation)
,   m_methods()
,   m_activated(false)
,   m_pScriptType()
,   m_globalObjects()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename ScriptableClass_type>
inline
script_type<ScriptableClass_type>::script_type(pScriptType_type _pScriptType)
:   m_pModule(NULL)
,   m_pScriptModule(_pScriptType->getScriptModule())
,   m_typeName(_pScriptType->getTypeName())
,   m_documentation(_pScriptType->getDocumentation())
,   m_methods()
,   m_activated(false)
,   m_pScriptType(_pScriptType)
,   m_globalObjects()
{
    Methods_type::iterator methodIter = m_methods.begin();
    while (methodIter != m_methods.end())
    {
        delete methodIter->second;
        methodIter++;
    }
    m_methods.clear();

    GlobalObjects_type::iterator objIter = m_globalObjects.begin();
    while (objIter != m_globalObjects.end())
    {
        delete objIter->second;
        objIter++;
    }
    m_globalObjects.clear();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename ScriptableClass_type>
template<typename Method_type>
script_type<ScriptableClass_type>&
script_type<ScriptableClass_type>::addMethod(const std::string& _methodName, Method_type _pFunction)
{
    typedef BOOST_TYPEOF((get_dispatch_helper(_pFunction, _pFunction))) DispatchHelper_type;
    typedef typename DispatchHelper_type::MethodReturn_type             MethodReturn_type;

    typedef script_method<ScriptableClass_type, Method_type, MethodReturn_type, DispatchHelper_type>
                                                                        ScriptMethod_type;

    I_ScriptMethod* const pMethod = new ScriptMethod_type(_pFunction, get_dispatch_helper(_pFunction, _pFunction), m_typeName, _methodName);

    m_methods[_methodName] = pMethod;

    return *this;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename ScriptableClass_type>
template<typename Method_type>
script_type<ScriptableClass_type>&
script_type<ScriptableClass_type>::addConstMethod(const std::string& _methodName, Method_type _pFunction)
{
    typedef BOOST_TYPEOF((get_const_dispatch_helper(_pFunction, _pFunction))) DispatchHelper_type;
    typedef typename DispatchHelper_type::MethodReturn_type             MethodReturn_type;

    typedef script_method<ScriptableClass_type, Method_type, MethodReturn_type, DispatchHelper_type>
                                                                        ScriptMethod_type;

    I_ScriptMethod* const pMethod = new ScriptMethod_type(_pFunction, get_const_dispatch_helper(_pFunction, _pFunction), m_typeName, _methodName);

    m_methods[_methodName] = pMethod;

    return *this;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename ScriptableClass_type>
inline
void
script_type<ScriptableClass_type>::activate()
{
    assert(!m_activated);

    // Create the m_pScriptType in the OO framework
    m_pScriptType = getScriptModule()->createScriptType(m_typeName, m_documentation, 0);

    // Iterate through the script_method and register them.
    for(Methods_type::iterator iter = m_methods.begin(); iter != m_methods.end(); iter++)
    {
        // TODO I_ScriptMethod needs to support documentation.  For now the second
        // parameter is simply an empty string.
        m_pScriptType->addMethod(iter->first, "", iter->second);
    }

    m_activated = true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename ScriptableClass_type>
inline
void
script_type<ScriptableClass_type>::createGlobals()
{
    // Create the global objects
    for(GlobalObjects_type::iterator iter = m_globalObjects.begin(); iter != m_globalObjects.end(); iter++)
    {
        getScriptModule()->createGlobalObject(iter->first, m_pScriptType, iter->second->getScriptObject());
    }

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename ScriptableClass_type>
inline
void
script_type<ScriptableClass_type>::createGlobalObject(const std::string& _objectName, I_ScriptableType* _pScriptableObject)
{
    // Defer the creation of the object until after the module has been activated.
    // For now, keep a collection of global objects, and when the module
    // is activated, create them using createGlobals()
    m_globalObjects[_objectName] = _pScriptableObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename ScriptableClass_type>
inline
Zen::Memory::managed_ptr<I_ScriptModule>
script_type<ScriptableClass_type>::getScriptModule()
{
    if(m_pModule)
    {
        return m_pModule->getScriptModule();
    }
    else
    {
        return m_pScriptModule.lock();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename ScriptableClass_type>
inline
script_module&
script_type<ScriptableClass_type>::getModule()
{
    return *m_pModule;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename ScriptableClass_type>
inline
std::map<std::string, I_ScriptMethod*>&
script_type<ScriptableClass_type>::getMethods()
{
    return m_methods;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename ScriptableClass_type>
inline
derived_script_type<ScriptableClass_type>
::derived_script_type(script_type_interface& _baseType, script_module& _module, const std::string& _typeName, const std::string& _documentation)
:   script_type<ScriptableClass_type>(_module, _typeName, _documentation)
,   m_baseType(_baseType)
{
    // Remember that m_pScriptModule may be NULL if the base type's module
    // has not been activated yet.

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename ScriptableClass_type>
inline
void
derived_script_type<ScriptableClass_type>
::activate()
 {
    // First, call the base class to create the script type and
    // then register all of the methods that have been added.

    script_type<ScriptableClass_type>::activate();

    // Next, iterate through all of the methods from the base type
    // and register them.

    Methods_type::iterator iter;

    for(iter =
            this->m_baseType.getMethods().begin();
            iter != this->m_baseType.getMethods().end();
            iter++)
    {
        // TODO I_ScriptMethod needs to support documentation.  For now the second
        // parameter is simply an empty string.
        this->m_pScriptType->addMethod(iter->first, "", iter->second);
    }

 }

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Scripting
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_SCRIPTING_SCRIPT_TYPE_IMPL_HPP_INCLUDED
