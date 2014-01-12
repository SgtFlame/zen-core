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
#ifndef ZEN_PLUGINS_I_APPLICATION_HPP_INCLUDED
#define ZEN_PLUGINS_I_APPLICATION_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Utility/log_stream.hpp>

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <list>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Plugins {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_Configuration;

/// @author Tony Richards
/// @since 1.0
class PLUGINS_DLL_LINK I_Application
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef boost::shared_ptr<I_Application>                ptr_type;
    //typedef I_ConfigurationElement::config_list_type        config_list_type;
    //typedef I_ConfigurationElement::config_list_ptr_type    config_list_ptr_type;
    /// @}

    /// @name I_Application interface
    /// @{
public:
    /// Get the configuration elements that match the specified name.
    virtual const I_Configuration& getConfiguration() const = 0;

    virtual const std::string& getId() const = 0;
    virtual const std::string& getName() const = 0;
    virtual const std::string& getVersion() const = 0;
    virtual const std::string& getProviderName() const = 0;
    virtual Utility::log_stream& getLogStream() = 0;
    /// @}

    /// @{ 'Structors
protected:
             I_Application();
    virtual ~I_Application();
    /// @}

};  // interface I_Application

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Plugins
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_PLUGINS_I_APPLICATION_HPP_INCLUDED

