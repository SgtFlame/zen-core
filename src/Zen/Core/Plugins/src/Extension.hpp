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
#ifndef ZEN_PLUGINS_EXTENSION_HPP_INCLUDED
#define ZEN_PLUGINS_EXTENSION_HPP_INCLUDED

#include "../Configuration.hpp"

#include <Zen/Core/Plugins/I_Extension.hpp>
#include <Zen/Core/Plugins/I_ConfigurationElement.hpp>

#include <Zen/Core/Threading/I_Mutex.hpp>

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <list>
#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Plugins {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class PluginInfo;

class Extension
:   public I_Extension
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_Extension implementation
    /// @{
public:
    virtual I_ClassFactory& getClassFactory();
    virtual const I_ConfigurationElement& getConfigurationElement() const;
    virtual const std::string& getType() const;
    /// @}

    /// @name Extension implementation
    /// @{
public:
    PluginInfo& getPluginInfo();
    const PluginInfo& getPluginInfo() const;

    /// Get the fully qualified (including namespace) extension point Id
    const std::string& getExtensionPointId() const;
    /// @}

    /// @name 'Structors
    /// @{
public:
			 Extension(const I_ConfigurationElement& _config, PluginInfo& _pluginInfo);
	virtual ~Extension();
    /// @}

    /// @name Member variables
    /// @{
private:
    const I_ConfigurationElement&       m_configuration;
    PluginInfo&                         m_pluginInfo;
    std::string                         m_extensionPointId;
    std::string                         m_type;

    /// @todo Think through the lifecycle of this class factory.
    ///         What happens when the plugin goes away?  Shouldn't the extensions
    ///         go away as well?  Need to think about this.
    I_ClassFactory* volatile            m_pClassFactory;
    Threading::I_Mutex*                 m_pGuard;

};	// class  Extension

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Plugins
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_PLUGINS_EXTENSION_HPP_INCLUDED
