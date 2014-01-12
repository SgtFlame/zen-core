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

#ifndef ZEN_PLUGINS_I_EXTENSION_POINT_HPP_INCLUDED
#define ZEN_PLUGINS_I_EXTENSION_POINT_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Plugins/I_ConfigurationElement.hpp>
#include <Zen/Core/Plugins/I_Extension.hpp>

#include <boost/noncopyable.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Plugins {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_ExtensionPoint
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef boost::shared_ptr<I_Extension>      pExtension_type;
    struct I_ExtensionVisitor;
    /// @}

    /// @name I_ExtensionPoint interface
    /// @{
public:
    //virtual const I_ConfigurationElement::config_list_ptr_type getConfigurationElements() const = 0;

    /// Gets the extension with the given extension Id
    /// @return The specified extension or an invalid pointer if the extension doesn't exist.
    virtual I_Extension::extension_ptr_type getExtension(const std::string& _extensionId) = 0;

    /// Get all extensions that implement this extension point.
    /// @deprecated Use the visitor version of getExtensions()
    /// @see http://www.indiezen.org/wiki/ticket/111
    virtual I_Extension::extension_list_ptr_type getExtensions() = 0;

    /// Get all extensions that implement this extension point using a visitor
    virtual void getExtensions(I_ExtensionVisitor& _visitor) = 0;

    /// Gets a displayable label for this extension point.
    virtual const std::string& getLabel() const = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
			 I_ExtensionPoint();
	virtual ~I_ExtensionPoint();
    /// @}

    /// @name Inner Classes
    /// @{
public:
    /// @brief Extension Visitor
    ///
    /// Implement this interface to use the visitor version of getExtensions()
    struct I_ExtensionVisitor
    {
        virtual void begin() = 0;
        virtual void visit(pExtension_type _pExtension) = 0;
        virtual void end() = 0;
    };  // interface I_ExtensionVisitor
    /// @}

};	// interface I_ExtensionPoint

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Plugins
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_PLUGINS_I_EXTENSION_POINT_HPP_INCLUDED
