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
#ifndef ZEN_THREADING_CRITICAL_SECTION_HPP_INCLUDED
#define ZEN_THREADING_CRITICAL_SECTION_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Threading/I_Mutex.hpp>
#include <Zen/Core/Threading/SpinLock.hpp>

#include <boost/noncopyable.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Threading {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~


class CriticalSection
:	public ::boost::noncopyable
{
    /// @name Types
    /// @{
public:
    /// @}

	/// @name Implementation
	/// @{
public:
    void suspend();
    void resume();
	/// @}

    /// @name 'Structors
    /// @{
public:
    CriticalSection(I_Mutex& _Mutex);
    CriticalSection(I_Mutex* _pMutex);
    /// Constructor that uses requires_synchronization trait
    CriticalSection(I_Mutex& _Mutex, const boost::true_type&);
    /// Constructor that uses requires_synchronization trait
    CriticalSection(I_Mutex* _pMutex, const boost::true_type&);
    /// Constructor that uses requires_synchronization trait
    CriticalSection(I_Mutex& _Mutex, const boost::false_type&);
    /// Constructor that uses requires_synchronization trait
    CriticalSection(I_Mutex* _pMutex, const boost::false_type&);

    virtual ~CriticalSection();
    /// @}

	/// @name Member variables
	/// @{
private:
    I_Mutex*		m_pMutex;

    friend class CriticalSection_ExemptionZone;
	/// @}

};	// class CriticalSection

class THREADING_DLL_LINK CriticalSection_ExemptionZone
:	public ::boost::noncopyable
{
    /// @name Types
    /// @{
public:
    /// @}

	/// @name Implementation
	/// @{
public:
	/// @}

    /// @name 'Structors
    /// @{
public:
             CriticalSection_ExemptionZone(CriticalSection& _enclosingCritSec);
    virtual ~CriticalSection_ExemptionZone();
    /// @}

	/// @name Member variables
	/// @{
private:
    CriticalSection&    m_enclosingCritSec;
	/// @}

};	// class CriticalSection_ExemptionZone

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class xCriticalSection
:   public ::boost::noncopyable
{
    /// @name 'Structors
    /// @{
public:
    xCriticalSection(SpinLock& _Mutex);
    xCriticalSection(SpinLock* const _pMutex);
    xCriticalSection(SpinLock& _Mutex, const boost::true_type&);
    xCriticalSection(SpinLock* const _pMutex, const boost::true_type&);
    xCriticalSection(SpinLock& _Mutex, const boost::false_type&);
    xCriticalSection(SpinLock* const _pMutex, const boost::false_type&);

    virtual ~xCriticalSection();
    /// @}

    /// @name Member Variables
    /// @{
private:

    SpinLock* const m_pMutex;

    friend class xCriticalSection_ExemptionZone;
    /// @}

};  // class xCriticalSection

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
CriticalSection::CriticalSection(I_Mutex& _Mutex)
:   m_pMutex(&_Mutex)
{
    m_pMutex->acquire();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
CriticalSection::CriticalSection(I_Mutex* _pMutex)
:   m_pMutex(_pMutex)
{
    if (m_pMutex) m_pMutex->acquire();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
CriticalSection::CriticalSection(I_Mutex& _Mutex, const boost::true_type&)
:   m_pMutex(&_Mutex)
{
    m_pMutex->acquire();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
CriticalSection::CriticalSection(I_Mutex* _pMutex, const boost::true_type&)
:   m_pMutex(_pMutex)
{
    if (m_pMutex) m_pMutex->acquire();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
CriticalSection::CriticalSection(I_Mutex& _Mutex, const boost::false_type&)
:   m_pMutex(NULL)
{
    // Not required to do anything
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
CriticalSection::CriticalSection(I_Mutex* _pMutex, const boost::false_type&)
:   m_pMutex(NULL)
{
    // Not required to do anything
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
CriticalSection::~CriticalSection()
{
    if (m_pMutex) m_pMutex->release();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
void
CriticalSection::suspend()
{
    if (m_pMutex) m_pMutex->suspend();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
void
CriticalSection::resume()
{
    if (m_pMutex) m_pMutex->resume();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
CriticalSection_ExemptionZone::CriticalSection_ExemptionZone(CriticalSection& _enclosingCritSec)
:   m_enclosingCritSec(_enclosingCritSec)
{
    // suspend the original critical section
    m_enclosingCritSec.suspend();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
CriticalSection_ExemptionZone::~CriticalSection_ExemptionZone()
{
    // resume the original critical section
    m_enclosingCritSec.resume();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
xCriticalSection::xCriticalSection(SpinLock& _Mutex)
:   m_pMutex(&_Mutex)
{
    m_pMutex->xAcquire();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
xCriticalSection::xCriticalSection(SpinLock* const _pMutex)
:   m_pMutex(_pMutex)
{
    if (m_pMutex) m_pMutex->xAcquire();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
xCriticalSection::xCriticalSection(SpinLock& _Mutex, const boost::true_type&)
:   m_pMutex(&_Mutex)
{
    m_pMutex->xAcquire();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
xCriticalSection::xCriticalSection(SpinLock* const _pMutex, const boost::true_type&)
:   m_pMutex(_pMutex)
{
    if (m_pMutex) m_pMutex->xAcquire();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
xCriticalSection::xCriticalSection(SpinLock& _Mutex, const boost::false_type&)
:   m_pMutex(&_Mutex)
{
    // Nothing to do
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
xCriticalSection::xCriticalSection(SpinLock* const _pMutex, const boost::false_type&)
:   m_pMutex(_pMutex)
{
    // Nothing to do
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
xCriticalSection::~xCriticalSection()
{
    if (m_pMutex) m_pMutex->xRelease();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Threading
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_THREADING_I_MUTEX_HPP_INCLUDED
