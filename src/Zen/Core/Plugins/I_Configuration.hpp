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
#ifndef ZEN_PLUGINS_I_CONFIGURATION_HPP_INCLUDED
#define ZEN_PLUGINS_I_CONFIGURATION_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Plugins/I_ConfigurationElement.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

#include <string>
#include <list>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Plugins {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class PLUGINS_DLL_LINK I_Configuration
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef const I_Configuration&                              const_config_ref_type;
    typedef std::list<I_ConfigurationElement::const_ptr_type>   config_list_type;
    typedef boost::shared_ptr<config_list_type>                 config_list_ptr_type;
    /// @}

    /// @name I_Configurable interface
    /// @{
public:
    /// Get all of the configuration elements.
    //virtual config_list_ptr_type getConfigurationElements() const = 0;

    /// Get a configuration element by name.
    virtual I_ConfigurationElement::const_ptr_type getConfigurationElement(const std::string& _name) const = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_Configuration();
    virtual ~I_Configuration();
    /// @}

};	// interface I_Configuration

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Plugins
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_PLUGINS_I_CONFIGURATION_HPP_INCLUDED
