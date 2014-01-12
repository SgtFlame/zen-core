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

#include "ExtensionPoint.hpp"
#include "Extension.hpp"

#include <Zen/Core/Plugins/I_PluginManager.hpp>
#include <Zen/Core/Plugins/I_Application.hpp>

#include <Zen/Core/Utility/log_stream.hpp>

#include <boost/bind.hpp>
#include <boost/pointer_cast.hpp>

#include <iostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Plugins {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ExtensionPoint::ExtensionPoint(const I_ConfigurationElement& _config, PluginInfo& _pluginInfo)
:   m_config(_config)
,   m_pluginInfo(_pluginInfo)
,   m_extensions(new I_Extension::extension_list_type)
{
    // Get the logger stream
    Zen::Utility::log_stream& logStream(
        I_PluginManager::getSingleton().getApplication()->getLogStream()
    );

    m_id = _config.getAttribute("id");
    m_label = _config.getAttribute("name");
    m_schemaFileName = _config.getAttribute("schema");

    m_schema.listenElement("schema",    boost::bind(&ExtensionPoint::onSchemaElement, this, _1));

    logStream << "DEBUG: Creating extension-point Id: " << m_id << " Label: " << m_label << " Schema: " << m_schemaFileName << std::endl;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ExtensionPoint::~ExtensionPoint()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
ExtensionPoint::parseSchema(const boost::filesystem::path& _schemaPath)
{
    // TODO Make sure the required elements are present, etc and return false
    //      if any errors occur.
    return m_schema.parse(_schemaPath);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_Extension::extension_ptr_type
ExtensionPoint::getExtension(const std::string& _extensionId)
{
    I_Extension::extension_ptr_type pExtension;

    extension_index_type::iterator iter = m_extensionIdIndex.find(_extensionId);

    if (iter == m_extensionIdIndex.end())
    {
        return pExtension;
    }

    return iter->second;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_Extension::extension_list_ptr_type
ExtensionPoint::getExtensions()
{
    return m_extensions;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ExtensionPoint::getExtensions(I_ExtensionVisitor& _visitor)
{
    _visitor.begin();

    for(I_Extension::extension_list_type::iterator iter = m_extensions->begin(); iter != m_extensions->end(); iter++)
    {
        _visitor.visit(*iter);
    }

    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
ExtensionPoint::getLabel() const
{
    return m_label;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
ExtensionPoint::getId() const
{
    return m_id;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
ExtensionPoint::getSchemaFileName() const
{
    return m_schemaFileName;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
ExtensionPoint::getNamespace() const
{
    return m_namespace;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ExtensionPoint::addExtension(boost::shared_ptr<Extension> _extension)
{
    // Get the logger stream
    Zen::Utility::log_stream& logStream(
        I_PluginManager::getSingleton().getApplication()->getLogStream()
    );

    logStream << "Adding extension " << _extension->getType() << 
        " to " << m_namespace << "::" << m_id << std::endl;

    boost::shared_ptr<I_Extension> pExtension(boost::dynamic_pointer_cast<I_Extension>(_extension));
    m_extensions->push_back(pExtension);
    m_extensionIdIndex[_extension->getType()] = pExtension;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ExtensionPoint::onSchemaElement(XMLConfigurationElement::ptr_type _pElement)
{
    // Get the logger stream
    Zen::Utility::log_stream& logStream(
        I_PluginManager::getSingleton().getApplication()->getLogStream()
    );

    m_namespace = _pElement->getAttribute("targetNamespace");

    logStream << "DEBUG: targetNamepace: " << m_namespace << std::endl;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Zen
}   // namespace Plugins
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
