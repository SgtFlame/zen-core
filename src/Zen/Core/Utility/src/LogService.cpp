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

#include "LogService.hpp"
#include "Log.hpp"

#include <Zen/Core/Threading/MutexFactory.hpp>
#include <Zen/Core/Threading/CriticalSection.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <boost/bind.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Utility {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
LogService::LogService(const std::string& _name)
:   m_name(_name)
,   m_pScriptObject(NULL)
,   m_pScriptEngine()
,   m_pScriptModule(NULL)
,   m_pLogsMutex(Zen::Threading::MutexFactory::create())
,   m_logs()
,   m_pDefaultLog()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
LogService::~LogService()
{
    Zen::Threading::MutexFactory::destroy(m_pLogsMutex);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
LogService::pLog_type 
LogService::createLog(const std::string& _name, 
                      bool _isDefault, 
                      bool _debuggerOutput,
                      bool _suppressFileOutput)
{
    Zen::Threading::CriticalSection guard(m_pLogsMutex);

    Logs_type::iterator iter = m_logs.find(_name);
    if (iter == m_logs.end())
    {
        Log* pRawLog = new Log(*this, _name, _debuggerOutput, _suppressFileOutput);
        pLog_type pLog(
            pRawLog,
            boost::bind(&LogService::destroyLog, this, _1)
        );

        if ( !m_pDefaultLog.isValid() || _isDefault )
        {
            m_pDefaultLog = pLog;
        }

        m_logs[_name] = pLog;

        return pLog;
    }
    else
    {
        /// TODO Throw exception?
        return iter->second;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
LogService::pLog_type 
LogService::getLog(const std::string& _name)
{
    Zen::Threading::CriticalSection guard(m_pLogsMutex);

    Logs_type::iterator iter = m_logs.find(_name);
    if (iter != m_logs.end())
    {
        return iter->second;
    }
    else
    {
        throw Zen::Utility::runtime_exception("LogService::getLog() : Log does not exist.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
LogService::pLog_type 
LogService::getDefaultLog()
{
    Zen::Threading::CriticalSection guard(m_pLogsMutex);

    return m_pDefaultLog;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
LogService::pLog_type 
LogService::setDefaultLog(pLog_type _pLog)
{
    Zen::Threading::CriticalSection guard(m_pLogsMutex);

    pLog_type pOldLog = m_pDefaultLog;
    m_pDefaultLog = _pLog;
    return pOldLog;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
LogService::logMessage(const std::string& _message, 
                       I_Log::LogMessageLevel _logMessageLevel, 
                       bool _maskDebug)
{
    Zen::Threading::CriticalSection guard(m_pLogsMutex);

    if (m_pDefaultLog.isValid())
    {
        m_pDefaultLog->logMessage(_message, _logMessageLevel, _maskDebug);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
log_stream_buffer
LogService::getStreamBuffer(I_Log::LogMessageLevel _logMessageLevel,
                      bool _maskDebug)
{
    Zen::Threading::CriticalSection guard(m_pLogsMutex);

    if (m_pDefaultLog.isValid())
    {
        return m_pDefaultLog->getStreamBuffer(_logMessageLevel, _maskDebug);
    }
    else
    {
        throw Zen::Utility::runtime_exception("LogService::getStream() : Default log not found");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
LogService::setLogDetail(I_Log::LogLevel _logLevel)
{
    Zen::Threading::CriticalSection guard(m_pLogsMutex);

    if (m_pDefaultLog.isValid())
    {
        m_pDefaultLog->setLogLevel(_logLevel);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Scripting::I_ObjectReference*
LogService::getScriptObject()
{
    if (m_pScriptObject == NULL)
    {
        m_pScriptObject = new ScriptWrapper_type(
            getScriptModule(),
            getScriptModule()->getScriptType(getScriptTypeName()),
            this->getSelfReference().lock()
        );
    }

    return m_pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
LogService::pScriptModule_type
LogService::getScriptModule()
{
    return m_pScriptModule->getScriptModule();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
LogService::registerScriptEngine(pScriptEngine_type _pScriptEngine)
{
    // Only do this once.
    assert(m_pScriptModule == NULL);

    m_pScriptEngine = _pScriptEngine;

    static Zen::Scripting::script_module module(_pScriptEngine, "Logging");

    // Keep a reference to the module.
    m_pScriptModule = &module;

    module.addType<I_LogService>(getScriptTypeName(), "Log Service")
        .addMethod("createLog", &I_LogService::createLog)
        .addMethod("getLog", &I_LogService::getLog)
        .addMethod("getDefaultLog", &I_LogService::getDefaultLog)
        .addMethod("setDefaultLog", &I_LogService::setDefaultLog)
        .addMethod("logMessage", &I_LogService::logMessage)
        .addMethod("setLogDetail", &I_LogService::setLogDetail)
        .createGlobalObject(m_name, this)
    ;

    // ScriptAction
    module.addType<I_Log>("Log", "Log Object")
        .addMethod("getName", &I_Log::getName)
        .addMethod("isDebugOutputEnabled", &I_Log::isDebugOutputEnabled)
        .addMethod("setDebugOutputEnabled", &I_Log::setDebugOutputEnabled)
        .addMethod("isFileOutputSuppressed", &I_Log::isFileOutputSuppressed)
        .addMethod("setFileOutputSuppressed", &I_Log::setFileOutputSuppressed)
        .addMethod("getLogLevel", &I_Log::getLogLevel)
        .addMethod("setLogLevel", &I_Log::setLogLevel)
        .addMethod("logMessage", &I_Log::logMessage)
    ;

    module.activate();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
LogService::getName() const
{
    return m_name;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
LogService::destroyLog(wpLog_type _pLog)
{
    Log* pLog =
        dynamic_cast<Log*>(_pLog.get());

    if (pLog)
    {
        delete pLog;
    }
    else
    {
        throw Zen::Utility::runtime_exception("LogService::destroy() : Invalid type.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Utility
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
