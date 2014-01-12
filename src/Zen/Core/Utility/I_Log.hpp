//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Core Framework
//
// Copyright (C) 2001 - 2010 Tony Richards
// Copyright (C) 2008 - 2010 Matthew Alan Gray
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
#ifndef ZEN_UTILITY_I_LOG_HPP_INCLUDED
#define ZEN_UTILITY_I_LOG_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Scripting.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_self_ref.hpp>

#include <boost/iostreams/stream.hpp>
#include <boost/noncopyable.hpp>

#include <string>
#include <ostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Utility {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

template <class sink_type>
class log_sink_t;

template <class sink_type>
class log_stream_buffer_t;

typedef log_sink_t< boost::iostreams::sink >    log_sink;
typedef log_stream_buffer_t< log_sink >         log_stream_buffer;

class UTILITY_DLL_LINK I_Log
:   public Zen::Scripting::I_ScriptableType
,   public Zen::Memory::managed_self_ref<I_Log>
,   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    enum LogLevel
    {
        LL_LOW = 1,
        LL_NORMAL = 2,
        LL_BOREME = 3
    };

    enum LogMessageLevel
    {
        LML_TRIVIAL = 1,
        LML_NORMAL = 2,
        LML_CRITICAL = 3
    };

    typedef Zen::Memory::managed_ptr<I_Log>     pScriptObject_type;
    typedef Scripting::ObjectReference<I_Log>   ScriptObjectReference_type;
    typedef Scripting::ObjectReference<I_Log>   ScriptWrapper_type;
    /// @}

    /// @name I_Log interface
    /// @{
public:
    /// Return the name of the log.
    virtual const std::string& getName() const = 0;

    /// Get the debug output state for this log.
    virtual bool isDebugOutputEnabled() const = 0;

    /// Set the debug output state for this log.
    virtual void setDebugOutputEnabled(bool _isEnabled) = 0;

    /// Get the file output state for this log.
    virtual bool isFileOutputSuppressed() const = 0;

    /// Set the file output state for this log.
    virtual void setFileOutputSuppressed(bool _isEnabled) = 0;

    /// Get the log level for this log.
    virtual LogLevel getLogLevel() const = 0;

    /// Set the log level for this log.
    virtual void setLogLevel(LogLevel _logLevel) = 0;

    /// Log a message.
    virtual void logMessage(const std::string& _message, 
                            LogMessageLevel _logMessageLevel = LML_NORMAL, 
                            bool _maskDebug = false) = 0;

    /// Get a stream buffer object for targetting this log.
    virtual log_stream_buffer getStreamBuffer(LogMessageLevel _logMessageLevel = LML_NORMAL, bool _maskDebug = false) = 0;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual const std::string& getScriptTypeName();
    /// @}

    /// @name Events
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_Log();
    virtual ~I_Log();
    /// @}

};  // interface I_Log

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Utility
namespace Memory {
    // I_Log is managed by factory
    template<>
    struct is_managed_by_factory<Zen::Utility::I_Log>
    :   public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_UTILITY_I_LOG_HPP_INCLUDED
