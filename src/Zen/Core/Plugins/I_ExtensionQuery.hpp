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

#ifndef ZEN_PLUGINS_I_EXTENSION_QUERY_HPP_INCLUDED
#define ZEN_PLUGINS_I_EXTENSION_QUERY_HPP_INCLUDED

#include "Configuration.hpp"

#include <boost/noncopyable.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Plugins {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

/// @deprecated Instead of using this, use I_ExtensionRegistry::getExtensionPoints()
/// @see I_ExtensionRegistry::getExtensionPoints().
class I_ExtensionQuery
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_ExtensionQuery interface
    /// @{
public:
    /// Set the namespace of the extension
    virtual void setNamespace(const std::string& _namespace) = 0;

    /// Set the extension point of the extension
    virtual void setExtensionPoint(const std::string& _extensionPoint) = 0;

    /// Set the type of the extension.
    /// This is a very general type and should be considered a
    /// quick and dirty hack.  This probably will not be supported
    /// in the future.
    virtual void setType(const std::string& _type) = 0;

    /// Get the string representation of this query
    virtual const std::string& toString() const = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
                 I_ExtensionQuery();
	virtual ~I_ExtensionQuery();
    /// @}

};	// interface I_ExtensionQuery

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Plugins
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_PLUGINS_I_EXTENSION_QUERY_HPP_INCLUDED
