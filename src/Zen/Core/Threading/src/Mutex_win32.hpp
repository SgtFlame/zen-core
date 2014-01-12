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
#ifndef ZEN_THREADING_MUTEX_WIN32_HPP_INCLUDED
#define ZEN_THREADING_MUTEX_WIN32_HPP_INCLUDED

#include "../I_Mutex.hpp"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Threading {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class Mutex_win32
:	public I_Mutex
{
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
             Mutex_win32();
    virtual ~Mutex_win32();
    /// @}

	/// @name Accessor functions
	/// @{
protected:
	inline HANDLE& 
	getNativeMutex()				
	{ 
		return m_handle; 
	}

	inline const HANDLE& 
	getNativeMutex() const	
	{ 
		return m_handle; 
	}
	/// @}

	/// @name Member variables
	/// @{
private:
	HANDLE	m_handle;
	/// @}

};	// class Mutex_win32

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Threading
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_THREADING_MUTEX_WIN32_HPP_INCLUDED
