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
//  Inspired and derived from the original works of:
//  John S. Givler, Ph.D.(Computer Science) - Dr.John.S.Givler@Givler.com
//
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_THREADING_MUTEX_SOLARIS_HPP_INCLUDED
#define ZEN_THREADING_MUTEX_SOLARIS_HPP_INCLUDED

#ifdef THREADMODEL_SOLARIS

#include "../I_Mutex.hpp"

#include <synch.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Threading {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class Mutex_solaris
:	public I_Mutex
{
    friend class Condition_solaris;  // pthreads condition variables require pthreads mutexes

    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_Mutex implementation
    /// @{
public:
	virtual void acquire();
	virtual void release();
    /// @}

    /// @name 'Structors
    /// @{
public:
             Mutex_solaris();
    virtual ~Mutex_solaris();
    /// @}

	/// @name Accessor functions
	/// @{
protected:
	inline mutex_t& 
	getNativeMutex()				
	{ 
		return m_mutex; 
	}

	inline const mutex_t& 
	getNativeMutex() const	
	{ 
		return m_mutex; 
	}
	/// @}

	/// @name Member variables
	/// @{
private:
	mutex_t     m_mutex;
	/// @}


};	// class Mutex_solaris

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Threading
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // THREADMODEL_SOLARIS

#endif // ZEN_THREADING_MUTEX_SOLARIS_HPP_INCLUDED
