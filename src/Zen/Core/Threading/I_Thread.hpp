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
#ifndef ZEN_THREADING_I_THREAD_HPP_INCLUDED
#define ZEN_THREADING_I_THREAD_HPP_INCLUDED

#include "Configuration.hpp"

#include <boost/noncopyable.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Threading {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class THREADING_DLL_LINK I_Thread
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    struct ThreadId;
    /// @}

    /// @name I_Thread interface
    /// @{
public:
	/// Start this thread
	virtual void start() = 0;

	/// Stop this thread, or at least request it to stop because it is not
	/// guaranteed to actually stop.
	virtual void stop() = 0;

	/// Block until this I_Thread has terminated.
	virtual void join() = 0;

    /// Returns a copy of running (kernel-)thread's id, rendered in this 
    /// object's id format (so that you can safely compare it to 
    /// getThreadId()).  Always valid.
    static ThreadId getCurrentThreadId();

    /// Returns a copy of this object's id. Not valid until after start().
    virtual const ThreadId& getThreadId() const = 0; 
    /// @}

    /// @name Static Methods
    /// @{
public:
    static void sleepForMilliseconds(unsigned const _milliseconds); // current thread will sleep
    /// @}

    /// @name Inner Classes
    /// @{
public:
    struct ThreadId
    {
        /// @name Inner Interfaces
        /// @{
    public:
        class I_NativeThreadId
        {
            /// @name Types
            /// @{
        public:
            /// @}

            /// @name I_NativeThreadId interface
            /// @{
        public:
            virtual bool operator==(const I_NativeThreadId& _otherId) const = 0;
            virtual bool operator!=(const I_NativeThreadId& _otherId) const = 0;
            virtual bool operator<(const I_NativeThreadId& _otherId) const = 0;
            virtual I_NativeThreadId* clone() const = 0;
            virtual std::string toString() const = 0;
            /// @}

            /// @name 'Structors
            /// @{
        public:
                     I_NativeThreadId() {}
            virtual ~I_NativeThreadId() {}
            /// @}

        };  // class I_NativeThreadId
        /// @}

        /// @name ThreadId implementation
        /// @{
    public:
        ThreadId& operator=  (const ThreadId& _otherId)       {delete m_pNativeThreadId; m_pNativeThreadId = _otherId.m_pNativeThreadId->clone(); return *this;}
        bool      operator== (const ThreadId& _otherId) const {return (m_pNativeThreadId == _otherId.m_pNativeThreadId) || ( m_pNativeThreadId &&  _otherId.m_pNativeThreadId && (*m_pNativeThreadId == *_otherId.m_pNativeThreadId));}
        bool      operator!= (const ThreadId& _otherId) const {return (m_pNativeThreadId != _otherId.m_pNativeThreadId) && (!m_pNativeThreadId || !_otherId.m_pNativeThreadId || (*m_pNativeThreadId != *_otherId.m_pNativeThreadId));}
        bool      operator<  (const ThreadId& _otherId) const {return (m_pNativeThreadId <  _otherId.m_pNativeThreadId); }
                  operator std::string ()               const {return m_pNativeThreadId ? m_pNativeThreadId->toString() : "";}
        std::string toString()                          const {return m_pNativeThreadId ? m_pNativeThreadId->toString() : "";}
        /// @}

        /// @name 'Structors
        /// @{
    public:
         ThreadId(I_NativeThreadId* const _pId = NULL) : m_pNativeThreadId(_pId)                                {}
         ThreadId(const ThreadId& _otherId)            : m_pNativeThreadId(_otherId.m_pNativeThreadId->clone()) {}
        ~ThreadId()                                                                                             {delete m_pNativeThreadId;}
        /// @}

        /// @name Member variables
        /// @{
    public:
        I_NativeThreadId* m_pNativeThreadId;
        /// @}

    };  // struct ThreadId
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_Thread();
public:
    virtual ~I_Thread();
    /// @}

};	// interface I_Thread

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Threading
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_THREADING_I_THREAD_HPP_INCLUDED
