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
#ifndef ZEN_THREADING_SPIN_LOCK_HPP_INCLUDED
#define ZEN_THREADING_SPIN_LOCK_HPP_INCLUDED

#include <Zen/Core/Threading/I_Mutex.hpp>
#include <Zen/Core/Utility/runtime_exception.hpp>

//#define SPINLOCK_IS_WRAPPER

// C++
#include <stdexcept>

#if   defined(HOST_POSIX) || defined(HOST_SOLARIS)
    #include <unistd.h>                             // ::sleep(), ::usleep()
#elif defined(HOST_WIN32) || defined(HOST_WIN64)
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include <Windows.h>                            // ::Sleep()
#endif

#if  !defined(SPINLOCK_IS_WRAPPER)

    #if   defined(_MSC_VER)
        // Have to revert to SPINLOCK_IS_WRAPPER in x64 since inline __asm is not supported.
        #if defined(_WIN64)
            #define SPINLOCK_IS_WRAPPER
        #endif

        // "SpinLock" will use true hardware-based spin locks.
        #if !defined(HOST_CPU_X86) && !defined(HOST_CPU_X86_64)
            #ifndef WIN32_LEAN_AND_MEAN
            #define WIN32_LEAN_AND_MEAN
            #endif
            #include <Windows.h>                    // ::InterlockedExchange()
        #endif
    #elif defined(__APPLE__)
        #include <libkern/OSAtomic.h>
        #define APPLE_ATOMIC_MEMORY_ACCESS_API
        typedef int _Atomic_word;
    #elif defined(__GNUG__)
        // "SpinLock" might use true hardware-based spin locks.
        #if !defined(HOST_CPU_X86) && !defined(HOST_CPU_X86_64)
            // at some point around 4.2, gcc moved bits/atomicity.h to ext/atomicity.h
            #if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 2)) //&& ( !__APPLE__ )
                #include <ext/atomicity.h>
            #else
                #include <bits/atomicity.h>             // ::__gnu_cxx::__exchange_and_add(), ::_Atomic_word
            #endif
        #endif
        #if (__GNUC__ > 4) || ((__GNUC__ == 4) && ((__GNUC_MINOR__ > 1) || ((__GNUC_MINOR__ == 1) && !defined(HOST_SOLARIS))))
            #define GNUC_ATOMIC_MEMORY_ACCESS_API   // ::__sync_lock_test_and_set(), ::__sync_lock_release()
        #endif
    #elif defined(SPINLOCK_ENABLE_OPENMP)
        // try #pragma omp critical
    #else
        // "SpinLock" will be a thin wrapper for the platform's native locks.
        #define SPINLOCK_IS_WRAPPER
    #endif

    #ifndef SPINLOCK_NUMBER_OF_SHEEP
        #define SPINLOCK_NUMBER_OF_SHEEP 1000
    #endif  // ! SPINLOCK_NUMBER_OF_SHEEP

#endif  // ! SPINLOCK_IS_WRAPPER

#if   defined(SPINLOCK_IS_WRAPPER)
    #if defined(THREADMODEL_SOLARIS) || defined(THREADMODEL_POSIX) || defined(THREADMODEL_WIN32)  // or anything else we support
        // "SpinLock" will be a thin wrapper for the platform's native locks.
        #include "MutexFactory.hpp"
    #else
        // "SpinLock" cannot be implemented at all.
        #error  "Spin locks are not supported for this compiler on this platform."
    #endif
#endif

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Threading {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class THREADING_DLL_LINK SpinLock
: public I_Mutex
{
public:

    #if   defined(SPINLOCK_IS_WRAPPER)

        SpinLock (const long =0)
        {
             m_pMutex = MutexFactory::create();
        }
        virtual
       ~SpinLock ()
        {
            MutexFactory::destroy(m_pMutex);
        }

    #elif  defined(_MSC_VER) || defined(__GNUG__) || defined(SPINLOCK_ENABLE_OPENMP)

        SpinLock (const long _SizeOfFlock =SPINLOCK_NUMBER_OF_SHEEP) : m_AmLocked(false), m_NumSheep(_SizeOfFlock)
        {
        }
        virtual
       ~SpinLock ()
        {
        }

    #else

        #error  "Spin locks are not supported for this compiler on this platform."

    #endif

    /// @name I_Mutex implementation
    /// @{
public:
    virtual void    acquire()   {xAcquire();}
    virtual void    release()   {xRelease();}
    /// @}

    //----------------------------------------------------------------------------
    // <in-line interface (experimental)>
    //----------------------------------------------------------------------------
    inline
    void     xAcquire ()
    {
    #if   defined(SPINLOCK_IS_WRAPPER)

        m_pMutex->acquire();

    #elif defined(_MSC_VER) || defined(__GNUG__) || defined(SPINLOCK_ENABLE_OPENMP)

        long RemainingSheep = m_NumSheep;  // we will count sheep before going to Sleep()
        long PreviousFlag   = 1;
        while ((--RemainingSheep >= 0) && ((PreviousFlag = xTestAndSet(&m_AmLocked)) != 0))
        {
            #if 0

            // Don't wait; just waste processor cycles until we get the lock.
            // On some x86 CPU micro-architectures (e.g. Intel Core2), there is a very large penalty for doing this (should 'pause', instead; see below).

            #elif defined(_MSC_VER) && (defined(HOST_CPU_X86) || defined(HOST_CPU_X86_64))

            __asm
            {
                pause
            } // __asm

            #elif defined(__GNUG__) && (defined(HOST_CPU_X86) || defined(HOST_CPU_X86_64))

            __asm__  __volatile__
            (
                "pause"
                :  // output
                :  // input
                :  // clobbered registers
            );  // __asm__  __volatile__

            #else

            // Don't wait; just waste processor cycles until we get the lock.
            // Damn the torpedoes, full speed ahead!

            #endif
        }
        if (PreviousFlag != 0) while (xTestAndSet(&m_AmLocked))
        {
            // We must wait for another thread to release the lock.
            // Of course, that other thread may not be running now, because we are running instead ...
            #if 1

                // relinquish our time slice (the most civilized choice!)
                #if defined(HOST_POSIX) || defined(HOST_LINUX) || defined(HOST_SOLARIS) || defined(HOST_DARWIN)
                    ::usleep(1);                // 1 microsecond, which is a long time.
                #else
                    SleepForMilliseconds(1);    // 1 millisecond, which is a VERY long time!
                #endif

            #else

                #if 0

                // Don't wait; just waste processor cycles until we get the lock.
                // On some x86 CPU micro-architectures (e.g. Intel Core2), there is a very large penalty for doing this (should 'pause', instead; see below).

                #elif defined(_MSC_VER) && (defined(HOST_CPU_X86) || defined(HOST_CPU_X86_64))

                __asm
                {
                    pause
                } // __asm

                #elif defined(__GNUG__) && (defined(HOST_CPU_X86) || defined(HOST_CPU_X86_64))

                __asm__  __volatile__
                (
                    "pause"
                    :  // output
                    :  // input
                    :  // clobbered registers
                );  // __asm__  __volatile__

                #else

                // Don't wait; just waste processor cycles until we get the lock.
                // Damn the torpedoes, full speed ahead!

                #endif

            #endif
        }

    #else

        #error  "Spin locks are not supported for this compiler on this platform."

    #endif
    }
    //----------------------------------------------------------------------------
    inline
    void     xRelease ()
    {
    #if   defined(SPINLOCK_IS_WRAPPER)

        m_pMutex->release();

    #elif defined(_MSC_VER)

        #if (defined(HOST_CPU_X86) || defined(HOST_CPU_X86_64))

            m_AmLocked = 0;                 // volatile.  x86 SMP architecture maintains write-release semantics, which is good enough for us.

        #else

            xTestAndSet(&m_AmLocked, 0);    // volatile.  Locks memory bus, synchronizes processors.  Very slow.

        #endif

    #elif defined(__GNUG__)

        #if (defined(HOST_CPU_X86) || defined(HOST_CPU_X86_64))

            m_AmLocked = 0;                 // volatile.  x86 SMP architecture maintains write-release semantics, which is good enough for us.

        #elif defined(GNUC_ATOMIC_MEMORY_ACCESS_API)

            ::__sync_lock_release(&m_AmLocked);

        #else

            xTestAndSet(&m_AmLocked, 0);    // volatile.  Locks memory bus, synchronizes processors.  Very slow.

        #endif

    #elif defined(SPINLOCK_ENABLE_OPENMP)

        #if (defined(HOST_CPU_X86) || defined(HOST_CPU_X86_64))

            m_AmLocked = 0;                 // volatile.  x86 SMP architecture maintains write-release semantics, which is good enough for us.

        #else

            xTestAndSet(&m_AmLocked, 0);    // volatile.  Locks memory bus, synchronizes processors.  Very slow.

        #endif

    #else

        #error  "Spin locks are not supported for this compiler on this platform."

    #endif
    }

    //----------------------------------------------------------------------------
    static
    void    SleepForSeconds (unsigned long const _Seconds =0UL)
    {
    #if   defined(HOST_POSIX) || defined(HOST_SOLARIS) || defined(HOST_DARWIN) || defined(__APPLE__)

        // surrender time slice
        ::sleep(_Seconds);

    #elif defined(HOST_WIN32) || defined(HOST_WIN64)

        // surrender time slice
        ::Sleep(_Seconds * 1000 /* milliseconds/second */);

    #else // all other platforms

        #error "SpinLock::SleepForSeconds() is not presently supported on this platform.  It is supported on HOST_POSIX, HOST_SOLARIS, HOST_DARWIN, HOST_WIN32 and HOST_WIN64."

    #endif
    }

    //----------------------------------------------------------------------------
    static
    void    SleepForMilliseconds (unsigned long const _Milliseconds =0UL)
    {
    #if   defined(HOST_POSIX) || defined(HOST_LINUX) || defined(HOST_SOLARIS) || defined(HOST_DARWIN) || defined(__APPLE__)

        // surrender time slice
        ::usleep(_Milliseconds * 1000 /* microseconds/millisecond */);

    #elif defined(HOST_WIN32) || defined(HOST_WIN64)

        // surrender time slice
        ::Sleep(_Milliseconds);

    #else // all other platforms

        #error "SpinLock::SleepForMilliseconds() is not presently supported on this platform.  It is supported on HOST_POSIX, HOST_SOLARIS, HOST_DARWIN, HOST_WIN32 and HOST_WIN64."

    #endif
    }

    //----------------------------------------------------------------------------
    // </in-line interface (experimental)>
    //----------------------------------------------------------------------------

private:

    //----------------------------------------------------------------------------
    #if   defined(__GNUG__) && !(defined(HOST_CPU_X86) || defined(HOST_CPU_X86_64))     // e.g. g++ on SPARC or Itanium

        typedef ::_Atomic_word      Atomic_Word_type;   // presently an 'int', chosen to match ::__gnu_cxx::__exchange_and_add()'s parameter

    #elif defined(_MSC_VER) && !(defined(HOST_CPU_X86) || defined(HOST_CPU_X86_64))     // e.g. Itanium

        typedef long                Atomic_Word_type;   // chosen to match ::InterlockedExchange()'s parameter

    #else

        typedef int                 Atomic_Word_type;   // on x86, 'int' has 32 bits, regardless of CPU & OS choices

    #endif

    //----------------------------------------------------------------------------

    #if   defined(SPINLOCK_IS_WRAPPER)

        I_Mutex*                    m_pMutex;

    #elif defined(_MSC_VER) || defined(__GNUG__) || defined(SPINLOCK_ENABLE_OPENMP)

        volatile Atomic_Word_type   m_AmLocked;
        long                        m_NumSheep;

    #else

        #error  "Spin locks are not supported for this compiler on this platform."

    #endif

    //----------------------------------------------------------------------------
    #if   !defined(SPINLOCK_IS_WRAPPER)

    inline static
    Atomic_Word_type xTestAndSet (volatile Atomic_Word_type * const _pSharedFlag, const Atomic_Word_type _SetValue =1)  // _SetValue should be either 0 or 1.
    {
    #if defined(_MSC_VER)

        #if (defined(HOST_CPU_X86) || defined(HOST_CPU_X86_64))   // !!! needs to be tested on 64-bit Windoze

            __asm
            {
                mov         eax,    _SetValue
                mov         edx,    _pSharedFlag
                lock xchg   eax,    DWORD PTR [edx] ;   // test & set atomically (locking the bus, and enforcing a full memory barrier)
            } // __asm
            // returns 'int' in register eax

        #else   // Itanium !?

            return ::InterlockedExchange(_pSharedFlag, _SetValue);

        #endif


    #elif defined(__GNUG__)

        #if defined(HOST_CPU_X86) || defined(HOST_CPU_X86_64)

            register Atomic_Word_type Flag;         // will always be altered before being read
            __asm__  __volatile__
            (
                "movl  %1,    %%eax"    "\n\t"
                "lock"                  "\n\t"      // lock prefix forces a full memory barrier
                "xchgl %%eax, (%2)"     "\n\t"
                "movl  %%eax,  %0"
                :  // output
                "=r" (Flag)                 // %0
                :  // input
                "g" (_SetValue),            // %1
                "r" (_pSharedFlag)          // %2
                :  // clobbered registers
                "%eax"
            );  // __asm__  __volatile__
            return Flag;

        #elif defined(APPLE_ATOMIC_MEMORY_ACCESS_API)
        #elif defined(GNUC_ATOMIC_MEMORY_ACCESS_API)

            return ::__sync_lock_test_and_set(_pSharedFlag, _SetValue);

        #else

            // Note: on some platforms (e.g. Darwin/MacOSX) the ::__gnu_cxx::__exchange_and_add() function uses platform locks (possibly pthreads), so in those cases
            // performance here is actually a little worse than if we #define'd SPINLOCK_IS_WRAPPER (assuming that we actually support normal locks on this platform).
            // On other platforms (e.g. Linux, Solaris, Windows) the ::__gnu_cxx::__exchange_and_add() routines use atomic-access machine instructions,
            // and so performance here is much better than with SPINLOCK_IS_WRAPPER.
            //
            // This is not actually an implementation of test-and-set; it is essentially a nesting-depth test:
            if (_SetValue)
            {
                Atomic_Word_type const OldValue = ::__gnu_cxx::__exchange_and_add(_pSharedFlag, 1); // "push" 1 level
                if (OldValue)                                                                       // already locked?
                {
                    ::__gnu_cxx::__exchange_and_add(_pSharedFlag, -1);                              // ... cancel
                }
                return OldValue;                                            // may be 0, 1, or >1 -- caller must treat 1 and >1 as the same
            }
            else
            {
                return ::__gnu_cxx::__exchange_and_add(_pSharedFlag, -1);   // "pop" 1 level; does not reset all levels (i.e. cannot directly set the flag to zero).  Returned value is meaningless (& not used).
            }

        #endif


    #elif defined(SPINLOCK_ENABLE_OPENMP)

        Atomic_Word_type register Flag;

        #pragma omp critical    // probably implemented via platform locks; SPINLOCK_IS_WRAPPER is a better choice.
            {
                Flag = *_pSharedFlag;
                *_pSharedFlag = _SetValue;
            }
        return Flag;

    #else   // other compilers, other platforms

        // We don't expect to actually call this function in this configuration.
        throw Utility::runtime_exception("Threading::SpinLock::xTestAndSet() is not implemented for this compiler on this platform.");

    #endif  // compilers, platforms

    }

    #endif  // ! SPINLOCK_IS_WRAPPER

};  // class SpinLock

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Threading
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_THREADING_SPIN_LOCK_HPP_INCLUDED
