//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Core Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C)        2009 Johan Verbelen
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
/// @author Tony Richards trichards@indiezen.com
/// @author Johan Verbelen johan.verbelen@telenet.be
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#ifndef ZEN_UTILITY_XML_CONFIGURATION_ELEMENT_HPP_INCLUDED
#define ZEN_UTILITY_XML_CONFIGURATION_ELEMENT_HPP_INCLUDED

#include <Zen/Core/Utility/I_ConfigurationElement.hpp>

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include <boost/shared_ptr.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/function.hpp>

#include <map>
#include <utility>

// <string> and <list> were already included from <I_ConfigurationElement.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Utility {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class XMLConfiguration;

class XMLConfigurationElement
:   public I_ConfigurationElement
{
    /// @name Types
    /// @{
public:
    typedef boost::shared_ptr<XMLConfigurationElement>              ptr_type;

    typedef std::multimap<std::string, std::string>                 property_collection_type;
    typedef std::pair<property_collection_type::key_type, 
                      property_collection_type::mapped_type>        property_pair_type;

    typedef std::multimap<std::string, ptr_type>                    children_collection_type;
    typedef std::pair<children_collection_type::key_type, 
                      children_collection_type::mapped_type>        children_pair_type;

    /// @}

    /// @name I_ConfigurationElement implementation
    /// @{
public:
    virtual const std::string& getName() const;
    virtual const std::string& getAttribute(const std::string& _name) const;
    virtual void getChildren(const std::string& _name, I_ConfigurationElementVisitor& _visitor) const;
    virtual const I_ConfigurationElement* getChild(const std::string& _name) const;
    virtual const I_ConfigurationElement* getParent() const;
    /// @}

    /// @name XMLConfigurationElement implementation
    /// @{
protected:
    void addChild(ptr_type _pChild);
    /// @}

    /// @name 'Structors
    /// @{
public:
             XMLConfigurationElement(XMLConfiguration& _topParent, XMLConfigurationElement* _pParent, xmlNodePtr const _pNode);
    virtual ~XMLConfigurationElement();
    /// @}

    /// @name Member Variables
    /// @{
private:
    XMLConfiguration&           m_topParent;
    XMLConfigurationElement*    m_pParent;
    bool                        m_isValid;
    std::string                 m_name;
    std::string                 m_value;
    property_collection_type    m_properties;
    children_collection_type    m_children;
    /// @}

};	// class XMLConfigurationElement

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Utility
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_UTILITY_XML_CONFIGURATION_ELEMENT_HPP_INCLUDED
