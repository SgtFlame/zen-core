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

#include "ExtensionRegistry.hpp"
#include "ExtensionQuery.hpp"
#include "Extension.hpp"

#include <Zen/Core/Plugins/I_Extension.hpp>
#include <Zen/Core/Plugins/I_ExtensionPoint.hpp>
#include <Zen/Core/Plugins/I_ExtensionQuery.hpp>

#include <Zen/Core/Plugins/I_PluginManager.hpp>
#include <Zen/Core/Plugins/I_Application.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>
#include <Zen/Core/Utility/log_stream.hpp>

#include <stddef.h>
#include <string.h>

#include <stdexcept>
#include <sstream>
#include <iostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Plugins {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

ExtensionRegistry::ExtensionRegistry()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ExtensionRegistry::~ExtensionRegistry()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_ExtensionRegistry::pExtensionPoint_type
ExtensionRegistry::getExtensionPoint(const std::string& _namespace, const std::string& _extensionPointName)
{
    std::stringstream extensionPointId;
    extensionPointId << _namespace << "::" << _extensionPointName;

    // Find the extension point
    extension_point_map::iterator iter = m_extensionPoints.find(extensionPointId.str());

    // If the extension point wasn't found, return an empty (NULL) extension point.
    if (iter == m_extensionPoints.end())
    {
        return extension_point_ptr_type();
    }

    return iter->second;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_ExtensionRegistry::pExtensionQuery_type
ExtensionRegistry::createQuery()
{
    pExtensionQuery_type pQuery(new ExtensionQuery);

    return pQuery;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_ExtensionRegistry::extension_result_set_ptr_type
ExtensionRegistry::findExtensions(const pExtensionQuery_type _pQuery)
{
    // Get the logger stream
    Zen::Utility::log_stream& logStream(
        I_PluginManager::getSingleton().getApplication()->getLogStream()
    );

    // TODO Create an I_QueryResult so that it can take ownership of _pQuery

    extension_result_set_ptr_type pResult(new extension_result_set_type);

    ExtensionQuery* const pQuery = dynamic_cast<ExtensionQuery*>(_pQuery.get());

    // TODO This code was written with the assumption that the query fills in a namespace and an extension.
    // Should the createQuery() method require these two as parameters or should we not assume that these
    // fields are always present?
    extension_point_ptr_type pExtensionPoint(getExtensionPoint(pQuery->getNamespace(), pQuery->getExtensionPoint()));

    if (pExtensionPoint.get() == NULL)
    {
        std::stringstream errorMessage;
        errorMessage << "Error finding extension-point " << pQuery->getNamespace() << "::" << pQuery->getExtensionPoint() << std::endl;
        errorMessage << "Check the application xml configuration file plugin-path." << std::endl;
        errorMessage << "Make sure it includes a path to the Framework plugin that" << std::endl;
        errorMessage << "contains the definition of this extension-point.";
        std::string msg(errorMessage.str());

        logStream << msg;
        throw Zen::Utility::runtime_exception(errorMessage.str().c_str());
    }

    I_Extension::extension_list_ptr_type pExtensions(pExtensionPoint->getExtensions());

    logStream << "DEBUG: Found the extension-point " << pQuery->getNamespace() << "::" << pQuery->getExtensionPoint()
        << " with " << pExtensions->size() << " extensions." << std::endl;

    for(I_Extension::extension_list_type::iterator iter = pExtensions->begin(); iter != pExtensions->end(); ++iter)
    {
        Extension* const pExtension = dynamic_cast<Extension*>((*iter).get());

        const I_ConfigurationElement& extensionConfig = pExtension->getConfigurationElement();

        // TODO Fix this bug.  "type" isn't here.  Probably have to make ExtensionPoint smarter and have it find type
        // For now I've hacked the plugin.xml
        const std::string& extensionType = extensionConfig.getAttribute("type");

        if (strcmp(extensionType.c_str(), pQuery->getType().c_str()) == 0)
        {
            logStream << "DEBUG: Found " << extensionType << std::endl;
            pResult->push_back(*iter);
        }
        else
        {
            logStream << "DEBUG: Type " << extensionType << " didn't match " << pQuery->getType() << std::endl;
        }
    }

    return pResult;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ExtensionRegistry::getExtensionPoints(I_ExtensionPointVisitor& _visitor)
{
    _visitor.begin();

    for(extension_point_map::iterator iter = m_extensionPoints.begin(); iter != m_extensionPoints.end(); iter++)
    {
        _visitor.visit(iter->second);
    }

    _visitor.end();
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_ExtensionRegistry::class_factory_ref_type
ExtensionRegistry::getClassFactory(extension_ptr_type _extension)
{
    // TODO Should this even be here since the extension can return its own
    // class factory?

    return _extension->getClassFactory();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
boost::shared_ptr<ExtensionPoint>
ExtensionRegistry::createExtensionPoint(const I_ConfigurationElement& _config, PluginInfo& _pluginInfo)
{
    boost::shared_ptr<ExtensionPoint> pExtensionPoint(new ExtensionPoint(_config, _pluginInfo));

    // TODO Finish?

    return pExtensionPoint;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ExtensionRegistry::installExtensionPoint(boost::shared_ptr<ExtensionPoint> _extensionPoint)
{
    // Get the logger stream
    Zen::Utility::log_stream& logStream(
        I_PluginManager::getSingleton().getApplication()->getLogStream()
    );

    // Save the extension point indexed by the fully qualified name (namespace::id)
    std::stringstream fullExtensionId;
    fullExtensionId << _extensionPoint->getNamespace() << "::" << _extensionPoint->getId();
    m_extensionPoints[fullExtensionId.str()] = _extensionPoint;

    logStream << "DEBUG: Installing extension point: " << fullExtensionId.str()
        << " [" << _extensionPoint->getLabel() << "]" << std::endl;

    extension_point_container_ptr_type pExtensions;

    // Check to see if the namespace already exists.
    namespace_container_type::iterator iter = m_namespaces.find(_extensionPoint->getNamespace());

    // If the namespace doesn't exist yet, create it.
    if (iter == m_namespaces.end())
    {
        pExtensions.reset(new extension_point_container_type);
        m_namespaces[_extensionPoint->getNamespace()] = pExtensions;
    }
    else
    {
        pExtensions = iter->second;
    }

    (*pExtensions.get()).push_back(_extensionPoint);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
boost::shared_ptr<Extension>
ExtensionRegistry::createExtension(const I_ConfigurationElement& _config, PluginInfo& _pluginInfo)
{
    boost::shared_ptr<Extension> pExtension(new Extension(_config, _pluginInfo));

    // TODO Finish?
    m_extensions.push_back(pExtension);

    return pExtension;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ExtensionRegistry::installExtension(boost::shared_ptr<Extension> _extension)
{
    // Get the logger stream
    Zen::Utility::log_stream& logStream(
        I_PluginManager::getSingleton().getApplication()->getLogStream()
    );

    extension_point_map::iterator iter;

    iter = m_extensionPoints.find(_extension->getExtensionPointId());
    if (iter == m_extensionPoints.end())
    {
#ifdef _DEBUG
    // TODO Log Debug
    logStream << "DEBUG: Dumping extension points while looking for " << _extension->getExtensionPointId() << std::endl;
    for(iter = m_extensionPoints.begin(); iter != m_extensionPoints.end(); ++iter)
    {
        // TODO Log Debug
        std::stringstream message;
        message << "DEBUG: Extension point: " << iter->first << " "
            << iter->second->getNamespace() << "::" << iter->second->getId() << "["
            << iter->second->getLabel() << "]";

        logStream << message.str() << std::endl;
    }
#endif

        std::stringstream errorMessage;
        errorMessage << "ExtensionRegistry::installExtension(): Error installing extension " <<
            _extension->getPluginInfo().getName() << ".  Cannot find extension point " << _extension->getExtensionPointId();
        std::string msg(errorMessage.str());

        logStream << msg;
        throw Zen::Utility::runtime_exception(errorMessage.str().c_str());
    }

    iter->second->addExtension(_extension);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Zen
}   // namespace Plugins
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
