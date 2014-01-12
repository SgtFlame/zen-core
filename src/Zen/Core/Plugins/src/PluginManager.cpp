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
#include "PluginManager.hpp"
#include "PluginInfo.hpp"
#include "XMLConfigurationElement.hpp"
#include "Application.hpp"
#include "Extension.hpp"
#include "ExtensionRegistry.hpp"

#include <Zen/Core/Plugins/I_ModuleManager.hpp>
#include <Zen/Core/Plugins/I_ModuleService.hpp>
#include <Zen/Core/Plugins/I_Module.hpp>
#include <Zen/Core/Plugins/I_Plugin.hpp>
#include <Zen/Core/Utility/runtime_exception.hpp>
#include <Zen/Core/Utility/I_LogManager.hpp>
#include <Zen/Core/Utility/I_LogService.hpp>
#include <Zen/Core/Utility/I_Log.hpp>

#include <boost/bind.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/pointer_cast.hpp>

#include <sstream>
#include <iostream>
#include <stdexcept>

#include <stddef.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Plugins {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

PluginManager::PluginManager()
:   m_specialCharacter(0)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PluginManager::~PluginManager()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
boost::shared_ptr<I_Application>
PluginManager::installApplication(const boost::filesystem::path& _configFile,
                                  const boost::filesystem::path& _logFile)
{
    m_pApplication.reset();

    // Initialize the default logging service.
    // TODO Don't hardcode log name and params.
    Utility::I_LogManager::pLogService_type pLogService = 
        Utility::I_LogManager::getSingleton().create("zenLogService");
    pLogService->createLog(_logFile.string());

    // Need the raw pointer; this is safe since this line and the next won't throw
    // an exception.
    Application* const pApplication = new Application(pLogService);
    m_pApplication = boost::shared_ptr<I_Application>(pApplication);

    pApplication->parseConfigurationFile(_configFile);

    return m_pApplication;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
boost::shared_ptr<I_Application>
PluginManager::getApplication()
{
    return m_pApplication;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PluginManager::setRootPath(const char _ch, const boost::filesystem::path& _rootPath)
{
    m_specialCharacter = _ch;
    m_rootPath = _rootPath;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PluginManager::setPluginPath(const boost::filesystem::path& _pluginPath)
{
    if (m_specialCharacter != 0 && _pluginPath.string().c_str()[0] == m_specialCharacter)
    {
        boost::filesystem::path pluginPath =
            boost::filesystem::system_complete(
            m_rootPath / boost::filesystem::path(&_pluginPath.string().c_str()[1]));
        m_pluginsSearchPath.push_back(pluginPath);
        m_pApplication->getLogStream() << "DEBUG: setPluginPath using wildcard " << pluginPath.string() << std::endl;
    }
    else
    {
        m_pApplication->getLogStream() << "DEBUG: setPluginPath " << _pluginPath.string() << std::endl;
        m_pluginsSearchPath.push_back(_pluginPath);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PluginManager::setModulePath(const boost::filesystem::path& _modulePath)
{
    if (m_specialCharacter != 0 && _modulePath.string().c_str()[0] == m_specialCharacter)
    {
        boost::filesystem::path modulePath =
            boost::filesystem::system_complete(
            m_rootPath / boost::filesystem::path(&_modulePath.string().c_str()[1]));
        I_ModuleManager::getSingleton().addPath(modulePath);
        m_pApplication->getLogStream() << "DEBUG: setModulePath using wildcard " << modulePath.string() << std::endl;
    }
    else
    {
        m_pApplication->getLogStream() << "DEBUG: setModulePath " << _modulePath.string() << std::endl;
        I_ModuleManager::getSingleton().addPath(_modulePath);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_PluginManager::plugin_info_ptr_type
PluginManager::installPlugin(const std::string& _moduleName)
{
    // Make sure this plugin hasn't already been installed.
    plugin_info_collection_type::iterator foundPlugin = m_pluginInfos.find(_moduleName);
    if (foundPlugin != m_pluginInfos.end())
    {
        return foundPlugin->second;
    }

    // TODO Log Debug
    m_pApplication->getLogStream() << "DEBUG: installing plugin " << _moduleName << std::endl;
    boost::shared_ptr<PluginInfo> pPluginInfo(new PluginInfo());

    // Search the plugin search path for the correct plugin.xml file
    boost::filesystem::path pluginConfigurationPath;
    bool bPluginConfigFound = false;

    plugin_search_path_type::iterator pathIter;
    for(pathIter = m_pluginsSearchPath.begin(); pathIter != m_pluginsSearchPath.end(); pathIter++)
    {
        pluginConfigurationPath = (*pathIter) /
            boost::filesystem::path(_moduleName) /
            boost::filesystem::path("plugin.xml");

        m_pApplication->getLogStream() << "DEBUG: Searching for file " << pluginConfigurationPath.string() << std::endl;

        if (boost::filesystem::exists(pluginConfigurationPath))
        {
            bPluginConfigFound = true;
            break;
        }
    }

    // Error, the plugin.xml file wasn't found
    if (!bPluginConfigFound)
    {
        std::stringstream errorMessage;
        errorMessage << "Error finding " << _moduleName << "/plugin.xml";
        m_pApplication->getLogStream() << "DEBUG: " << errorMessage.str() << std::endl;
        throw Zen::Utility::runtime_exception(errorMessage.str());
    }
    else if (pPluginInfo->parseConfigurationFile(pluginConfigurationPath))
    {
        m_pluginInfoList.push_back(pPluginInfo);
        m_pluginInfos[pPluginInfo->getName()] = pPluginInfo;

        // Get all of the extension points defined by this plugin
        const std::list<boost::shared_ptr<ExtensionPoint> >& extensionPoints(pPluginInfo->getExtensionPoints());

        // Parse the schemas for each extension point
        for(std::list<boost::shared_ptr<ExtensionPoint> >::const_iterator iter = extensionPoints.begin(); iter != extensionPoints.end(); ++iter)
        {
            boost::filesystem::path schemaPath = (*pathIter) /
                boost::filesystem::path(_moduleName) /
                boost::filesystem::path((*iter)->getSchemaFileName());

            // TODO Log Debug
            m_pApplication->getLogStream() << "DEBUG: Parsing Schema: " << schemaPath.string() << std::endl;

            //if (XMLConfigurationElement::parse(schemaPath, boost::bind(&ExtensionPoint::addSchemaEntry, (*iter).get(), _1)))
            if ((*iter)->parseSchema(schemaPath))
            {
                // Extension point schema was correctly parsed so it's safe to install it.
                ExtensionRegistry::getSingleton().installExtensionPoint(*iter);
            }
            else
            {
                // Extension point schema was incorrectly parsed
                pPluginInfo.reset();

                std::stringstream errorMessage;
                errorMessage << "Error parsing schema " << (*iter)->getSchemaFileName() << " for plugin " << _moduleName << ".  File: " << schemaPath.string();
                m_pApplication->getLogStream() << errorMessage.str();
                throw Zen::Utility::runtime_exception(errorMessage.str());
            }
        }

        // Before installing any extensions, install all of the dependent plugins
        installRequiredPlugins(pPluginInfo);

        // Get all of the extensions defined by this plugin
        const std::list<boost::shared_ptr<Extension> >& extensions(pPluginInfo->getExtensions());

        // Install the extensions
        for(std::list<boost::shared_ptr<Extension> >::const_iterator iter = extensions.begin(); iter != extensions.end(); ++iter)
        {
            // TODO Is this all that needs to be done?

            ExtensionRegistry::getSingleton().installExtension(*iter);
        }
    }
    else
    {
        // On failure, pPluginInfo is reset to NULL
        pPluginInfo.reset();
    }

    // Return the result as an I_PluginInfo shared pointer
    return boost::dynamic_pointer_cast<I_PluginInfo>(pPluginInfo);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PluginManager::installRequiredPlugins(boost::shared_ptr<PluginInfo> _pluginInfo)
{
    const std::list<std::string>& requiredPlugins = _pluginInfo->getRequiredPlugins();

    // Iterate through the required plugin list
    for(std::list<std::string>::const_iterator iter = requiredPlugins.begin(); iter != requiredPlugins.end(); ++iter)
    {
        // Check to see if the plugin has already been installed
        plugin_info_collection_type::iterator foundPlugin = m_pluginInfos.find(*iter);

        // If not installed yet, install it.
        if (foundPlugin == m_pluginInfos.end())
        {
            // TODO What to do with the resulting plugin info pointer?
            installPlugin(*iter);
        }
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const I_PluginManager::plugin_info_list_type&
PluginManager::enumeratePlugins() const
{
    return m_pluginInfoList;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#if 0   // deprecated
I_PluginManager::plugin_ptr_type
PluginManager::loadPlugin(plugin_info_ptr_type _pPluginInfo)
{
    return loadPlugin(_pPluginInfo.get());
}
#endif

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#if 0   // deprecated
I_PluginManager::plugin_ptr_type
PluginManager::getPlugin(plugin_info_ptr_type _pPluginInfo)
{
    return getPlugin(_pPluginInfo.get());
}
#endif

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#if 0   // deprecated
boost::shared_ptr<I_ClassFactory>
PluginManager::getClassFactory(Extension* const _pExtension)
{
    boost::shared_ptr<I_Plugin> pPlugin(_pExtension->getPluginInfo().getPlugin());

    return pPlugin->getClassFactory(_pExtension->getConfigurationElement());
}
#endif

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
boost::shared_ptr<I_Plugin>
PluginManager::getPlugin(PluginInfo* const _pPluginInfo)
{
    // TODO Log Debug
    m_pApplication->getLogStream() << "DEBUG: Getting plugin " << _pPluginInfo->getName() << std::endl;

    plugin_ptr_type pPlugin;

    plugin_collection_type::iterator iter = m_plugins.find(_pPluginInfo->getName());
    if (iter != m_plugins.end())
    {
        return iter->second;
    }
    else
    {
        // TODO Log Debug
        m_pApplication->getLogStream() << "DEBUG: Oops, " << _pPluginInfo->getName() << " isn't loaded yet. Loading..." << std::endl;

        // This plugin hasn't been loaded yet, so load it.
        return loadPlugin(_pPluginInfo);
    }

    return pPlugin;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
boost::shared_ptr<I_Plugin>
PluginManager::loadPlugin(PluginInfo* const _pPluginInfo)
{
    // TODO First make sure the plugin dependencies are loaded.
    // Since the dependencies are installed, maybe this isn't necessary?

    I_PluginManager::plugin_ptr_type pPlugin;

    // Do a dynamic cast to make sure this plugin info is the right type.
    PluginInfo* const pPluginInfo = dynamic_cast<PluginInfo*>(_pPluginInfo);

    // Dynamic cast failed so return a NULL plugin.
    if (pPluginInfo == NULL)
    {
        throw Zen::Utility::runtime_exception("PluginManager::loadPlugin(): dynamic_cast of I_PluginInfo type failed.");
    }

    // TODO Don't assume this is a dynamic plugin
    I_ModuleManager::service_ptr_type pModuleService = I_ModuleManager::getSingleton().getService(I_ModuleManager::DYNAMIC);

    // Load the module.
    I_ModuleService::module_ptr_type pModule = pModuleService->load(_pPluginInfo->getName());

    if (pModule != NULL)
    {
        pPlugin = pModule->getPlugin(pPluginInfo->getName());

        // Save the indexed plugin
        m_plugins[pPluginInfo->getName()] = pPlugin;

        pPluginInfo->setWantsToLoad(true);
        pPluginInfo->setLoaded(true);
    }
    else
    {
        // TODO Log Debug
        m_pApplication->getLogStream() << "DEBUG: Error loading module " << _pPluginInfo->getName() << std::endl;
    }

    return pPlugin;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Plugins
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
