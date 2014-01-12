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
#include "ModuleManager.hpp"
#include "ModuleService.hpp"

#include <Zen/Core/Plugins/I_PluginManager.hpp>
#include <Zen/Core/Plugins/I_Application.hpp>

#include <Zen/Core/Utility/log_stream.hpp>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <iostream>
#include <sstream>

#include <stddef.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Plugins {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

ModuleManager::ModuleManager()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ModuleManager::~ModuleManager()
{
    m_modulePaths.clear();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static ModuleService sm_moduleService;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_ModuleManager::service_ptr_type
ModuleManager::getService(MODULE_SERVICE_TYPE _serviceType)
{
    // Return the correct ModuleService
    return &sm_moduleService;

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ModuleManager::addPath(const boost::filesystem::path& _modulePath)
{
    m_modulePaths.push_back(_modulePath);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ModuleManager::dropPath(const boost::filesystem::path& _modulePath)
{
    // Get the logger stream
    Zen::Utility::log_stream& logStream(
        I_PluginManager::getSingleton().getApplication()->getLogStream()
    );

    module_paths_iterator_type iter;

    iter = m_modulePaths.begin();
    while( (*iter != _modulePath) &&
           (iter  != m_modulePaths.end()) )
    {
        iter++;
    }

    if( iter != m_modulePaths.end() )
        m_modulePaths.erase(iter);
    else
    {
        logStream << "DEBUG: Path " << _modulePath << " is not in the list of module search paths." << std::endl;

        // TODO Throw an exception with the error.
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
ModuleManager::findPath(const std::string _moduleName, boost::filesystem::path &_modulePath)
{
    // Get the logger stream
    Zen::Utility::log_stream& logStream(
        I_PluginManager::getSingleton().getApplication()->getLogStream()
    );

    bool modulePathFound = false;
    boost::filesystem::path modulePath;
    module_paths_iterator_type iter;

    if( m_modulePaths.empty() )
        return false;

    // Set the extension
    std::stringstream moduleName;

#if   defined _WIN32 || HOST_CYGWIN
    moduleName << _moduleName;
#ifdef _DEBUG
    moduleName << "_d";
#endif
    moduleName << ".dll";
#elif defined HOST_DARWIN
    moduleName << "lib" << _moduleName << ".dylib";
#else
    moduleName << "lib" << _moduleName << ".so";
#endif


    iter = m_modulePaths.begin();
    do
    {

        // Create the fully qualified path
        modulePath = *iter / boost::filesystem::path(moduleName.str());

        // Check if the path is valid
        modulePathFound = boost::filesystem::exists(modulePath);

        logStream << "DEBUG: attempting " << modulePath.string() << std::endl;

        // Iterate path iterator
        iter++;

    }
    while( iter != m_modulePaths.end() &&
           !modulePathFound );

    if( modulePathFound )
    {
        _modulePath = modulePath;
        return true;
    }
    else
    {
        _modulePath = std::string("");
        return false;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}	// namespace Plugins
}	// namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

