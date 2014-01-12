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

#include "Thread_posix.hpp"

#include "../I_Runnable.hpp"
#include "../SpinLock.hpp"
#include "../CriticalSection.hpp"

#include <stdexcept>
#include <sstream>
#include <map>

#include <unistd.h>
#include <sys/errno.h>
#include <assert.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Threading {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

Thread_posix::Thread_posix(I_Runnable *const _pRunnable)
:   m_pRunnable(_pRunnable)
,   m_nativeThread()
,   m_threadId()
,   m_isStarted(false)
,   m_isJoined(false)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Thread_posix::~Thread_posix()
{
    if (m_isStarted)
    {
        stop(); // might or might not be helpful
        join();
    }

    delete m_pRunnable;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Thread_posix::start()
{
    if (!m_isStarted)
    {
        if (m_pRunnable != NULL)
        {
            const int rc = ::pthread_create(&m_nativeThread, NULL, ThreadFunction, this);
            if (rc != 0)
            {
                std::ostringstream ErrMsg;
                ErrMsg << "Thread_posix::Start(): pthread_create() failed, rc='"<<rc<<"'.";
                throw std::runtime_error(ErrMsg.str());
            }
            m_threadId.m_pNativeThreadId = new NativeThreadId_posix(m_nativeThread);
            m_isStarted = true;
        }
        else
        {
            throw std::runtime_error("Thread_posix::Start(): Missing the required I_Runnable.");
        }
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Thread_posix::stop()
{
    if (m_isStarted && (m_pRunnable != NULL))
    {
        m_pRunnable->stop();  // not all I_Runnables honor the Stop request, some just run to completion
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Thread_posix::join()
{
    if (m_isStarted && !m_isJoined)
    {
        const int rc = ::pthread_join(m_nativeThread, NULL);
        if (rc != 0)
        {
            std::ostringstream ErrMsg;
            ErrMsg << "Thread_posix::Join(): pthread_join() failed, rc='"<<rc<<"'.";
            throw std::runtime_error(ErrMsg.str());
        }
        m_isJoined = true;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_Thread::ThreadId
Thread_posix::getPosixCurrentThreadId()
{
    return ThreadId(new NativeThreadId_posix(::pthread_self()));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const I_Thread::ThreadId&
Thread_posix::getThreadId() const
{
    return m_threadId;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
//DWORD WINAPI
void*
(Thread_posix::ThreadFunction(void* _pThis))
{
    Thread_posix* const pThis = static_cast <Thread_posix*> (_pThis);

    try
    {
        pThis->m_pRunnable->run();
    }
    catch(...)
    {
    }

    return 0;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
Thread_posix::NativeThreadId_posix::operator==(const I_Thread::ThreadId::I_NativeThreadId& _id) const
{
    const NativeThreadId_posix* const pNativeThreadId_posix = static_cast<const NativeThreadId_posix*>(&_id);
    return (pNativeThreadId_posix != NULL) && (::pthread_equal(m_nativeThreadId, pNativeThreadId_posix->m_nativeThreadId) != 0);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
Thread_posix::NativeThreadId_posix::operator!=(const I_Thread::ThreadId::I_NativeThreadId& _id) const
{
    const NativeThreadId_posix* const pNativeThreadId_posix = static_cast<const NativeThreadId_posix*>(&_id);
    return (pNativeThreadId_posix == NULL) || (::pthread_equal(m_nativeThreadId, pNativeThreadId_posix->m_nativeThreadId) == 0);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
Thread_posix::NativeThreadId_posix::operator<(const I_Thread::ThreadId::I_NativeThreadId& _id) const
{
    const NativeThreadId_posix* const pNativeThreadId_posix = dynamic_cast<const NativeThreadId_posix*>(&_id);
    assert(pNativeThreadId_posix != NULL);
    return (m_nativeThreadId < pNativeThreadId_posix->m_nativeThreadId);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_Thread::ThreadId::I_NativeThreadId*
Thread_posix::NativeThreadId_posix::clone() const
{
    return new NativeThreadId_posix(m_nativeThreadId);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
std::string
Thread_posix::NativeThreadId_posix::toString() const
{
    std::ostringstream oStream;
    oStream << std::hex << std::uppercase << "0x" << m_nativeThreadId;
    return oStream.str();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Threading
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // THREADMODEL_POSIX
