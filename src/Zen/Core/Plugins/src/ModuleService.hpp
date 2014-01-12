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
#ifndef ZEN_PLUGINS_MODULE_SERVICE_HPP_INCLUDED
#define ZEN_PLUGINS_MODULE_SERVICE_HPP_INCLUDED

#include <Zen/Core/Plugins/I_ModuleInfo.hpp>
#include <Zen/Core/Plugins/I_ModuleService.hpp>
#include <Zen/Core/Threading/I_Mutex.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/filesystem/path.hpp>

#include <map>
#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Plugins {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class ModuleService
:   public I_ModuleService
{
    /// @name Types
    /// @{
public:
    typedef std::map<std::string, module_ptr_type>                      module_name_index_type;
    typedef std::map<std::string, module_ptr_type>::iterator            module_name_index_iterator_type;
    typedef boost::shared_ptr<I_ModuleInfo>                             module_info_ptr_type;
    typedef std::map<module_ptr_type, module_info_ptr_type>             modules_type;
    typedef std::map<module_ptr_type, module_info_ptr_type>::iterator   modules_iterator_type;
    typedef Threading::I_Mutex*                                         mutex_ptr_type;
    /// @}

    /// @name I_ModuleService implementation
    /// @{
public:
    virtual module_ptr_type load(const std::string& _moduleName);
    virtual void unload(module_ptr_type _pModule);
    /// @}

    /// @name 'Structors
    /// @{
public:
             ModuleService();
    virtual ~ModuleService();
    /// @}

    /// @name Member variables
    /// @{
private:
    module_name_index_type      m_moduleIndex;
    modules_type                m_modules;
    mutex_ptr_type              m_pModuleGuard;
    /// @}

};  // interface ModuleService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}	// namespace Plugins
}	// namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_PLUGINS_MODULE_SERVICE_HPP_INCLUDED
