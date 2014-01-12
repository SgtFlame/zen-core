
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Core Framework
//
// Copyright (C) 2006 - 2008 John Givler
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
//  John S. Givler, Ph.D.(Computer Science) - Dr.John.S.Givler@Givler.com
//  Tony Richards                           - trichards@indiezen.com
//
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifdef THREADMODEL_POSIX

#include "Condition_posix.hpp"


#include "../CriticalSection.hpp"

// ISO C++
#include <stdexcept>
#include <iostream>
#include <sstream>

// ISO C
#include <errno.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Threading {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

Condition_posix::Condition_posix(const bool _initialState)
:   m_nativeCondition()
,   m_exclusiveAccessLock()
,   m_isAsserted(_initialState)
,   m_threadsAreBlocked(false)
{
    const int rc = ::pthread_cond_init(&m_nativeCondition, NULL);
    if (rc != 0)
    {
        #ifndef NDEBUG
        std::cerr << "Condition_posix::Condition_posix(): pthread_cond_init() failed, rc="<<rc<<": "<<getNameOfError(rc)<<"." << std::endl;
        #endif  // ! NDEBUG
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Condition_posix::~Condition_posix()
{
    if (m_threadsAreBlocked)
    {
        assertCondition();       // wake up all waiting threads (in a properly-designed application, there should be none)
    }
    const int rc = ::pthread_cond_destroy(&m_nativeCondition);
    if (rc != 0)
    {
        #ifndef NDEBUG
        std::cerr << "Condition_posix::~Condition_posix(): pthread_cond_destroy() failed, rc="<<rc<<": "<<getNameOfError(rc)<<"." << std::endl;
        #endif  // ! NDEBUG
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_Condition&
Condition_posix::assertCondition()
{
    CriticalSection for_isAsserted (m_exclusiveAccessLock);
    m_isAsserted = true;
    if (m_threadsAreBlocked)
    {
        m_threadsAreBlocked = false;    // all currently-blocked threads are about to be awakened
        const int rc = ::pthread_cond_broadcast(&m_nativeCondition);
        if (rc != 0)
        {
            std::ostringstream ErrMsg;
            ErrMsg << "Condition_posix::Assert(): pthread_cond_broadcast() failed, rc="<<rc<<": "<<getNameOfError(rc)<<".";
            throw std::runtime_error(ErrMsg.str());
        }
    }
    return *this;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_Condition&
Condition_posix::retractCondition()
{
    CriticalSection for_IsAsserted (m_exclusiveAccessLock);
    m_isAsserted = false;
    return *this;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_Condition&
Condition_posix::requireCondition()
{
    CriticalSection for_IsAsserted (m_exclusiveAccessLock);
    if (!m_isAsserted)
    {
        m_threadsAreBlocked = true;
        const int rc = ::pthread_cond_wait(&m_nativeCondition, &m_exclusiveAccessLock.getNativeMutex());
        if (rc != 0)
        {
            std::ostringstream ErrMsg;
            ErrMsg << "Condition_posix::Require(): cond_wait() failed, rc='"<<rc<<"'.";
            throw std::runtime_error(ErrMsg.str());
        }
    }
    return *this;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
Condition_posix::getNameOfError(const int _errNo)
{
    // similar to strerror()

    static const std::string eTimedOut  ("ETIMEDOUT");
    static const std::string eDeadLk    ("EDEADLK");
    static const std::string eNoMem     ("ENOMEM");
    static const std::string eAgain     ("EAGAIN");
    static const std::string eInval     ("EINVAL");
    static const std::string eBusy      ("EBUSY");
    static const std::string eSrch      ("ESRCH");
    static const std::string ePerm      ("EPERM");
    static const std::string NoClue     ("<unknown>");

    switch (_errNo)
    {
    case ETIMEDOUT:
        return eTimedOut;
    case EDEADLK:
        return eDeadLk;
    case ENOMEM:
        return eNoMem;
    case EAGAIN:
        return eAgain;
    case EINVAL:
        return eInval;
    case EBUSY:
        return eBusy;
    case ESRCH:
        return eSrch;
    case EPERM:
        return ePerm;
    default:
        return NoClue;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Threading
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // THREADMODEL_POSIX
