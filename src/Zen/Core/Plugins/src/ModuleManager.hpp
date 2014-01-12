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
#ifndef ZEN_PLUGINS_MODULE_MANAGER_HPP_INCLUDED
#define ZEN_PLUGINS_MODULE_MANAGER_HPP_INCLUDED

#include <Zen/Core/Plugins/I_ModuleManager.hpp>

#include <boost/shared_ptr.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Plugins {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class ModuleManager
:   public I_ModuleManager
{
    /// @name Types
    /// @{
public:
    typedef boost::shared_ptr<I_ModuleService>                  module_service_ptr_type;
    typedef std::list</*const*/ boost::filesystem::path>            module_paths_type;
    typedef std::list</*const*/ boost::filesystem::path>::iterator  module_paths_iterator_type;
    /// @}

    /// @name I_ModuleManager implementation
    /// @{
public:
    virtual service_ptr_type getService(MODULE_SERVICE_TYPE _serviceType);
    virtual void addPath(const boost::filesystem::path& _modulePath);
    virtual void dropPath(const boost::filesystem::path& _modulePath);
    virtual bool findPath(const std::string _moduleName, boost::filesystem::path &_modulePath);
    /// @}

    /// @name ModuleManager implementation
    /// @{
public:
    static ModuleManager& getSingleton() { return *(dynamic_cast<ModuleManager*>(&I_ModuleManager::getSingleton())); }
    /// @}

    /// @name 'Structors
    /// @{
public:
             ModuleManager();
    virtual ~ModuleManager();
    /// @}

    /// @name Member variables
    /// @{
private:

    /// Module service
    module_service_ptr_type     m_pModuleService;

    /// Module search paths
    module_paths_type           m_modulePaths;
    /// @}

};  // interface ModuleManager

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}	// namespace Plugins
}	// namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_PLUGINS_MODULE_MANAGER_HPP_INCLUDED
