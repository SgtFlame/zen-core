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
#ifndef ZEN_THREADING_I_MUTEX_HPP_INCLUDED
#define ZEN_THREADING_I_MUTEX_HPP_INCLUDED

#include "Configuration.hpp"

#include <boost/type_traits.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Threading {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/// Requires Synchronization trait
/// True if the type requires synchronization
/// Defaults to True
template<typename T>
struct requires_synchronization : public boost::true_type{};

/// Basic Mutex interface
///
/// The underlying implementation is NOT required to support nested locking.
///
/// @note Note from Dr. G: (Nestable locks are what some ignorati mis-name 
///		"recursive locks" [sic], and other ignorati mis-name "reentrant locks" 
///		[sic].)
///
class THREADING_DLL_LINK I_Mutex
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_Mutex interface
    /// @{
public:
	/// Acquire the lock
	/// @throw std::runtime_error if an error occurred.
	virtual void acquire() = 0;

	/// Release the lock
	/// @throw std::runtime_error if an error occurred.
	virtual void release() = 0;

	virtual void suspend() { this->release(); }

	virtual void resume() { this->acquire(); }

    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_Mutex();
    virtual ~I_Mutex();
    /// @}

};	// interface I_Mutex

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Threading
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_THREADING_I_MUTEX_HPP_INCLUDED
