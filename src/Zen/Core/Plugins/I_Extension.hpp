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

#ifndef ZEN_PLUGINS_I_EXTENSION_HPP_INCLUDED
#define ZEN_PLUGINS_I_EXTENSION_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Plugins/I_ConfigurationElement.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <list>
#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Plugins {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_ClassFactory;
class I_ConfigurationElement;

class I_Extension
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef boost::shared_ptr<I_Extension>          extension_ptr_type;
    typedef std::list<extension_ptr_type>           extension_list_type;
    typedef boost::shared_ptr<extension_list_type>  extension_list_ptr_type;
    /// @}

    /// @name I_Extension interface
    /// @{
public:
    /// Return the class factory for this extension.
    /// @return Never NULL (obviously) but if the class factory isn't found
    ///     then an exception is thrown.
    /// @todo TR I think I_ClassFactory should be renamed to I_ExtensionFactory
    virtual I_ClassFactory& getClassFactory() = 0;

    /// Get the configuration
    virtual const I_ConfigurationElement& getConfigurationElement() const = 0;

    virtual const std::string& getType() const = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
			 I_Extension();
	virtual ~I_Extension();
    /// @}

};	// interface I_Extension

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Plugins
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_PLUGINS_I_EXTENSION_HPP_INCLUDED
