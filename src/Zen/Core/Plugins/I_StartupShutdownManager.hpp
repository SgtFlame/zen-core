//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Core Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C) 2008 - 2009 Matthew Alan Gray
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
//  Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_PLUGINS_I_STARTUP_SHUTDOWN_MANAGER_HPP_INCLUDED
#define ZEN_PLUGINS_I_STARTUP_SHUTDOWN_MANAGER_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>

#include <Zen/Core/Plugins/I_Configuration.hpp>

#include <Zen/Core/Threading/I_Condition.hpp>

#include <boost/noncopyable.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Plugins {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_StartupShutdownParticipant;

/// @brief Base StartupShutdown Manager
class PLUGINS_DLL_LINK I_StartupShutdownManager
:   boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_StartupShutdownParticipant>  pParticipant_type;
    typedef Zen::Event::Event<const std::string&>                   instanceEvent_type;
    /// @}

    /// @name I_StartupShutdownManager interface
    /// @{
public:
    /// Start the installed participants.
    virtual Zen::Threading::I_Condition* start() = 0;

    /// @brief Stop the installed participants
    ///
    /// This method will return when the manager is fully stopped and destroyed.
    /// Do not reference this object after stop() is called.
    virtual void stop() = 0;

    /// Install a participant into this manager.
    /// @param _name The name of the participant. This name must be unique to this manager.
    virtual void installParticipant(pParticipant_type _pParticipant) = 0;
    /// @}

    /// @name Static Methods
    /// @{
public:
    /// Get a startup-shutdown manager by name.
    ///
    /// The lifetime of a manager is from the time that getInstance()
    /// returns to the time that I_StartupShutdownManager::stop() is called and the 
    /// manager is fully stopped.
    static I_StartupShutdownManager& getInstance(const std::string& _instanceName);

    /// Start all initialized instances
    static void startAll();

    /// Stop and destroy all initialized instances
    static void stopAll();
    /// @}

    /// @name Events
    /// @{
public:
    instanceEvent_type      onStart;
    instanceEvent_type      onStop;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_StartupShutdownManager();
    virtual ~I_StartupShutdownManager();
    /// @}

    
};  // interface I_StartupShutdownManager

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Zen
}   // namespace Plugins
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_PLUGINS_I_STARTUP_SHUTDOWN_MANAGER_HPP_INCLUDED
