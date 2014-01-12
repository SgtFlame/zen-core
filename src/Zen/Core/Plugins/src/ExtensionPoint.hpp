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

#ifndef ZEN_PLUGINS_EXTENSION_POINT_HPP_INCLUDED
#define ZEN_PLUGINS_EXTENSION_POINT_HPP_INCLUDED

#include "XMLConfiguration.hpp"

#include <Zen/Core/Plugins/I_ConfigurationElement.hpp>
#include <Zen/Core/Plugins/I_ExtensionPoint.hpp>

#include <string>
#include <map>
#include <utility>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Plugins {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class PluginInfo;
class Extension;

class ExtensionPoint
:   public I_ExtensionPoint
{
    /// @name Types
    /// @{
public:
    typedef std::map<std::string, I_Extension::extension_ptr_type>  extension_index_type;
    typedef XMLConfigurationElement::ptr_type                       schema_element_ptr_type;
    typedef std::multimap<std::string, schema_element_ptr_type>     schema_collection_type;
    typedef std::pair<schema_collection_type::key_type, 
                      schema_collection_type::mapped_type>          schema_pair_type;
    /// @}

    /// @name I_ExtensionPoint implementation
    /// @{
public:
    //virtual const I_ConfigurationElement& getConfiguration() const;

    /// Gets the extension with the given extension Id
    /// @return The specified extension or NULL if the extension doesn't exist.
    virtual I_Extension::extension_ptr_type getExtension(const std::string& _extensionId);

    /// Get all extensions configured into this extension point.
    virtual I_Extension::extension_list_ptr_type getExtensions();

    virtual void getExtensions(I_ExtensionVisitor& _visitor);

    /// Gets a displayable label for this extension point.
    virtual const std::string& getLabel() const;

    /// @}

    /// @name ExtensionPoint implementation
    /// @{
public:
    const std::string& getSchemaFileName() const;

    bool parseSchema(const boost::filesystem::path& _schemaPath);

    /// Returns just the extension point Id without the namespace
    /// @todo Does this need to be moved to I_ExtensionPoint?
    const std::string& getId() const;

    // @todo Does this need to be moved to I_ExtensionPoint?
    const std::string& getNamespace() const;

    void addExtension(boost::shared_ptr<Extension> _extension);
    /// @}

    /// @name XML Parsing Event Handlers
    /// @{
public:
    void onSchemaElement(XMLConfigurationElement::ptr_type _pElement);
    /// @}

    /// @name 'Structors
    /// @{
public:
    ExtensionPoint(const I_ConfigurationElement& _config, PluginInfo& _pluginInfo);
	virtual ~ExtensionPoint();
    /// @}

    /// @name Member variables
    /// @{
private:
    const I_ConfigurationElement&           m_config;
    XMLConfiguration                        m_schema;

    /// Reference back to the parent plugin description that hosts this extension point
    PluginInfo&                             m_pluginInfo;

    /// id of the Extension point (sans namespace)
    std::string                             m_id;

    /// Label / Display Name of the extension point
    std::string                             m_label;

    /// Schema file name for this extension point.
    std::string                             m_schemaFileName;

    /// Namespace of this extension point.
    std::string                             m_namespace;

    /// All registered extensions that implement this extension point.
    I_Extension::extension_list_ptr_type    m_extensions;

    /// Index of extensions
    extension_index_type                    m_extensionIdIndex;

    schema_collection_type                  m_schemaConfigEntries;

    /// @}

};	// class ExtensionPoint

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Plugins
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_PLUGINS_EXTENSION_POINT_HPP_INCLUDED
