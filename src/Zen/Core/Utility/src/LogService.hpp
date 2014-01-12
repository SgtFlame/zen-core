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
#ifndef ZEN_UTILITY_LOG_SERVICE_HPP_INCLUDED
#define ZEN_UTILITY_LOG_SERVICE_HPP_INCLUDED

#include "../I_LogService.hpp"

#include <Zen/Core/Scripting.hpp>

#include <Zen/Core/Threading/I_Mutex.hpp>

#include <map>
#include <string>
#include <ostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Utility {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class LogService
:   public I_LogService
{
    /// @name Types
    /// @{
public:
    typedef pLog_type::weak_ptr_type    wpLog_type;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual Zen::Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name I_ScriptableService implementation
    /// @{
public:
    virtual void registerScriptEngine(pScriptEngine_type _pScriptEngine);
    /// @}

    /// @name I_LogService implementation
    /// @{
public:
    virtual pLog_type createLog(const std::string& _name, 
                                bool _isDefault = false, 
                                bool _debuggerOutput = true, 
                                bool _suppressFileOutput = false);
    virtual pLog_type getLog(const std::string& _name);
    virtual pLog_type getDefaultLog();
    virtual pLog_type setDefaultLog(pLog_type _pLog);
    virtual void logMessage(const std::string& _message, 
                            I_Log::LogMessageLevel _logMessageLevel = I_Log::LML_NORMAL, 
                            bool _maskDebug = false);
    virtual log_stream_buffer getStreamBuffer(I_Log::LogMessageLevel _logMessageLevel = I_Log::LML_NORMAL,
                                 bool _maskDebug = false);
    virtual void setLogDetail(I_Log::LogLevel _logLevel);
    /// @}

    /// @name LogService implementation
    /// @{
public:
    pScriptModule_type getScriptModule();
    const std::string& getName() const;
    void destroyLog(wpLog_type _pLog);
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class LogManager;
             LogService(const std::string& _name);
    virtual ~LogService();
    /// @}

    /// @name Member Variables
    /// @{
private:
    std::string                                 m_name;
    pScriptEngine_type                          m_pScriptEngine;
    Zen::Scripting::script_module*              m_pScriptModule;
    Zen::Scripting::I_ObjectReference*          m_pScriptObject;

    typedef std::map<std::string, pLog_type>    Logs_type;
    Logs_type                                   m_logs;
    Zen::Threading::I_Mutex*                    m_pLogsMutex;

    pLog_type                                   m_pDefaultLog;
    /// @}

};  // class LogService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Utility
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_UTILITY_LOG_SERVICE_HPP_INCLUDED
