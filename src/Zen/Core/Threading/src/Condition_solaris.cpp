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
#ifdef THREADMODEL_SOLARIS

#include "Condition_solaris.hpp"

#include "Mutex_solaris.hpp"
#include "../CriticalSection.hpp"
#include "../MutexFactory.hpp"

#include <stdexcept>
#include <iostream>
#include <strstream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Threading {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

Condition_solaris::Condition_solaris(const bool _initialState)
:	m_isAsserted(_initialState)
,   m_threadsAreBlocked(false)
,   m_mutex()
,   m_pNativeMutex(&m_Mutex.getNativeMutex())
,   m_nativeCondition()
{
    const int rc = ::cond_init(&m_nativeCondition, USYNC_THREAD, NULL);
    if (rc != 0)
    {
        #ifndef NDEBUG
        std::cerr << "Condition_win32::Condition_win32(): cond_init() failed, rc='"<<rc<<"'." << std::endl;
        #endif  // ! NDEBUG
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Condition_solaris::~Condition_solaris()
{
    if (m_threadsAreBlocked)
    {
        assertCondition();       // wake up all waiting threads (in a proper design, there should be none)
    }

    const int rc = ::cond_destroy(&m_nativeCondition);
    if (rc != 0)
    {
        #ifndef NDEBUG
        std::cerr << "Condition_win32::~Condition_win32(): cond_destroy() failed, rc='"<<rc<<"'." << std::endl;
        #endif  // ! NDEBUG
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_Condition&
Condition_solaris::assertCondition()
{
    CriticalSection for_isAsserted_and_broadcast(m_mutex);
    m_isAsserted = true;
    if (m_threadsAreBlocked)
    {
        m_ThreadsAreBlocked = false;    // all condition-blocked threads will be awakened
        const int rc = ::cond_broadcast(&m_NativeCondition);
        if (rc != 0)
        {
            std::ostringstream ErrMsg;
            ErrMsg << "Condition_solaris::Assert(): cond_broadcast() failed, rc='"<<rc<<"'.";
            throw std::runtime_error(ErrMsg.str());
        }
    }
	return *this;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_Condition&
Condition_solaris::retractCondition()
{
    CriticalSection for_IsAsserted (m_Mutex);
    m_isAsserted = false;
    return *this;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_Condition&
Condition_solaris::requireCondition()
{
    CriticalSection for_IsAsserted (m_Mutex);
    if (!m_IsAsserted)
    {
        m_ThreadsAreBlocked = true;
        const int rc = ::cond_wait(&m_NativeCondition, m_pNativeMutex);
        if (rc != 0)
        {
            std::ostringstream ErrMsg;
            ErrMsg << "Condition_solaris::Require(): cond_wait() failed, rc='"<<rc<<"'.";
            throw std::runtime_error(ErrMsg.str());
        }
    }
    return *this;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Threading
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // THREADMODEL_SOLARIS
