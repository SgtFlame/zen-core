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

#include "Application.hpp"

#include "XMLConfigurationElement.hpp"
#include "PluginManager.hpp"

#include <Zen/Core/Utility/I_LogService.hpp>
#include <Zen/Core/Utility/I_Log.hpp>
#include <Zen/Core/Utility/log_stream.hpp>

#include <boost/bind.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>



#include <iostream>

#include <string.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Plugins {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

Application::Application(Utility::I_LogManager::pLogService_type _pLogService)
:   m_logStreamBuffer(_pLogService->getStreamBuffer())
,   m_logStream(m_logStreamBuffer)
{
    m_config.listenElement("application",   boost::bind(&Application::onApplicationElement, this, _1));

    // TODO This should be listening to application/plugin-path, etc but that functionality doesn't exist yet
    // HACK For now I've hacked the XML parser to dispatch these.  Need to fix this.
    m_config.listenElement("plugin-path",   boost::bind(&Application::onPluginPathElement, this, _1));
    m_config.listenElement("module-path",   boost::bind(&Application::onModulePathElement, this, _1));
    m_config.listenElement("requires",      boost::bind(&Application::onRequiresElement, this, _1));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Application::~Application()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const I_Configuration&
Application::getConfiguration() const
{
    return m_config;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
Application::getId() const
{
    return m_id;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
Application::getName() const
{
    return m_name;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
Application::getVersion() const
{
    return m_version;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
Application::getProviderName() const
{
    return m_providerName;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Utility::log_stream&
Application::getLogStream()
{
    return m_logStream;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
Application::parseConfigurationFile(const boost::filesystem::path& _configFile)
{
    m_logStream << "DEBUG: Parsing configuration file: " << _configFile.string() << std::endl;

    // TODO Make sure the required elements are present, etc and return false
    //      if any errors occur.
    const bool rc = m_config.parse(_configFile);

    m_logStream << "DEBUG: Done parsing." << std::endl;
    m_logStream << "DEBUG: Application Id: " << getId() << std::endl;
    m_logStream << "DEBUG:           Name: " << getName() << std::endl;
    m_logStream << "DEBUG:        Version: " << getVersion() << std::endl;
    m_logStream << "DEBUG:       Provider: " << getProviderName() << std::endl;

    return rc;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Application::onApplicationElement(XMLConfigurationElement::ptr_type _pElement)
{
    // Get the basic application information
    setId(_pElement->getAttribute("id"));
    setName(_pElement->getAttribute("name"));
    setVersion(_pElement->getAttribute("version"));
    setProviderName(_pElement->getAttribute("provider-name"));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Application::onPluginPathElement(XMLConfigurationElement::ptr_type _pElement)
{
    // Get the plugin path and inform the plugin manager.
    boost::filesystem::path pluginPath = 
        boost::filesystem::system_complete(
                boost::filesystem::path(_pElement->getAttribute("path"))
            ).normalize();

    PluginManager::getSingleton().setPluginPath(pluginPath);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Application::onModulePathElement(XMLConfigurationElement::ptr_type _pElement)
{
    // Get the module path and inform the plugin manager.
    boost::filesystem::path modulePath = 
        boost::filesystem::system_complete(
                boost::filesystem::path(_pElement->getAttribute("path"))
            ).normalize();

    PluginManager::getSingleton().setModulePath(modulePath);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Application::onRequiresElement(XMLConfigurationElement::ptr_type _pElement)
{
    XMLConfigurationElement::config_list_type::iterator iter;

    // Get the list of "import"ed plugins.
    XMLConfigurationElement::config_list_ptr_type pImportList = _pElement->getChildren("import");

    // Iterate through the imported plugin list and ask the PluginManager to install them.
    for(iter = pImportList->begin(); iter != pImportList->end(); ++iter)
    {
        const std::string& requiredPluginName = (*iter)->getAttribute("plugin");

        PluginManager::getSingleton().installPlugin(requiredPluginName);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Application::setId(const std::string& _id)
{
    m_id = _id;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Application::setName(const std::string& _name)
{
    m_name = _name;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Application::setVersion(const std::string& _version)
{
    m_version = _version;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Application::setProviderName(const std::string& _providerName)
{
    m_providerName = _providerName;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Plugins
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
