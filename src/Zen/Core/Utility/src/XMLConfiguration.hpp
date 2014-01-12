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
#ifndef ZEN_UTILITY_XML_CONFIGURATION_HPP_INCLUDED
#define ZEN_UTILITY_XML_CONFIGURATION_HPP_INCLUDED

#include <Zen/Core/Utility/I_Configuration.hpp>

#include "XMLConfigurationElement.hpp"

#include <boost/function.hpp>

#include <string>
#include <map>
#include <list>
#include <utility>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Utility {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class XMLConfiguration
:   public I_Configuration
{
    /// @name Types
    /// @{
public:
    typedef XMLConfigurationElement::ptr_type                       config_element_ptr_type;
    typedef std::multimap<std::string, config_element_ptr_type>     config_collection_type;
    typedef std::pair<config_collection_type::key_type, 
                      config_collection_type::mapped_type>          config_pair_type;

    typedef boost::function<void (config_element_ptr_type)>         element_event_type;
    typedef std::map<std::string, element_event_type>               element_event_map_type;
    /// @}

    /// @name I_Configuration implementation
    /// @{
public:
    virtual const I_ConfigurationElement* getConfigurationElement(const std::string& _name) const;
    /// @}

    /// @name XMLConfiguration implementation
    /// @{
public:
    void listenElement(const std::string& _name, element_event_type _function);

    bool parse(const boost::filesystem::path& _configFile);

    void dispatchEvent(config_element_ptr_type _pElement);
    /// @}

    /// @name 'Structors
    /// @{
public:
             XMLConfiguration();
    virtual ~XMLConfiguration();
    /// @}

    /// @name Member variables
    /// @{
private:
    config_collection_type  m_config;
    element_event_map_type  m_elementEvents;
    /// @}

};	// class XMLConfiguration

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Utility
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_UTILITY_XML_CONFIGURATION_HPP_INCLUDED
