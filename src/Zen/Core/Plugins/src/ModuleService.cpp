//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Core Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
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

#include "ModuleService.hpp"
#include "ModuleInfo.hpp"

#include <Zen/Core/Plugins/I_Module.hpp>
#include <Zen/Core/Plugins/I_ModuleInfo.hpp>
#include <Zen/Core/Plugins/I_ModuleManager.hpp>
#include <Zen/Core/Threading/MutexFactory.hpp>
#include <Zen/Core/Threading/CriticalSection.hpp>
#include <Zen/Core/Plugins/I_PluginManager.hpp>
#include <Zen/Core/Plugins/I_Application.hpp>

#include <Zen/Core/Utility/GetLastError.hpp>
#include <Zen/Core/Utility/log_stream.hpp>


#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <iostream>
#include <sstream>

#include <stddef.h>

#ifdef __APPLE__
#include <dlfcn.h>
#endif // __APPLE__

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Plugins {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

ModuleService::ModuleService()
{
    // Acquire a mutex
    m_pModuleGuard = Threading::MutexFactory::create();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ModuleService::~ModuleService()
{
    // Release a mutex
    Threading::MutexFactory::destroy(m_pModuleGuard);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_ModuleService::module_ptr_type
ModuleService::load(const std::string& _moduleName)
{
    // Get the logger stream
    Zen::Utility::log_stream& logStream(
        I_PluginManager::getSingleton().getApplication()->getLogStream()
    );

    // Guard this method
    boost::shared_ptr<Threading::CriticalSection> pGuard(
        new Threading::CriticalSection(m_pModuleGuard) );

    module_name_index_type::iterator iter = m_moduleIndex.find(_moduleName);
    if(iter != m_moduleIndex.end())
    {
        // Increment the reference count
        m_modules[iter->second]->incrementReferences();

        // Return the
        return iter->second;
    }

    module_info_ptr_type pModuleInfo(new ModuleInfo);

    I_ModuleManager* pModuleManager = &I_ModuleManager::getSingleton();

    boost::filesystem::path modulePath;
    if(!pModuleManager->findPath(_moduleName, modulePath))
    {
        logStream << "DEBUG: Module " << _moduleName << " not found in defined module search paths." << std::endl;

        // TODO: Throw an exception with the error
        return NULL;
    }

    pModuleInfo->setName(_moduleName);

#ifdef _WIN32
    I_ModuleInfo::module_handle_type hModule = LoadLibraryA(modulePath.string().c_str());
#else
    logStream << "DEBUG: dlopen " << modulePath.string().c_str() << std::endl;
    I_ModuleInfo::module_handle_type hModule = dlopen(modulePath.string().c_str(), RTLD_NOW | RTLD_GLOBAL);
#endif // _WIN32

    if (hModule == NULL)
    {
        int err = Zen::Utility::GetLastError();

        logStream << "DEBUG: Error loading module " << modulePath.string()
#ifndef _WIN32
        << dlerror()
#else
            << (err == 14001 ? "probably the module has dependencies not on the path.  Use depends.exe to figure it out." : "")
#endif
            << std::endl;


        // TODO: Throw an exception with the error
        return NULL;
#if 0
        std::stringstream errorStream;
        errorStream << "Unable to find plugin module: " << moduleName.str();
        pPluginInfo->setError(errorStream.str());
#endif
    }

    pModuleInfo->setHandle(hModule);

    // Get the "getModule" procedure
#if   defined _WIN32
    FARPROC proc = GetProcAddress(hModule, "getModule");
#else
    void* proc = dlsym(hModule, "getModule");
#endif

    // Check to make sure the procedure exists
    if (proc)
    {
        // Convert the procedure type to the assumed type
#ifdef _WIN32
        I_Module::proc_ptr_type pRealProc = (I_Module::proc_ptr_type)proc;
#else
        I_Module::QUERY_MODULE_FUNCTION_PTR pRealProc = reinterpret_cast<I_Module::QUERY_MODULE_FUNCTION_PTR>(proc);
#endif

        // Execute the procedure to get the I_Module for this module.
        module_ptr_type pModule = &(pRealProc());

        // Set the reference count to 1
        pModuleInfo->incrementReferences();

        // Put it in the index
        m_moduleIndex[_moduleName] = pModule;

        // Put it in the cache
        m_modules[pModule] = pModuleInfo;

        // And return it.
        return pModule;
    }
    else
    {
        logStream << "DEBUG: Error getting procedure address in module " << modulePath.string() << " Error " << Zen::Utility::GetLastError() << std::endl;

        // Not found, so return NULL
        return NULL;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ModuleService::unload(module_ptr_type _pModule)
{
    // Get the logger stream
    Zen::Utility::log_stream& logStream(
        I_PluginManager::getSingleton().getApplication()->getLogStream()
    );

    // Guard this method
    boost::shared_ptr<Threading::CriticalSection> pGuard(
        new Threading::CriticalSection(m_pModuleGuard) );

    // Use _pModule to get the module info
    module_info_ptr_type pModuleInfo = m_modules[_pModule];

    // Decrement the reference
    // If the count is zero, get the handle and unload the library
    if( pModuleInfo->decrementReferences() == 0 )
    {
        std::string moduleName = pModuleInfo->getName();

        // Remove module from the index
        module_name_index_iterator_type moduleIndexIterator = m_moduleIndex.find(moduleName);
        m_moduleIndex.erase(moduleIndexIterator);

        // Physically unload module
        I_ModuleInfo::module_handle_type hModule = pModuleInfo->getHandle();

#ifdef _WIN32
        if( !FreeLibrary(hModule) )
#else
        if( dlclose(hModule) )
#endif // _WIN32
        {
            logStream << "DEBUG: Error unloading module " << moduleName << " Error " << Zen::Utility::GetLastError() << std::endl;

            // TODO Throw an exception with the error
            return;
        }

        // Remove module from the cache
        modules_iterator_type moduleIterator = m_modules.find(_pModule);
        m_modules.erase(moduleIterator);

    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}	// namespace Plugins
}	// namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
