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
//  Tony Richards                           - trichards@indiezen.com
//
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#ifndef ZEN_UTILITY_I_ENVIRONMENT_HANDLER_HPP_INCLUDED
#define ZEN_UTILITY_I_ENVIRONMENT_HANDLER_HPP_INCLUDED

#include "Configuration.hpp"

#include <string>
#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Utility {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

class UTILITY_DLL_LINK I_EnvironmentHandler
{
    /// @name Types
    /// @{
public:
    typedef std::map<std::string, std::string>      Environment_type;
    /// @}

    /// @name I_EnvironmentHandler interface
    /// @{
public:
    /// Append variables to the environment.
    virtual void appendEnvironment(const Environment_type& _environment) = 0;

    /// Set the command line variables
    virtual void setCommandLine(int argc, const char* argv[]) = 0;

    /// Get the value of an environment variable
    /// @todo This only supports a one to one key-value pair, but possibly a more
    ///     advanced (multi-map or tree) structure might be more useful.
    virtual const std::string& getEnvironment(const std::string& _name) const = 0;

    /// Set the value of an environment variable
    /// @todo This only supports a one to one key-value pair, but possibly a more
    ///     advanced (multi-map or tree) structure might be more useful.
    virtual void setEnvironment(const std::string& _name, const std::string& _value) = 0;

    /// @brief Append a new handler to the existing one.
    ///
    /// New handlers are generally dynamic handlers attached to a database, LDAP
    /// or some other form of dynamic environment service.  The default handler
    /// is typically a static handler that gets values from the operating system,
    /// command line, and static configuration files.
    /// 
    /// @param _handler - Handler to append
    /// @param _before - True if _handler should be pre-pended before this handler,
    ///                  False if _handler should be appended after this handler.
    ///                  The order of handlers determines the order of precedence.
    virtual void appendHandler(I_EnvironmentHandler& _handler, bool _before) = 0;
    /// @}

    /// @name Static Methods 
    /// @{
public:
    /// Get the default environment handler
    static I_EnvironmentHandler& getDefaultHandler();
    /// @}

    /// @name 'Structors
    /// @{
protected:
			 I_EnvironmentHandler();
public:
	virtual ~I_EnvironmentHandler();
    /// @}

};	// interface I_EnvironmentHandler

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Utility
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_UTILITY_I_ENVIRONMENT_HANDLER_HPP_INCLUDED
