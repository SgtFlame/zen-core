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

#include "PluginInfo.hpp"

#include "PluginManager.hpp"
#include "ExtensionRegistry.hpp"

#include <Zen/Core/Threading/MutexFactory.hpp>
#include <Zen/Core/Threading/I_Mutex.hpp>
#include <Zen/Core/Threading/CriticalSection.hpp>

#include <boost/bind.hpp>

#include <stddef.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Plugins {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

PluginInfo::PluginInfo()
:   m_isLoaded(false)
,   m_wantsToLoad(false)
//,   m_hModule(NULL)
,   m_pGuard(Threading::MutexFactory::create())
,   m_pRawPlugin(NULL)
{
    m_config.listenElement("plugin", boost::bind(&PluginInfo::onPluginElement, this, _1));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PluginInfo::PluginInfo(const std::string& _name, const std::string& _description)
:   m_name(_name)
,   m_description(_description)
,   m_isLoaded(false)
,   m_wantsToLoad(false)
{
    Threading::MutexFactory::destroy(m_pGuard);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PluginInfo::~PluginInfo()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
PluginInfo::getName() const
{
    return m_name;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
PluginInfo::getDescription() const
{
    return m_description;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
PluginInfo::isLoaded() const
{
    return m_isLoaded;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
PluginInfo::getLoadError() const
{
    return m_error;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PluginInfo::setError(const std::string& _errorMessage)
{
    m_error = _errorMessage;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PluginInfo::setWantsToLoad(bool _wantsToLoad)
{
    m_wantsToLoad = _wantsToLoad;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
PluginInfo::getWantsToLoad() const
{
    return m_wantsToLoad;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PluginInfo::setLoaded(bool _isLoaded)
{
    m_isLoaded = _isLoaded;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#if 0   // deprecated
void
PluginInfo::setModule(HMODULE _hModule)
{
    m_hModule = _hModule;
}
#endif

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
PluginInfo::parseConfigurationFile(const boost::filesystem::path& _configFile)
{
    // TODO Make sure the required elements are present, etc and return false
    //      if any errors occur.
    return m_config.parse(_configFile);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PluginInfo::onPluginElement(XMLConfigurationElement::ptr_type _pElement)
{
    m_name = _pElement->getAttribute("id");
    m_description = _pElement->getAttribute("name");
    m_version = _pElement->getAttribute("version");
    m_provider = _pElement->getAttribute("provider-name");

    // Get the runtime information
    const I_ConfigurationElement* const pRuntime = _pElement->getChild("runtime");
    if (pRuntime)
    {
        const I_ConfigurationElement* const pLibrary = pRuntime->getChild("library");

        if (pLibrary)
        {
            m_moduleName = pLibrary->getAttribute("name");
        }
    }

    // Get the list of dependencies / required plugins
    const I_ConfigurationElement* const pRequires = _pElement->getChild("requires");
    if (pRequires)
    {
        I_ConfigurationElement::config_list_ptr_type pImportList(pRequires->getChildren("import"));

        for(I_ConfigurationElement::config_list_type::iterator iter = pImportList->begin(); iter != pImportList->end(); ++iter)
        {
            m_requiredPlugins.push_back((*iter)->getAttribute("plugin"));
        }
    }

    // Get the list of extension points
    I_ConfigurationElement::config_list_ptr_type pExtensionPoints(_pElement->getChildren("extension-point"));
    for(I_ConfigurationElement::config_list_type::iterator iter = pExtensionPoints->begin(); iter != pExtensionPoints->end(); ++iter)
    {
        // Install the extension point
        boost::shared_ptr<ExtensionPoint> pExtensionPoint(ExtensionRegistry::getSingleton().createExtensionPoint(*(*iter), *this));

        m_extensionPoints.push_back(pExtensionPoint);
    }

    // Get the list of extensions implemented in this plugin
    I_ConfigurationElement::config_list_ptr_type pExtensions(_pElement->getChildren("extension"));
    for(I_ConfigurationElement::config_list_type::iterator iter = pExtensions->begin(); iter != pExtensions->end(); ++iter)
    {
        // Install the extension
        boost::shared_ptr<Extension> pExtension(ExtensionRegistry::getSingleton().createExtension(*(*iter), *this));

        m_extensions.push_back(pExtension);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
boost::shared_ptr<I_Plugin>
PluginInfo::getPlugin()
{
    if (m_pRawPlugin == NULL)
    {
        Threading::CriticalSection lock(m_pGuard);

        if(m_pRawPlugin == NULL)
        {
            m_plugin = PluginManager::getSingleton().getPlugin(this);
            m_pRawPlugin = m_plugin.get();
        }
    }

    return m_plugin;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::list<boost::shared_ptr<ExtensionPoint> >&
PluginInfo::getExtensionPoints() const
{
    return m_extensionPoints;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::list<boost::shared_ptr<Extension> >&
PluginInfo::getExtensions() const
{
    return m_extensions;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::list<std::string>&
PluginInfo::getRequiredPlugins() const
{
    return m_requiredPlugins;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Plugins
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
