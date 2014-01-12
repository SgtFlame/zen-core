//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Core Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
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
//
//  Original design by:
//  John S. Givler, Ph.D.(Computer Science) - Dr.John.S.Givler@Givler.com
//
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_THREADING_THREAD_WIN32_HPP_INCLUDED
#define ZEN_THREADING_THREAD_WIN32_HPP_INCLUDED

#if defined(HOST_WIN32)
#include "../I_Thread.hpp"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#ifndef USE_CPP_THREAD
#include <process.h>
#endif

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Threading {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_Runnable;

class Thread_win32
:	public I_Thread
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_Thread implementation
    /// @{
public:
    virtual void start();
    virtual void stop();
    virtual void join();
    static ThreadId getWin32CurrentThreadId();
    virtual const ThreadId& getThreadId() const;
    /// @}

    /// @name Inner classes
    /// @{
public:
    class NativeThreadId_win32
    :   public I_Thread::ThreadId::I_NativeThreadId
    {
        /// @name Friend declarations
        /// @{
    private:
        friend class Thread_win32;
        /// @}

        /// @name NativeThreadId_win32 implementation
        /// @{
    public:
        virtual bool operator==(const I_NativeThreadId& _otherId) const;
        virtual bool operator!=(const I_NativeThreadId& _otherId) const;
        virtual bool operator< (const I_NativeThreadId& _otherId) const;
        virtual I_NativeThreadId* clone() const;
        virtual std::string toString() const;
        /// @}

        /// @name 'Structors
        /// @{
    public:
                 NativeThreadId_win32() : m_nativeThreadId() {}
                 NativeThreadId_win32(::DWORD const _id) : m_nativeThreadId(_id) {}
        virtual ~NativeThreadId_win32() {}
        /// @}

        /// @name Member variables
        /// @{
    private:
        ::DWORD m_nativeThreadId;
        /// @}

    };  // class NativeThreadId_win32
    /// @}

    /// @name 'Structors
    /// @{
public:
             Thread_win32(I_Runnable* const);
    virtual ~Thread_win32();
    /// @}

	/// @name Additional implementation
	/// @{
public:
    /// This really shoud be private, so do not call it.
    static DWORD WINAPI threadFunction(LPVOID);
	/// @}

	/// @name Member Variables
	/// @{
private:
    I_Runnable*		m_pRunnable;
    ::HANDLE		m_threadHandle;
    ThreadId        m_threadId;
    volatile bool   m_isStarted;
    volatile bool   m_isJoined;
	/// @}

};	// interface Thread_win32

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Threading
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // HOST_WIN32

#endif // ZEN_THREADING_THREAD_WIN32_HPP_INCLUDED
