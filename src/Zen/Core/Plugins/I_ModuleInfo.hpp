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
#ifndef ZEN_PLUGINS_I_MODULE_INFO_HPP_INCLUDED
#define ZEN_PLUGINS_I_MODULE_INFO_HPP_INCLUDED

#include "Configuration.hpp"

#include <boost/noncopyable.hpp>

#include <string>

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Plugins {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class PLUGINS_DLL_LINK I_ModuleInfo
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
#if defined _WIN32
    typedef HMODULE     module_handle_type;
#else
    typedef void*       module_handle_type;
#endif // _WIN32
    /// @}

    /// @name I_ModuleInfo interface
    /// @{
public:
    virtual void setName(const std::string _name) = 0;
    virtual std::string getName() const = 0;

    virtual void setHandle(const module_handle_type _handle) = 0;
    virtual module_handle_type getHandle() const = 0;

    virtual unsigned incrementReferences() = 0;
    virtual unsigned decrementReferences() = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ModuleInfo();
    virtual ~I_ModuleInfo();
    /// @}

};  // interface I_ModuleInfo

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Plugins
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_PLUGINS_I_MODULE_INFO_HPP_INCLUDED
