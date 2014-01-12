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

#ifndef ZEN_PLUGINS_I_EXTENSION_REGISTRY_HPP_INCLUDED
#define ZEN_PLUGINS_I_EXTENSION_REGISTRY_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Plugins/I_ConfigurationElement.hpp>

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <list>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Plugins {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_ClassFactory;
class I_Extension;
class I_ExtensionPoint;
class I_ExtensionQuery;

class PLUGINS_DLL_LINK I_ExtensionRegistry
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    /// @deprecated Use pExtensionPoint_type
    /// @see http://www.indiezen.org/wiki/ticket/111
    typedef boost::shared_ptr<I_ExtensionPoint>             extension_point_ptr_type;
    /// @deprecated
    /// @see http://www.indiezen.org/wiki/ticket/111
    typedef boost::shared_ptr<I_Extension>                  extension_ptr_type;
    /// @deprecated
    /// @see http://www.indiezen.org/wiki/ticket/111
    typedef std::list<extension_ptr_type>                   extension_result_set_type;
    /// @deprecated
    /// @see http://www.indiezen.org/wiki/ticket/111
    typedef boost::shared_ptr<extension_result_set_type>    extension_result_set_ptr_type;
    typedef I_ClassFactory&                                 class_factory_ref_type;

    // New types (old ones are deprecated as they use an older coding style)
    typedef boost::shared_ptr<I_Extension>                  pExtension_type;
    typedef boost::shared_ptr<I_ExtensionPoint>             pExtensionPoint_type;
    typedef boost::shared_ptr<I_ExtensionQuery>             pExtensionQuery_type;

    /// Visitor inteface for traversing extension  points.  
    /// Defined below as an inner class.
    class I_ExtensionPointVisitor;
    /// @}

    /// @name I_ExtensionRegistry interface
    /// @{
public:
    /// Get an extension point by namespace and name.
    virtual pExtensionPoint_type getExtensionPoint(const std::string& _namespace, const std::string& _extensionPointName) = 0;

    /// Construct an empty query
    /// @deprecated Use the visitor version of getExtensionPoints() and 
    ///     I_ExtensionPoint::getExtensions()
    /// @see http://www.indiezen.org/wiki/ticket/111
    virtual pExtensionQuery_type createQuery() = 0;

    /// Find some extensions based on a query
    /// Note that the ownership of _query is passed on to the result set.
    /// @deprecated Use the visitor version of getExtensionPoints() and 
    ///     I_ExtensionPoint::getExtensions()
    /// @see http://www.indiezen.org/wiki/ticket/111
    virtual extension_result_set_ptr_type findExtensions(const pExtensionQuery_type _query) = 0;

    /// Get all of the extension points using a visitor
    virtual void getExtensionPoints(I_ExtensionPointVisitor& _visitor) = 0;

    /// Get the class factory for an extension.
    /// @throw Utility::runtime_exception if the class factory is not found.
    virtual class_factory_ref_type getClassFactory(extension_ptr_type _extension) = 0;
    /// @}

    /// @name I_ExtensionRegistry methods that aren't implemented yet.
    /// @{
public:
    //virtual I_ConfigurationElement::collection_type& getConfigurationElementsFor(const std::string& _extensionPointId) = 0;

    //virtual I_ConfigurationElement::collection_type& getConfigurationElementsFor(const std::string& _namespace, const std::string& _extensionPointName, const std::string& _extensionId = Utility::EmptyString) = 0;

    //virtual I_Extension* getExtension(const std::string& _extensionId) = 0;
    //virtual I_Extension* getExtension(const std::string& _extensionPointId, const std::string& _extensionId) = 0;
    //virtual I_Extension* getExtension(const std::string& _namespace, const std::string& _extensionPointName, const std::string& _extensionId) = 0;

    //virtual I_Extension::collection_ref_type getExtensions(const std::string& _namespace) = 0;

    //virtual I_ExtensionPoint* getExtensionPoint(const std::string& _extensionPointId) = 0;

    //virtual I_ExtensionPoint::const_collection_ref_type getExtensionPoints() = 0;
    //virtual I_ExtensionPoint::const_collection_ref_type getExtensionPoints(const std::string& _namespace) = 0;

    //virtual const std::list<std::string> getNamespaces() = 0;

    //virtual bool removeExtension(I_Extension* _extension) = 0;
    //virtual bool removeExtensionPoint(I_ExtensionPoint _extensionPoint) = 0;

    //virtual void stop() = 0;

    /// @}

    /// @name Singleton instance
    /// @{
public:
    static I_ExtensionRegistry& getSingleton();
    /// @}

    /// @name 'Structors
    /// @{
protected:
                 I_ExtensionRegistry();
	virtual ~I_ExtensionRegistry();
    /// @}

    /// @name Inner Classes
    /// @{
public:
    /// @brief Extension Visitor
    ///
    /// Implement this interface to use the visitor version of getExtensionPoints()
    class I_ExtensionPointVisitor
    {
    public:
        virtual void begin() = 0;
        virtual void visit(pExtensionPoint_type) = 0;
        virtual void end() = 0;
    };  // interface I_ExtensionVisitor
    /// @}
};	// interface I_ExtensionRegistry

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Plugins
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_PLUGINS_I_EXTENSION_REGISTRY_HPP_INCLUDED
