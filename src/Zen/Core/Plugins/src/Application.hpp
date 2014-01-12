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
#ifndef ZEN_PLUGINS_APPLICATION_HPP_INCLUDED
#define ZEN_PLUGINS_APPLICATION_HPP_INCLUDED

#include <Zen/Core/Plugins/I_Application.hpp>

#include <Zen/Core/Utility/I_LogManager.hpp>
#include <Zen/Core/Utility/log_stream.hpp>

#include "XMLConfiguration.hpp"

#include <string>
#include <map>
#include <utility>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Plugins {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/// @author Tony Richards
/// @since 1.0
class Application
:   public I_Application
{
    /// @name Types
    /// @{
public:
    typedef XMLConfigurationElement::ptr_type                       config_element_ptr_type;
    typedef std::multimap<std::string, config_element_ptr_type>     config_collection_type;
    typedef std::pair<config_collection_type::key_type, 
                      config_collection_type::mapped_type>          config_pair_type;
    /// @}

    /// @name I_Application implementation
    /// @{
public:
    virtual const I_Configuration& getConfiguration() const;
    virtual const std::string& getId() const;
    virtual const std::string& getName() const;
    virtual const std::string& getVersion() const;
    virtual const std::string& getProviderName() const;
    virtual Utility::log_stream& getLogStream();
    /// @}

    /// @name Application implementation
    /// @{
public:
    /// Parse the configuration file.
    ///
    /// @note This should only be called by the PluginManager when the
    ///     application is being installed.
    bool parseConfigurationFile(const boost::filesystem::path& _configFile);
protected:
    void setId(const std::string& _id);
    void setName(const std::string& _name);
    void setVersion(const std::string& _version);
    void setProviderName(const std::string& _providerName);
    /// @}

    /// @name XML Parsing Event Handlers
    /// @{
public:
    void onApplicationElement(XMLConfigurationElement::ptr_type _pElement);
    void onPluginPathElement(XMLConfigurationElement::ptr_type _pElement);
    void onModulePathElement(XMLConfigurationElement::ptr_type _pElement);
    void onRequiresElement(XMLConfigurationElement::ptr_type _pElement);
    /// @}

    /// @{ 'Structors
public:
    Application(Utility::I_LogManager::pLogService_type _pLogService);
    virtual ~Application();
    /// @}

    /// @name Member variables
    /// @{
private:
    XMLConfiguration            m_config;
    std::string                 m_id;
    std::string                 m_name;
    std::string                 m_version;
    std::string                 m_providerName;

    Zen::Utility::log_stream_buffer     m_logStreamBuffer;
    Zen::Utility::log_stream            m_logStream;
    /// @}

};  // class Application

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Plugins
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_PLUGINS_APPLICATION_HPP_INCLUDED

