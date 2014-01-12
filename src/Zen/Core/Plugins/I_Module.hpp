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
#ifndef ZEN_PLUGINS_I_MODULE_HPP_INCLUDED
#define ZEN_PLUGINS_I_MODULE_HPP_INCLUDED

#include "Configuration.hpp"

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <string>
#include <list>

#ifdef HOST_POSIX
#include <dlfcn.h>
#endif //HOST_POSIX

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Plugins {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

// Forward declarations
class I_Plugin;

/// Base class for a Zen Module
/// A plugin DLL should implement one of these
class PLUGINS_DLL_LINK I_Module
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef std::string                 plugin_name_type;
    typedef std::list<plugin_name_type> plugin_name_collection_type;
    typedef boost::shared_ptr<I_Plugin> plugin_ptr_type;

#ifdef HOST_WIN32
    typedef I_Module& (*proc_ptr_type)();
#elif HOST_POSIX | __APPLE__
    typedef I_Module& (*QUERY_MODULE_FUNCTION_PTR)();
#else
#error Unsupported platform in I_Module.hpp
#endif
    /// @}

    /// @name I_Module interface
    /// @{
public:
    /// Returns the names of the plugins implemented by this module.
    virtual plugin_name_collection_type& getPluginNames() = 0;

    /// Returns a plugin by name.
    /// For now, plugins are all considered singletons.   The plugin should
    /// normally not be created until getPlugin is called, in case the plugin
    /// isn't required.
    /// @return - Returns the plugin if the name is a valid plugin name for this
    ///         module, otherwise NULL is returned.
    virtual plugin_ptr_type getPlugin(const plugin_name_type& _PluginName) = 0;

    /// Destroys the plugin.
    /// The framework will call this when it is finished with the plugin.
    virtual void destroyPlugin(plugin_ptr_type _Plugin) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_Module();
    virtual ~I_Module();
    /// @}

};  // interface I_Module

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Zen
}   // namespace Plugins
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_PLUGINS_I_MODULE_HPP_INCLUDED
