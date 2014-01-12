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
#ifndef ZEN_PLUGINS_PLUGIN_MANAGER_HPP_INCLUDED
#define ZEN_PLUGINS_PLUGIN_MANAGER_HPP_INCLUDED

#include <Zen/Core/Plugins/I_PluginManager.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/filesystem/path.hpp>

#include <string>
#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Plugins {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class Application;
class I_ClassFactory;
class Extension;
class PluginInfo;
class ExtensionPoint;

class PluginManager
:   public I_PluginManager
{
    /// @name Types
    /// @{
public:
    typedef boost::shared_ptr<I_Plugin>                 plugin_ptr_type;
    typedef boost::shared_ptr<I_PluginInfo>             plugin_info_ptr_type;
    typedef std::map<std::string, plugin_ptr_type>      plugin_collection_type;
    typedef std::map<std::string, plugin_info_ptr_type> plugin_info_collection_type;
    typedef boost::filesystem::path                     plugin_path_type;
    typedef std::list<plugin_path_type>                 plugin_search_path_type;
    /// @}

    /// @name I_PluginManager implementation
    /// @{
public:
    virtual app_ptr_type installApplication(const boost::filesystem::path& _configFile, const boost::filesystem::path& _logFile);
    virtual app_ptr_type getApplication();
    virtual void setRootPath(const char _ch, const boost::filesystem::path& _rootPath);
    virtual void setPluginPath(const boost::filesystem::path& _pluginPath);
    virtual void setModulePath(const boost::filesystem::path& _modulePath);

    virtual plugin_info_ptr_type installPlugin(const std::string& _pluginName);

    virtual const plugin_info_list_type& enumeratePlugins() const;

    //virtual plugin_ptr_type loadPlugin(plugin_info_ptr_type _pPluginInfo);
    //virtual plugin_ptr_type getPlugin(plugin_info_ptr_type _pPluginInfo);
    /// @}

    /// @name PluginManager implementation
    /// @{
public:
    static PluginManager& getSingleton() { return *(dynamic_cast<PluginManager*>(&I_PluginManager::getSingleton())); }
    //boost::shared_ptr<I_ClassFactory> getClassFactory(Extension* _pExtension);
    boost::shared_ptr<I_Plugin> getPlugin(PluginInfo* const _pPluginInfo);
    
    /// Load the schema for the given extension point
    void loadSchema(ExtensionPoint* _pExtensionPoint);

protected:
    // Load a plugin
    boost::shared_ptr<I_Plugin> loadPlugin(PluginInfo* const _pPluginInfo);

    // Install all of the plugins required by the given plugin
    void installRequiredPlugins(boost::shared_ptr<PluginInfo> _pluginInfo);
    /// @}

    /// @name 'Structors
    /// @{
public:
             PluginManager();
    virtual ~PluginManager();
    /// @}

    /// @name Member variables
    /// @{
private:
    /// Base path to the plugin where the pluginname/plugin.xml are found.
    //boost::filesystem::path                 m_pluginPath;
    plugin_search_path_type                 m_pluginsSearchPath;


    /// Base path to the plugin modules where the pluginname.dll / pluginname.so are found.
#if 0   // moved to ModuleService_win32
    boost::filesystem::path                 m_modulePath;
#endif // moved

    /// List of plugin info
    plugin_info_list_type                   m_pluginInfoList;

    /// Collection of plugin info indexed by name
    plugin_info_collection_type             m_pluginInfos;

    /// Collection of loaded plugins indexed by name
	plugin_collection_type					m_plugins;

    boost::shared_ptr<I_Application>        m_pApplication;

    char                                    m_specialCharacter;
    boost::filesystem::path                 m_rootPath;
    /// @}
};  // interface PluginManager

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Plugins
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_PLUGINS_PLUGIN_MANAGER_HPP_INCLUDED
