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
#ifndef ZEN_PLUGINS_I_PLUGIN_HPP_INCLUDED
#define ZEN_PLUGINS_I_PLUGIN_HPP_INCLUDED

#include "Configuration.hpp"

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <list>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace ResourceManager {
        class I_Importer;
        class I_Exporter;
    } // namespace ResourceManager
namespace Plugins {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_ClassFactory;
class I_ConfigurationElement;

/// @author Tony Richards
/// @since 1.0
class PLUGINS_DLL_LINK I_Plugin
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_Plugin interface
    /// @{ 
public:
    /// Initialize the extension points implemented by this plugin
    virtual void initExtensionPoints() = 0;

    /// Initialize the extensions implemented by this plugin
    virtual void initExtensions() = 0;

    /// Get the class factory for the given extension.
    /// @param _pConfig - the <extension>...</extension> configuration for the extension for which
    ///     the framework needs a class factory.  It's up to the plugin to return the appropriate class
    ///     factory, or return NULL if none is available.
    /// @return The class factory that implements the specified extension, or NULL if 
    ///     the class factory doesn't exist.
    virtual I_ClassFactory* getClassFactory(const I_ConfigurationElement& _config) = 0;
    /// @}

    /// @{ 'Structors
protected:
             I_Plugin();
    virtual ~I_Plugin();
    /// @}

};  // interface I_Plugin
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Plugins
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_PLUGINS_I_PLUGIN_HPP_INCLUDED

