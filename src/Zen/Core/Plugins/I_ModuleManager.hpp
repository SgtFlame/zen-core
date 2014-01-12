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
#ifndef ZEN_PLUGINS_I_MODULE_MANAGER_HPP_INCLUDED
#define ZEN_PLUGINS_I_MODULE_MANAGER_HPP_INCLUDED

#include "Configuration.hpp"

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/filesystem/path.hpp>

#include <list>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Plugins {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_ModuleService;

class PLUGINS_DLL_LINK I_ModuleManager
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef I_ModuleService*        service_ptr_type;

    /// Eventually other service types will be supported such as
    /// static, downloadable, remote, etc.
    enum MODULE_SERVICE_TYPE
    {
        DYNAMIC
    };
    /// @}

    /// @name I_ModuleManager interface
    /// @{
public:
    /// Load a module service
    virtual service_ptr_type getService(MODULE_SERVICE_TYPE _serviceType) = 0;

    /// Add a module search path
    virtual void addPath(const boost::filesystem::path& _modulePath) = 0;

    // Drop a module search path
    virtual void dropPath(const boost::filesystem::path& _modulePath) = 0;

    // Find module path if one exists
    virtual bool findPath(const std::string _moduleName, boost::filesystem::path &_modulePath) = 0;
    /// @}

    /// @name Static Instance
    /// @{
public:
    static I_ModuleManager& getSingleton();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ModuleManager();
    virtual ~I_ModuleManager();
    /// @}

};  // interface I_ModuleManager

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}	// namespace Plugins
}	// namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_PLUGINS_I_MODULE_MANAGER_HPP_INCLUDED

