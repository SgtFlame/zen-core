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
#ifndef ZEN_UTILITY_LOG_STREAM_HPP_INCLUDED
#define ZEN_UTILITY_LOG_STREAM_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Core/Utility/I_Log.hpp>

#include <boost/iostreams/stream.hpp>

#include <ostream>
#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Utility {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

template <class sink_type>
class log_sink_t;

template <class sink_type>
class log_stream_t;

template <class sink_type>
class log_stream_buffer_t;

typedef log_sink_t< boost::iostreams::sink >    log_sink;
typedef log_stream_t< log_sink >                log_stream;
typedef log_stream_buffer_t< log_sink >         log_stream_buffer;

typedef log_sink_t< boost::iostreams::wsink >   log_wsink;
typedef log_stream_t< log_wsink >               log_wstream;
typedef log_stream_buffer_t< log_wsink >        log_wstream_buffer;

template <class sink_type>
class log_sink_t
:   public sink_type
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_Log>     pLog_type;
    /// @}

    /// @name log_sink implementation
    /// @{
public:
    std::streamsize write(const char* _s, std::streamsize _n)
    {
        std::string str(_s, static_cast<unsigned int>(_n));

        m_pLog->logMessage(str, m_logMessageLevel, m_maskDebug);

        return _n;
    }

    std::streamsize write(const wchar_t* _s, std::streamsize _n)
    {
        std::wstring str(_s, static_cast<unsigned int>(_n));

        m_pLog->logMessage(str, m_logMessageLevel, m_maskDebug);

        return _n;
    }
    /// @}

    /// @name 'Structors
    /// @{
public:
    log_sink_t(pLog_type _pLog, I_Log::LogMessageLevel _logMessageLevel, bool _maskDebug)
    :   m_pLog(_pLog)
    ,   m_logMessageLevel(_logMessageLevel)
    ,   m_maskDebug(_maskDebug)
    {}
    log_sink_t(const log_sink_t< sink_type >& _rhs)
    :   m_pLog(_rhs.m_pLog)
    ,   m_logMessageLevel(_rhs.m_logMessageLevel)
    ,   m_maskDebug(_rhs.m_maskDebug)
    {}
    virtual ~log_sink_t() {};
    /// @}

    /// @name Member Variables
    /// @{
protected:
    pLog_type               m_pLog;
    I_Log::LogMessageLevel  m_logMessageLevel;
    bool                    m_maskDebug;
    /// @}

};	// template class log_sink

template <class sink_type>
class log_stream_buffer_t
:   public boost::iostreams::stream_buffer<sink_type>
,   public sink_type
{
public:
    typedef Zen::Memory::managed_ptr<I_Log>     pLog_type;

    log_stream_buffer_t(pLog_type _pLog, I_Log::LogMessageLevel _logMessageLevel, bool _maskDebug)
    :   sink_type(_pLog, _logMessageLevel, _maskDebug)
    {
    }

    log_stream_buffer_t(const log_stream_buffer_t< sink_type >& _rhs)
    :   sink_type(_rhs)
    {
    }
};  // template class log_stream_buffer_t

template <class sink_type >
class log_stream_t
:   public boost::iostreams::stream< sink_type >
,   public sink_type
{
public:
    typedef Zen::Memory::managed_ptr<I_Log>     pLog_type;

    log_stream_t(pLog_type _pLog, I_Log::LogMessageLevel _logMessageLevel, bool _maskDebug)
    :   sink_type(_pLog, _logMessageLevel, _maskDebug)
    {
        open(*this);
    }

    log_stream_t(log_stream_buffer_t< sink_type>& _streambuf)
    :   boost::iostreams::stream< sink_type >(_streambuf)
    ,   sink_type(_streambuf)
    {
    }

};  // template class log_stream_t

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Utility
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_UTILITY_LOG_STREAM_HPP_INCLUDED
