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
#ifndef ZEN_UTILITY_LOG_HPP_INCLUDED
#define ZEN_UTILITY_LOG_HPP_INCLUDED

#include "../I_Log.hpp"

#include "../log_stream.hpp"

#include <fstream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Utility {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class LogService;

class Log
:   public I_Log
{
    /// @name Types
    /// @{
public:
    enum {ZEN_LOG_THRESHOLD = 4};
    typedef Zen::Memory::managed_ptr<I_Log>		pLog_type;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual Zen::Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name I_Log implementation
    /// @{
public:
    virtual const std::string& getName() const;
    virtual bool isDebugOutputEnabled() const;
    virtual void setDebugOutputEnabled(bool _isEnabled);
    virtual bool isFileOutputSuppressed() const;
    virtual void setFileOutputSuppressed(bool _isEnabled);
    virtual LogLevel getLogLevel() const;
    virtual void setLogLevel(LogLevel _logLevel);
    virtual void logMessage(const std::string& _message, 
                            LogMessageLevel _logMessageLevel = LML_NORMAL, 
                            bool _maskDebug = false);
    virtual log_stream_buffer getStreamBuffer(LogMessageLevel _logMessageLevel = LML_NORMAL, bool _maskDebug = false);
    /// @}

    /// @name Log implementation
    /// @{
public:
    pScriptModule_type getScriptModule();
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class LogService;
             Log(LogService& _service, const std::string& _name, bool _debuggerOutput, bool _suppressFileOutput);
    virtual ~Log();
    /// @}

    /// @name Member Variables
    /// @{
private:
    LogService&                             m_service;

    Zen::Scripting::I_ObjectReference*      m_pScriptObject;

    std::ofstream                           m_fileLog;
    LogLevel                                m_logLevel;
    bool                                    m_debugOutput;
    bool                                    m_suppressFile;
    std::string                             m_logName;

    Zen::Threading::I_Mutex*                m_pLogMutex;
    /// @}

};  // class Log

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Utility
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_UTILITY_LOG_HPP_INCLUDED
