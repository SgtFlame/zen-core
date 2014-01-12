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
#ifndef ZEN_UTILITY_I_LOG_SERVICE_HPP_INCLUDED
#define ZEN_UTILITY_I_LOG_SERVICE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Scripting.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_self_ref.hpp>

#include <Zen/Core/Utility/I_Log.hpp>

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

class UTILITY_DLL_LINK I_LogService
:   public Scripting::I_ScriptableService
,   public Zen::Memory::managed_self_ref<I_LogService>
,   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_LogService>          pScriptObject_type;
    typedef Zen::Scripting::ObjectReference<I_LogService>   ScriptObjectReference_type;
    typedef ScriptObjectReference_type                      ScriptWrapper_type;

    typedef Zen::Memory::managed_ptr<I_Log>                 pLog_type;
    /// @}

    /// @name I_LogService interface
    /// @{
public:
    /// Creates a new log with the given name.
    /// @param _name The name to give the log (i.e. 'Zen.log')
    /// @param _isDefault If true, this is the default log. The
    ///     first log created is always the default log unless 
    ///     this is set.
    /// @param _debuggerOutput If true, output to this log will 
    ///     also be output to the console/debugger's output.
    /// @param _suppressFileOutput If true, no file output will
    ///     be written for this log.
    virtual pLog_type createLog(const std::string& _name, 
                                bool _isDefault = false, 
                                bool _debuggerOutput = true, 
                                bool _suppressFileOutput = false) = 0;

    /// Gets a log managed by this class.
    virtual pLog_type getLog(const std::string& _name) = 0;

    /// Gets the default log.
    virtual pLog_type getDefaultLog() = 0;

    /// Sets the passed log as the default log.
    /// @returns The previous default log.
    virtual pLog_type setDefaultLog(pLog_type _pLog) = 0;

    /// Log a message to the default log.
    virtual void logMessage(const std::string& _message, 
                            I_Log::LogMessageLevel _logMessageLevel = I_Log::LML_NORMAL, 
                            bool _maskDebug = false) = 0;

    /// Get a stream for the default log.
    virtual log_stream_buffer getStreamBuffer(I_Log::LogMessageLevel _logMessageLevel = I_Log::LML_NORMAL,
                                 bool _maskDebug = false) = 0;

    /// Set the level of detail of the default log.
    virtual void setLogDetail(I_Log::LogLevel _logLevel) = 0;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual const std::string& getScriptTypeName();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_LogService();
    virtual ~I_LogService();
    /// @}

};  // interface I_LogService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Utility
namespace Memory {
    // I_LogService is managed by factory
    template<>
    struct is_managed_by_factory<Zen::Utility::I_LogService>
    :   public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_UTILITY_I_LOG_SERVICE_HPP_INCLUDED
