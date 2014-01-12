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
#ifndef ZEN_PLUGINS_I_PLUGIN_INFO_HPP_INCLUDED
#define ZEN_PLUGINS_I_PLUGIN_INFO_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Plugins/I_Configuration.hpp>
#include <Zen/Core/Plugins/I_ConfigurationElement.hpp>

#include <boost/noncopyable.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Plugins {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/// Plugin Description
/// @todo Change name to I_PluginDescription
class PLUGINS_DLL_LINK I_PluginInfo
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_PluginInfo interface
    /// @{
public:
    /// @return the name of the plugin
    virtual const std::string& getName() const = 0;

    /// @return The description of the plugin
    virtual const std::string& getDescription() const = 0;

    /// @return true if the plugin is loaded
    virtual bool isLoaded() const = 0;

    /// @return an error message if I_PluginManager::loadPlugin()
    ///         encounters an error while attempting to load the
    ///         plugin.  If no error was encountered, an empty
    ///         string is returned.
    virtual const std::string& getLoadError() const = 0;

    /// Set to true if the user wants to load this plugin
    virtual void setWantsToLoad(bool _WantsToLoad) = 0;

    /// True if the user wants to load this plugin.
    /// @see isLoaded() to check if the plugin is already
    ///     loaded or not.
    virtual bool getWantsToLoad() const = 0;

    /// Get the configuration related to this plugin info.
    /// @todo Should this be public?
    //virtual I_Configuration::const_config_ref_type getConfiguration(const std::string& _name) const = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_PluginInfo();
    virtual ~I_PluginInfo();
    /// @}

};  // interface I_PluginInfo

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Zen
}   // namespace Plugins
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_PLUGINS_I_PLUGIN_INFO_HPP_INCLUDED
