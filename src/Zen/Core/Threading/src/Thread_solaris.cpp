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

#include "Thread_solaris.hpp"

#include "../I_Runnable.hpp"
#include "../SpinLock.hpp"
#include "../CriticalSection.hpp"
#include <stdexcept>
#include <sstream>
#include <map>
#include <unistd.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Threading {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

Thread_solaris::Thread_solaris(I_Runnable *const _pRunnable)
:	m_pRunnable(_pRunnable)
,   m_nativeThread()
,   m_threadId()
,   m_isStarted(false)
,   m_isJoined(false)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Thread_solaris::~Thread_solaris()
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
Thread_solaris::start()
{
	if (!m_isStarted)
	{
        const int rc = ::thr_create(NULL, 0, ThreadFunction, this, 0, &m_NativeThread);
        if (rc != 0)
        {
            std::ostringstream ErrMsg;
            ErrMsg << "Thread_solaris::Start(): thr_create() failed, rc='"<<rc<<"'.";
            throw std::runtime_error(ErrMsg.str());
        }
        m_ThreadId.m_pNativeThreadId = new NativeThreadId_solaris(m_NativeThread);
        m_HaveStarted = true;
    }
    else
    {
        throw std::runtime_error("Thread_solaris::Start(): Missing the required I_Runnable.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Thread_solaris::stop()
{
    if (m_isStarted && (m_pRunnable != NULL))
    {
        m_pRunnable->stop();  // not all I_Runnables honor the Stop request, some just run to completion
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Thread_solaris::join()
{
    if (m_isStarted && !m_isJoined)
    {
        const int rc = ::thr_join(m_NativeThread, NULL, NULL);
        if (rc != 0)
        {
            std::ostringstream ErrMsg;
            ErrMsg << "Thread_solaris::Join(): thr_join() failed, rc='"<<rc<<"'.";
            throw std::runtime_error(ErrMsg.str());
        }
        m_HaveJoined = true;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_Thread::ThreadId
Thread_solaris::getSolarisCurrentThreadId()
{
    return ThreadId(new NativeThreadId_solaris(::thr_self()));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const I_Thread::ThreadId&
Thread_solaris::getThreadId() const
{
    return m_threadId;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DWORD WINAPI
Thread_solaris::threadFunction (::LPVOID _pThis)
{
    Thread_solaris* const pThis = static_cast <Thread_solaris*> (_pThis);
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
Thread_solaris::NativeThreadId_solaris::operator==(const I_Thread::ThreadId::I_NativeThreadId& _id) const
{
    const NativeThreadId_solaris* const pNativeThreadId_solaris = static_cast<const NativeThreadId_solaris*>(&_id);
    return (pNativeThreadId_solaris != NULL) && (m_nativeThreadId == pNativeThreadId_solaris->m_nativeThreadId);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
Thread_solaris::NativeThreadId_solaris::operator!=(const I_Thread::ThreadId::I_NativeThreadId& _id) const
{
    const NativeThreadId_solaris* const pNativeThreadId_solaris = static_cast<const NativeThreadId_solaris*>(&_id);
    return (pNativeThreadId_solaris == NULL) || (m_nativeThreadId != pNativeThreadId_solaris->m_nativeThreadId);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_Thread::ThreadId::I_NativeThreadId*
Thread_solaris::NativeThreadId_solaris::clone() const
{
    return new NativeThreadId_solaris(m_nativeThreadId);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
std::string
Thread_solaris::NativeThreadId_solaris::toString() const
{
    std::ostringstream oStream;
    oStream << std::hex << std::uppercase << "0x" << m_nativeThreadId;
    return oStream.str();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Threading
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // THREADMODEL_SOLARIS
