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

#include "../I_Thread.hpp"

#if defined(HOST_POSIX) || defined(__APPLE__)
#include "Thread_posix.hpp"
#endif

#if defined(HOST_SOLARIS)
#include "Thread_solaris.hpp"
#endif

#if     defined(HOST_WIN32)
    #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
    #endif
    #include "Windows.h"    // ::Sleep()
	#include "Thread_win32.hpp"
#elif   defined(HOST_POSIX) || defined(HOST_SOLARIS) || defined(__APPLE__)
    #include "unistd.h"     // ::sleep(), ::usleep()
#else
    #error  SleepForSeconds() cannot determine the proper system call to use.
#endif

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Threading {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

void
I_Thread::sleepForMilliseconds (unsigned const _milliseconds)
{
#if     defined(HOST_WIN32)
    ::Sleep(_milliseconds);         // milliseconds
#elif   defined(HOST_POSIX) || defined(HOST_SOLARIS) || defined(__APPLE__)
    ::usleep(_milliseconds * 1000); // microseconds
#endif
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_Thread::I_Thread()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_Thread::~I_Thread()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_Thread::ThreadId
I_Thread::getCurrentThreadId()
{
#if defined(HOST_WIN32)
    return Thread_win32::getWin32CurrentThreadId();
#elif defined(HOST_POSIX) || defined(__APPLE__)
    return Thread_posix::getPosixCurrentThreadId();
#elif defined(HOST_SOLARIS)
    return Thread_solaris::getSolarisCurrentThreadId();
#else
#error Platform not supported
#endif
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Threading
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
