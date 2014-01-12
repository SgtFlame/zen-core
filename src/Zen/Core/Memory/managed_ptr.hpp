//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Core Framework
//
// Copyright (C) 2006 - 2007 John Givler
// Copyright (C) 2006 - 2011 Tony Richards
// Copyright (C) 2008 - 2011 Matthew Alan Gray
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
//  20080614 TR -   Original implementation based heavily on Dr. G's
//                  SynchronizedSharedPtr<>.
//  20080709 TR -   Separated some of the managed_ptr out into this class so
//                  that it can be more readily manipulated.
//  20110204 MG -   Added implementations for serialize() methods and added
//                  the is_serializable<> trait for boost::serialization support
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#ifndef ZEN_MEMORY_MANAGED_PTR_HPP_INCLUDED
#define ZEN_MEMORY_MANAGED_PTR_HPP_INCLUDED

#include <Zen/Core/Memory/managed_payload.hpp>

#include <boost/type_traits.hpp>
#include <boost/static_assert.hpp>

#include <stddef.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace boost {
namespace archive {
    class polymorphic_iarchive;
    class polymorphic_oarchive;
} // namespace archive
} // namespace boost
namespace Zen {
namespace Memory {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
class managed_self_ref;

/// Managed pointer.
/// The managed_ptr is a smart pointer template.
/// This template suports the is_managed_by_factory trait as well
/// as the requires_synchronization trait.
/// If you are creating a smart pointer for a type that has a
/// requires_synchronization == false and a is_managed_by_factory == false
/// then you might as well use a boost::shared_ptr as it is slightly faster.
/// If you require a smart pointer for a type where either of these two
/// traits are true then this is the smart pointer you should use.
template<typename element_type = void>
class managed_ptr
{
    /// @name Types
    /// @{
public:
    /// Use the experimental critical section that uses SpinLock
    typedef Threading::xCriticalSection                                 CriticalSection;
    typedef managed_payload<element_type>                               payload_type;
    typedef typename payload_type::destroy_method_type                  destroy_method_type;
    typedef element_type*                                               pRawPointer_type;
    typedef managed_weak_ptr<element_type>                              weak_ptr_type;
	/// @}

    /// @name managed_ptr implementation
    /// @{
public:
    /// Reset the raw pointer to NULL
    void reset();

    /// Reset the raw pointer to another value
    /// The reference count of the original pointer is decremented
    /// and the object is destroyed if the reference count reaches
    /// zero.
    /// @note This is not valid for factory managed types
    void reset(element_type* const _pElement);

    /// Get the raw object pointer
    element_type* get() const;

    /// Do a dynamic cast of the managed pointer
    template<typename another_element_type>
    another_element_type as();

    /// Convert to a weak pointer
    managed_weak_ptr<element_type> getWeak() const;

    /// Get the use count of the object.
    /// The value returned should be considered volatile
    unsigned long getUseCount() const;

    /// Check to see if this is the only reference to the object.
    /// The value returned should be considered volatile
    bool isUnique() const;

    /// Check to see if this is a valid reference.
    /// The value returned should be considered volatile
    bool isValid() const;

    void serialize(boost::archive::polymorphic_iarchive& _archive, const int _version);
    void serialize(boost::archive::polymorphic_oarchive& _archive, const int _version);
    /// @}

    /// @name Operators
    /// @{
public:
    /// Reference another object, correctly managing
    /// reference counts to the old object and the new object.
    /// If the old object reference count reaches zero then the
    /// object is destroyed.
    managed_ptr& operator=(const managed_ptr<element_type>& _right);

    /// Get the raw pointer to the object
    element_type& operator*() const;

    /// Access the object
    /// This operator makes the pointer behave much like a raw pointer.
    element_type* operator->() const;

    /// Returns true if this managed pointer is mananging an object,
    /// or returns false if managing a NULL pointer.
    operator bool() const;

    /// Check to see if the underlying raw pointer is the same
    /// as the specified pointer.
    bool operator==(const managed_ptr<element_type>& _right) const;

    /// Check to see if the underlying raw pointer is not the same
    /// as the specified pointer.
    bool operator!=(const managed_ptr<element_type>& _right) const;

    /// Check to see if the underlying raw pointer is less than
    /// the specified pointer.  This is useful for allowing
    /// this managed pointer to be used in stl containers.
    bool operator<(const managed_ptr<element_type>& _right) const;
    /// @}

    /// @name 'Structors
    /// @{
public:
             /// Default constructor
             managed_ptr();

    /// Construct a managed pointer for a non factory managed object
    explicit managed_ptr(element_type* const _pElement);

    /// Construct a managed pointer for a factory managed object
    explicit managed_ptr(element_type* const _pElement, destroy_method_type _destroyMethod);

    /// Construct a managed pointer from a weak pointer
    explicit managed_ptr(const managed_weak_ptr<element_type>& _right);

             /// Copy a managed pointer
             managed_ptr(const managed_ptr& _right);

             /// Copy a managed pointer of a different type
             template<typename another_element_type>
             managed_ptr(const managed_ptr<another_element_type>& _right);
public:

             /// Destroy a managed pointer
            ~managed_ptr();
    /// @}

    /// @name Member Variables
    /// @{
private:
    template<class friend_element_type> friend class managed_ptr;
    template<class friend_element_type> friend class managed_weak_ptr;

	payload_type        m_payload;
    /// @}

};  // template managed_ptr

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
managed_ptr<element_type>::managed_ptr()
:   m_payload()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<class element_type, class self_ref_type>
void
inline
setSelfReference(managed_ptr<element_type>* _pSelf, managed_self_ref<self_ref_type>* _pRawSelf)
{
    if (_pRawSelf != NULL)
    {
        _pRawSelf->setSelfReference(managed_ptr<self_ref_type>(*_pSelf).getWeak());
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
inline
setSelfReference(...)
{
    // Do nothing if it didn't match the previous signature
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
managed_ptr<element_type>::managed_ptr(element_type* const _pElement)
:   m_payload(managed_payload<element_type>(_pElement))
{
    // If you get an error here, it's because you're trying to create a managed pointer
    // of a type that is managed by a factory without giving the factory destroy method.
    BOOST_STATIC_ASSERT(!(is_managed_by_factory<element_type>::value));

    setSelfReference(this, get());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
managed_ptr<element_type>::managed_ptr(element_type* const _pElement, destroy_method_type _destroyMethod)
:   m_payload(managed_payload<element_type>(_pElement, _destroyMethod))
{
    // If you get an error here, it's because you're trying to create a managed pointer
    // of a type that is not managed by a factory, but you're attempting to provide a
    // destroy method.
    BOOST_STATIC_ASSERT((is_managed_by_factory<element_type>::value));

    setSelfReference(this, get());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
managed_ptr<element_type>::managed_ptr(const managed_ptr& _right)
:   m_payload(_right.m_payload)
{
    m_payload.incrementUseCount();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
template<typename another_element_type>
inline
managed_ptr<element_type>::managed_ptr(const managed_ptr<another_element_type>& _right)
:   m_payload(_right.m_payload)
{
    m_payload.incrementUseCount();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
managed_ptr<element_type>::managed_ptr(const managed_weak_ptr<element_type>& _right)
:   m_payload(_right.m_payload)
{
    m_payload.incrementUseCountIfNonZero();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
managed_ptr<element_type>::~managed_ptr()
{
    payload_type payload(m_payload.decrementUseCount());

    payload.destroy(is_managed_by_factory<element_type>());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
managed_ptr<element_type>&
managed_ptr<element_type>::operator=(const managed_ptr<element_type>& _right)
{
    if (*this != _right)
    {
        payload_type payload(m_payload.decrementUseCount());
        if (payload)
        {
            payload.destroy(is_managed_by_factory<element_type>());
        }
        {
            // !!! Note that the results are undefined if two threads simultaneously
            // assign different pointers to *this.
            // Also, note that SpinLocks are not nestable so we can't acquire both
            // OldLock and NewLock concurrently since they are usually the same lock.
            m_payload = _right.m_payload;
            m_payload.incrementUseCount();

            setSelfReference(this, get());
        }
    }
    return *this;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
void
managed_ptr<element_type>::reset()
{
    payload_type payload(m_payload.decrementUseCount());

    m_payload.reset(NULL);

    payload.destroy(is_managed_by_factory<element_type>());

    setSelfReference(this, get());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
void
managed_ptr<element_type>::reset(element_type* const _pElement)
{
    /// You cannot use this method if the type is managed by a factory
    BOOST_STATIC_ASSERT(!(is_managed_by_factory<element_type>::value));

    if (managed_ptr::m_payload.m_pElement != _pElement)
    {
        payload_type payload(m_payload.decrementUseCount());

        m_payload.reset(_pElement);

        payload.destroy(is_managed_by_factory<element_type>());

        setSelfReference(this, get());
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
element_type&
managed_ptr<element_type>::operator*() const
{
    return *(m_payload.get());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
element_type*
managed_ptr<element_type>::operator->() const
{
    return m_payload.get();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
element_type*
managed_ptr<element_type>::get() const
{
    return m_payload.get();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
template<typename another_element_type>
inline
another_element_type
managed_ptr<element_type>::as()
{
    // TODO Make this work
#if 0
    /// You cannot use this method if the pointer management style
    /// between element_type and another_element_type does not match
    BOOST_STATIC_ASSERT_BOOL_CAST((is_managed_by_factory<element_type>::value)
        == (is_managed_by_factory<another_element_type>::value));
#endif 

    // Construct a pointer of the new type and return it.
    another_element_type newPointer(*this);
    return newPointer;
}


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
managed_weak_ptr<element_type>
managed_ptr<element_type>::getWeak() const
{
    return managed_weak_ptr<element_type>(*this);
}


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
unsigned long
managed_ptr<element_type>::getUseCount() const
{
    return m_payload.getUseCount();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
bool
managed_ptr<element_type>::isUnique() const
{
    return m_payload.isUnique();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
bool
managed_ptr<element_type>::isValid() const
{
    return m_payload.isValid();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
void
managed_ptr<element_type>::serialize(boost::archive::polymorphic_iarchive& _archive, const int _version)
{
    /// You cannot use this method if the type is not serializable
    BOOST_STATIC_ASSERT(is_serializable<element_type>::value);

    m_payload.serialize(_archive, _version, is_managed_by_factory<element_type>());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
void
managed_ptr<element_type>::serialize(boost::archive::polymorphic_oarchive& _archive, const int _version)
{
    /// You cannot use this method if the type is not serializable
    BOOST_STATIC_ASSERT(is_serializable<element_type>::value);

    m_payload.serialize(_archive, _version, is_managed_by_factory<element_type>());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
managed_ptr<element_type>::operator bool() const
{
    return m_payload.get() != NULL;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
bool
managed_ptr<element_type>::operator==(const managed_ptr<element_type>& _right) const
{
    return m_payload.get() == _right.m_payload.get();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
bool
managed_ptr<element_type>::operator!=(const managed_ptr<element_type>& _right) const
{
    return m_payload.get() != _right.m_payload.get();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
bool
managed_ptr<element_type>::operator<(const managed_ptr<element_type>& _right) const
{
    return m_payload.get() < _right.m_payload.get();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_MEMORY_MANAGED_PTR_HPP_INCLUDED
