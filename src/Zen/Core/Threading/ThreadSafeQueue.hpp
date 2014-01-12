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
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#if 0 // deprecated
#ifndef ZEN_THREADING_THREAD_SAFE_QUEUE_HPP_INCLUDED
#define ZEN_THREADING_THREAD_SAFE_QUEUE_HPP_INCLUDED

#include "Configuration.hpp"
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Threading {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

/// Thread Safe First In First Out Queue
///
/// Usage Pattern:  
///		Multi-threaded producer, single threaded consumer
///		Used as a first in first out queue with push_back and pop_front_all.
///		pop_front_all is executed in constant time no matter how many items
///		are on the queue and only locks once.  pop_front_all does not block
///     if there are no items (as opposed to SynchronizedQueue, which blocks
///     on pop() until there are items available.
///
/// @see SynchronizedQueue Use SynchronizedQueue instead.
template<typename _collection_type>
class ThreadSafeQueue
{
    /// @name Types
    /// @{
public:
	typedef _collection_type    collection_type;
	typedef _collection_type*   collection_ptr_type;
    /// @}

	/// @name Implementation
	/// @{
public:

	/// Push a single item onto the queue.
	/// @note Thread safe
	inline
	void
	push_back(typename _collection_type::value_type _value)
	{
		CriticalSection lock(m_pGuard);
		m_pCurrent->push_back(_value);
	}

	/// Push a collection of items onto the queue.
	/// Use this if possible because it only locks once per
	///	collection as opposed to locking once for every item.
	/// @note Thread safe
	inline
	void
	push_back(typename collection_type _value)
	{
		CriticalSection lock(m_pGuard);
		m_pCurrent->push_back(_value);

        if (!
	}

	/// Pop everything from the front of the queue.
	/// @note This call is not thread safe (can only be called by a single thread)
	/// but the interaction between push_back and pop_front_all() are thread safe
	/// and is designed to be used by two or more threads. (i.e. one thread calling
	/// pop_front_all, one or more threads calling push_back.)
	inline
	collection_ptr_type
	pop_front_all()
	{
		collection_ptr_type const pReturn = m_pCurrent;

		// Only the swap and the clear need to be guarded
		{
			CriticalSection lock(m_pGuard);
			m_pCurrent = m_pNotCurrent;
			m_pNotCurrent = pReturn;

			// Clear the now current collection
			m_pCurrent->clear();
		}

		return pReturn;
	}
	/// @}

    /// @name 'Structors
    /// @{
public:
	inline
	ThreadSafeQueue()
	{
		m_pCurrent      = &m_collection1;
		m_pNotCurrent   = &m_collection2;
	}

	inline
	~ThreadSafeQueue()
	{
	}
    /// @}

	/// @name Member Variables
	/// @{ 
private:

	/// Instantiated collections
	collection_type						m_collection1, m_collection2;

	/// Pointers to the two collections
	collection_ptr_type volatile        m_pCurrent;
	collection_ptr_type volatile		m_pNotCurrent;

	/// Guard (should be a spinlock but that's not implemented yet)
	I_Mutex*							m_pGuard;
	/// @}
};	// template ThreadSafeQueue

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Threading
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_THREADING_THREAD_SAFE_QUEUE_HPP_INCLUDED
#endif // deprecated
