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

#ifndef ZEN_PLUGINS_EXTENSION_REGISTRY_HPP_INCLUDED
#define ZEN_PLUGINS_EXTENSION_REGISTRY_HPP_INCLUDED

#include <Zen/Core/Plugins/I_ExtensionRegistry.hpp>

#include "PluginInfo.hpp"

#include <map>
#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Plugins {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class ExtensionRegistry
:   public I_ExtensionRegistry
{
    /// @name Types
    /// @{
public:
    //typedef std::map<std::string, extension_point_ptr_type>             extension_point_container_type;
    typedef std::list<extension_point_ptr_type>                         extension_point_container_type;
    typedef boost::shared_ptr<extension_point_container_type>           extension_point_container_ptr_type;
    typedef std::map<std::string, extension_point_container_ptr_type>   namespace_container_type;
    /// @}

    /// @name I_ExtensionRegistry implementation
    /// @{
public:
    virtual extension_point_ptr_type getExtensionPoint(const std::string& _namespace, const std::string& _extensionPointName);
    virtual I_ExtensionRegistry::pExtensionQuery_type createQuery();
    virtual extension_result_set_ptr_type findExtensions(const pExtensionQuery_type _pQuery);
    virtual void getExtensionPoints(I_ExtensionPointVisitor& _visitor);
    virtual class_factory_ref_type getClassFactory(extension_ptr_type _extension);
    /// @}

    /// @name ExtensionRegistry implementation
    /// @{
public:
    /// Get the singleton
    static ExtensionRegistry& getSingleton() { return *(dynamic_cast<ExtensionRegistry*>(&I_ExtensionRegistry::getSingleton())); }

    /// Create an extension point
    boost::shared_ptr<ExtensionPoint> createExtensionPoint(const I_ConfigurationElement& _config, PluginInfo& _plugin);

    /// Installs an extension point that is fully parsed and ready for use
    void installExtensionPoint(boost::shared_ptr<ExtensionPoint> _extensionPoint);

    /// Create an extension
    boost::shared_ptr<Extension> createExtension(const I_ConfigurationElement& _config, PluginInfo& _plugin);

    /// Installs an extension that is fully parsed and ready for use
    void installExtension(boost::shared_ptr<Extension> _extension);

    /// @}

    /// @name 'Structors
    /// @{
public:
			 ExtensionRegistry();
	virtual ~ExtensionRegistry();
    /// @}

    /// @name Member variables
    /// @{
private:
    /// Extension point namespace map with a list of extension points as the value.
    namespace_container_type    m_namespaces;

    typedef std::map<std::string, boost::shared_ptr<ExtensionPoint> >   extension_point_map;

    /// Extension point id (fully qualified including namespace) mapped to extension point objects
    extension_point_map   m_extensionPoints;

    /// List of extensions
    /// @todo Should this be a map of some sort?
    typedef std::list<pExtension_type>  Extensions_type;    
    Extensions_type         m_extensions;
    /// @}

};	// class ExtensionRegistry

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Plugins
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_PLUGINS_EXTENSION_REGISTRY_HPP_INCLUDED
