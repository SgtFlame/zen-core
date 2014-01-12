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
#ifndef ZEN_PLUGINS_UTILITIES_HPP_INCLUDED
#define ZEN_PLUGINS_UTILITIES_HPP_INCLUDED

#include <Zen/Core/Plugins/I_Module.hpp>
#include <Zen/Core/Plugins/I_Plugin.hpp>
#include <Zen/Core/Plugins/I_ConfigurationElement.hpp>

// Microsoft C++ compiler
#if defined(_MSC_VER) && !defined(__INTEL_COMPILER)
#   define  DLL_EXPORT __declspec(dllexport)
#endif  // _MSC_VER

// GNU C++ compiler
#if defined(__GNUG__) && !defined(__INTEL_COMPILER)
#   define DLL_EXPORT // __declspec(dllexport)
#endif  // __GNUG__


// Sun C++ compiler
#if defined(__SUNPRO_CC)
#   define DLL_EXPORT   // ?
#endif  // __SUNPRO_CC


// Intel C++ compiler
#if defined(__INTEL_COMPILER)
#   define DLL_EXPORT __declspec(dllexport)
#endif  // __INTEL_COMPILER


#ifndef DLL_EXPORT
#    define DLL_EXPORT
#endif  // ! ZENPLUGINS_EXPORTS

// Old style
#if 0
/// Declare a module
#define DECLARE_ZEN_MODULE()  \
#include <Zen/Core/Plugins/I_Module.hpp> \
extern "C" \
{ \
DLL_EXPORT Zen::Plugins::I_Module& getModule(); \
}
#endif

/// Declare a module
#define DECLARE_ZEN_MODULE(module_type, plugin_type) \
class module_type \
:   public Zen::Plugins::BaseModule<plugin_type> \
{ \
};

/// Implement a module
#define IMPLEMENT_ZEN_MODULE(module_type) \
extern "C" \
DLL_EXPORT \
Zen::Plugins::I_Module& \
getModule() \
{ \
    return module_type::instance(); \
} \
template <> Zen::Plugins::BaseModule<module_type::plugin_type>* Zen::Plugins::BaseModule<module_type::plugin_type>::sm_pModule = NULL;


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/// Declare a plugin
#define DECLARE_ZEN_PLUGIN(Plugin_type) \
    Plugin_type(); \
    virtual ~Plugin_type(); \
    virtual void initExtensionPoints(); \
    virtual void initExtensions(); \
    virtual Zen::Plugins::I_ClassFactory* getClassFactory(const Zen::Plugins::I_ConfigurationElement& _config);

/// Implement a plugin
#define IMPLEMENT_SIMPLE_ZEN_PLUGIN(Plugin_type) \
Plugin_type::Plugin_type() {} \
Plugin_type::~Plugin_type() {} \
void Plugin_type::initExtensionPoints() {} \
void Plugin_type::initExtensions() {} 

/// Begin a zen extension map
#define BEGIN_ZEN_EXTENSION_MAP(Plugin_type) \
Zen::Plugins::I_ClassFactory* \
Plugin_type::getClassFactory(const Zen::Plugins::I_ConfigurationElement& _config) \
{ \
    const std::string& extensionPoint = _config.getAttribute("point"); \
    if (false) {}

#define ZEN_EXTENSION(extension_point_type, factory_type) \
    else if(extensionPoint == extension_point_type) \
    { \
        return factory_type; \
    }

#define END_ZEN_EXTENSION_MAP() \
    return NULL; \
} 

#define IMPLEMENT_SINGLE_ZEN_PLUGIN(module_type, plugin_type) \
class plugin_type \
:   public Zen::Plugins::I_Plugin \
{ \
public: \
    DECLARE_ZEN_PLUGIN(plugin_type); \
}; \
IMPLEMENT_SIMPLE_ZEN_PLUGIN(plugin_type) \
DECLARE_ZEN_MODULE(module_type, plugin_type) \
IMPLEMENT_ZEN_MODULE(module_type)

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Plugins {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

/// This template implement a common implementation of I_Module.
/// You can use this or write your own implementation.
template<typename Plugin_type>
class BaseModule
:   public I_Module
{
    /// @name Types
    /// @{
public:
    typedef Plugin_type             plugin_type;
    /// @}

    /// @name I_Module implementation
    /// @{
public:
    virtual plugin_name_collection_type& getPluginNames();
    virtual I_Module::plugin_ptr_type getPlugin(const plugin_name_type& _pluginName);
    virtual void destroyPlugin(I_Module::plugin_ptr_type _plugin);
    /// @}

    /// @name 'Structors
    /// @{
public:
             BaseModule();
    virtual ~BaseModule();
    /// @}

    /// @name Implementation
    /// @}
public:
    static BaseModule& instance();
    /// @}

    /// @name Member Variables
    /// @{
private:
    plugin_name_collection_type                 m_names;
    plugin_ptr_type                             m_pPlugin;

    static BaseModule<Plugin_type>*             sm_pModule;
    /// @}
};  // template class BaseModule

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename Plugin_type>
BaseModule<Plugin_type>::BaseModule()
{
    // TODO: Register the plugins
    m_pPlugin = plugin_ptr_type(new Plugin_type);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename Plugin_type>
BaseModule<Plugin_type>::~BaseModule()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

template<typename Plugin_type>
BaseModule<Plugin_type>&
BaseModule<Plugin_type>::instance()
{
    // TODO: Guard
    if (sm_pModule == NULL)
    {
        sm_pModule = new BaseModule<Plugin_type>();
    }

    return *sm_pModule;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename Plugin_type>
I_Module::plugin_name_collection_type& 
BaseModule<Plugin_type>::getPluginNames()
{
    return m_names;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename Plugin_type>
I_Module::plugin_ptr_type
BaseModule<Plugin_type>::getPlugin(const plugin_name_type& _PluginName)
{
    // TODO: Return the appropriate plugin; 
    // For now since only one type is supported then return that.
    return m_pPlugin;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename Plugin_type>
void 
BaseModule<Plugin_type>::destroyPlugin(I_Module::plugin_ptr_type _pPlugin)
{
    // TODO delete?
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Plugins
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif  // ZEN_PLUGINS_UTILITIES_HPP_INCLUDED
