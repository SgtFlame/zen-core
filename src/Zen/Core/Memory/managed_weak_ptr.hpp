//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Core Framework
//
// Copyright (C) 2006 - 2008 Tony Richards
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
//  Tony Richards                           - trichards@indiezen.com
//
//  20080709 TR -	Separated some of the managed_ptr out into this class so
//                  that it can be more readily manipulated.
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_MEMORY_MANAGED_WEAK_PTR_HPP_INCLUDED
#define ZEN_MEMORY_MANAGED_WEAK_PTR_HPP_INCLUDED

#include <Zen/Core/Memory/managed_ptr.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Memory {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/// Managed weak pointer
template<typename element_type>
class managed_weak_ptr
{
    /// @name Types
    /// @{
public:
    typedef Threading::xCriticalSection                                 CriticalSection;
    typedef managed_payload<element_type>                               payload_type;
    typedef typename payload_type::destroy_method_type                  destroy_method_type;
    /// @}

public:
    bool expired();

    /// Get the raw object pointer
    element_type* get() const;

    /// Construct a managed pointer from this weak pointer
    managed_ptr<element_type> lock() const;

    /// @name Operators
    /// @{
public:
    managed_weak_ptr& operator=(const managed_weak_ptr<element_type>& _right);

    /// Access the object
    /// This operator makes the pointer behave much like a raw pointer
    element_type* operator->() const;

    bool operator==(const managed_weak_ptr<element_type>& _right) const;
    bool operator!=(const managed_weak_ptr<element_type>& _right) const;
    bool operator<(const managed_weak_ptr<element_type>& _right) const;
    /// @}

    /// @name 'Structors
    /// @{
public:
             managed_weak_ptr();

             /// Create a weak pointer from a managed pointer
    explicit managed_weak_ptr(const managed_ptr<element_type>& _right);

             /// Create a weak pointer from a managed payload
    explicit managed_weak_ptr(const managed_payload<element_type>& _right);
    /// @}

    /// @name Member Variables
    /// @{
private:
    template<class another_element_type> friend class managed_ptr;
    template<class another_element_type> friend class managed_weak_ptr;

	payload_type        m_payload;
    /// @}

};  // template managed_weak_ptr

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
managed_weak_ptr<element_type>::managed_weak_ptr()
:   m_payload()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
managed_weak_ptr<element_type>::managed_weak_ptr(const managed_ptr<element_type>& _right)
:   m_payload(_right.m_payload)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
managed_weak_ptr<element_type>::managed_weak_ptr(const managed_payload<element_type>& _right)
:   m_payload(_right)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
managed_weak_ptr<element_type>&
managed_weak_ptr<element_type>::operator=(const managed_weak_ptr<element_type>& _right)
{
    m_payload = _right.m_payload;

    return *this;
}

template<typename element_type>
inline
element_type*
managed_weak_ptr<element_type>::get() const
{
    return m_payload.get();
}


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
managed_ptr<element_type>
managed_weak_ptr<element_type>::lock() const
{
    return managed_ptr<element_type>(*this);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
bool
managed_weak_ptr<element_type>::expired()
{
    // If this payload has expired, reset it to NULL
    if (m_payload.getUseCount() == 0L)
    {
        m_payload.reset(NULL);
        return true;
    }
    return false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
element_type*
managed_weak_ptr<element_type>::operator->() const
{
    return m_payload.get();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
bool
managed_weak_ptr<element_type>::operator==(const managed_weak_ptr<element_type>& _right) const
{
    return m_payload.get() == _right.m_payload.get();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
bool
managed_weak_ptr<element_type>::operator!=(const managed_weak_ptr<element_type>& _right) const
{
    return m_payload.get() != _right.m_payload.get();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
bool
managed_weak_ptr<element_type>::operator<(const managed_weak_ptr<element_type>& _right) const
{
    return m_payload.get() < _right.m_payload.get();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_MEMORY_MANAGED_WEAK_PTR_HPP_INCLUDED
