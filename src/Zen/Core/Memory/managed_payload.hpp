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
//  Matthew Alan Gray                       - mgray@hatboystudios.com
//
//  20080709 TR -   Separated some of the managed_ptr out into this class so
//                  that it can be more readily manipulated.
//  20110204 MG -   Added implementations for serialize() methods and added
//                  the is_serializable<> trait for boost::serialization support
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_MEMORY_MANAGED_PAYLOAD_HPP_INCLUDED
#define ZEN_MEMORY_MANAGED_PAYLOAD_HPP_INCLUDED

#include <Zen/Core/Threading/SpinLock.hpp>
#include <Zen/Core/Threading/CriticalSection.hpp>

#include <boost/function.hpp>
#include <boost/type_traits.hpp>
#include <boost/noncopyable.hpp>

#ifndef DISABLE_SYNCH_PTR_PRIVATE_ALLOC
#ifndef ENABLE_SYNCH_PTR_PRIVATE_ALLOC
#define ENABLE_SYNCH_PTR_PRIVATE_ALLOC
#endif  // ! ENABLE_SYNCH_PTR_PRIVATE_ALLOC
#endif  // ! DISABLE_SYNCH_PTR_PRIVATE_ALLOC

#ifdef ENABLE_SYNCH_PTR_PRIVATE_ALLOC
#ifndef SYNCH_PTR_PRIVATE_ALLOC_BLOCKCOUNT
#define SYNCH_PTR_PRIVATE_ALLOC_BLOCKCOUNT 8000
#endif  // ! SYNCH_PTR_PRIVATE_ALLOC_BLOCKCOUNT
#endif  // ENABLE_SYNCH_PTR_PRIVATE_ALLOC

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
template<typename element_type = void> class managed_weak_ptr;
template<typename element_type> class managed_ptr;

/// Is Managed By Factory trait
/// Defaults to false
template<typename T>
struct is_managed_by_factory : public boost::false_type{};

/// Is Serializable trait
template<typename T>
struct is_serializable : public boost::false_type{};

#ifdef ENABLE_SYNCH_PTR_PRIVATE_ALLOC
struct UseCount_type
{
    UseCount_type() : m_UseCount(0) {};
    unsigned long                   m_UseCount;
    UseCount_type*                  m_pNextAvailableCounter;

    UseCount_type& operator ++   ()                             {++ m_UseCount; return *this;}
    UseCount_type& operator --   ()                             {-- m_UseCount; return *this;}
    UseCount_type& operator =    (const long _UseCount)         {m_UseCount = _UseCount; return *this;}
    bool           operator ==   (const long _UseCount) const   {return m_UseCount == _UseCount;}
                   operator long ()                     const   {return m_UseCount;}
};
typedef UseCount_type*              pUseCount_type;
#else   // ! ENABLE_SYNCH_PTR_PRIVATE_ALLOC
typedef unsigned long*              pUseCount_type;
#endif  // ! ENABLE_SYNCH_PTR_PRIVATE_ALLOC

struct AllocationPool_type
{
    Threading::SpinLock             m_poolLock;
    #ifdef ENABLE_SYNCH_PTR_PRIVATE_ALLOC
    UseCount_type*                  m_pAvailList;
    UseCount_type*                  m_pAvailArray;  // we're not going to deallocate these arrays, until the process ends (and then the OS does the work).
    UseCount_type*                  m_pNextInArray;

    AllocationPool_type() 
    :   m_poolLock()
    ,   m_pAvailList(NULL)  
    {
#ifdef HOST_WIN32
        // Disable memory allocation tracking.
        int flags = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
        flags &= ~_CRTDBG_ALLOC_MEM_DF;
        _CrtSetDbgFlag(flags);
#endif  // HOST_WIN32

        m_pNextInArray = &(
            m_pAvailArray = 
                new UseCount_type[SYNCH_PTR_PRIVATE_ALLOC_BLOCKCOUNT]
        )[SYNCH_PTR_PRIVATE_ALLOC_BLOCKCOUNT];

#ifdef HOST_WIN32
        // Enable memory allocation tracking.
        flags = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
        flags |= _CRTDBG_ALLOC_MEM_DF;
        _CrtSetDbgFlag(flags);
#endif  // HOST_WIN32

    }
    #else   // ! ENABLE_SYNCH_PTR_PRIVATE_ALLOC
    AllocationPool_type () : m_poolLock() {}
    #endif  // ! ENABLE_SYNCH_PTR_PRIVATE_ALLOC
};

class BaseDestroyMethod
:   public boost::noncopyable
{
public:
    virtual void destroy() = 0;

             BaseDestroyMethod() {}
    virtual ~BaseDestroyMethod() {}

};  // class BaseDestroyMethod

template<typename element_type = void>
class DestroyMethod
:   public BaseDestroyMethod
{
public:
    typedef managed_weak_ptr<element_type>                          wpElement_type;
    typedef boost::function<void(wpElement_type)>                   destroy_method_type;

    virtual void destroy()
    {
        m_destroyMethod(m_pElement);
    }

             DestroyMethod(destroy_method_type _destroyMethod, wpElement_type _pElement) : m_destroyMethod(_destroyMethod), m_pElement(_pElement) {}
    virtual ~DestroyMethod() {}

private:
    destroy_method_type         m_destroyMethod;
    wpElement_type              m_pElement;

};  // class DestroyMethod

template<typename element_type = void>
class managed_payload
{
    /// @name Types
    /// @{
public:
    typedef element_type                                            Element_type;
    typedef Threading::xCriticalSection                             CriticalSection;
    typedef boost::function<void(managed_weak_ptr<element_type>)>   destroy_method_type;
    typedef BaseDestroyMethod*                                      pDestroyMethod_type;
    typedef element_type*                                           pRawPointer_type;
    /// @}

    /// @name managed_payload implementation
    /// @{
private:
    template<class friend_element_type> friend class managed_ptr;
    template<class friend_element_type> friend class managed_weak_ptr;

    /// Atomic Increment
    unsigned long incrementUseCount();

    /// Atomic Increment, but don't increment above zero.
    /// This is used when a weak pointer is being promoted to a strong
    /// pointer and is a temporary workaround until the "weak pointer list"
    /// is implemented.
    unsigned long incrementUseCountIfNonZero();

    /// Automic Decrement
    /// @return "this" if use-count reaches zero, otherwise NULL
    managed_payload* decrementUseCount();

    /// Returns true if this payload actually contains something
    operator bool() const;

    /// Destroy a factory managed object
    void destroy(const boost::true_type&);

    /// Destroy a non-factory managed object
    void destroy(const boost::false_type&);

    /// Reset the raw pointer to another value
    /// If this is a new value, allocate a new use count.
    /// @note The original pointer is leaked.  The user is responsible for 
    ///     decrementing and calling destroy if necessary.
    void reset(element_type* const _pElement);

    /// Get the raw object pointer
    element_type* get() const;

    /// Get the use count of the object.
    /// The value returned should be considered volatile
    unsigned long getUseCount() const;

    /// Check to see if this is the only reference to the object.
    /// The value returned should be considered volatile
    bool isUnique() const;

    /// Check to see if this is a valid reference.
    /// The value returned should be considered volatile
    bool isValid() const;

    void serialize(boost::archive::polymorphic_iarchive& _archive, const int _version, const boost::true_type&);
    void serialize(boost::archive::polymorphic_iarchive& _archive, const int _version, const boost::false_type&);
    void serialize(boost::archive::polymorphic_oarchive& _archive, const int _version, const boost::true_type&);
    void serialize(boost::archive::polymorphic_oarchive& _archive, const int _version, const boost::false_type&);
private:
    /// Get a new counter from the pool
    /// Use returnCounter to return the counter to the pool
    void borrowCounter();

    /// Return a counter back to the pool
    void returnCounter();

    /// Get this class's counter pool
    static AllocationPool_type* getClassPool();
    /// @}

    /// @name 'Structors
    /// @{
public:
    managed_payload();
    managed_payload(element_type* _pElement);
    managed_payload(element_type* _pElement, destroy_method_type _destroyMethod);

    /// Default copy constructor does not increment the use count.
    managed_payload(const managed_payload& _right);

    /// Copy constructor of another type does not increment the use count.
    template<typename another_element_type>
    managed_payload(const managed_payload<another_element_type>& _right);

    /// Copy constructor from an optional payload.  Does not increment the use count.
    managed_payload(const managed_payload* const _pRight);

    /// Destructor
    ~managed_payload();
    /// @}

	/// @name Member Variables
	/// These are public and are directly manipulated by managed_ptr and managed_weak_ptr (for now)
	/// @{
public:
	AllocationPool_type*                m_pPool;        //< allocation pools for our ref counter.  May differ across separately-linked or -loaded libraries.
	element_type*                       m_pElement;     //< each *pElement has a *pUseCount
	pUseCount_type                      m_pUseCount;    //< some pUseCount's have a *pUseCount and (therefore) also a *pElement
	pDestroyMethod_type                 m_pDestroyMethod;
	// Invariant: (m_pPool != NULL).
	// Invariant: (m_pElement != NULL) implies *m_pUseCount is valid and (*m_pUseCount > 0).
	// Invariant: (m_pElement != NULL and element_type has the is_managed_by_factory trait) implies m_destroyMethod is valid
	/// @}

};	// template class managed_payload

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
managed_payload<element_type>::managed_payload(const managed_payload* const _pRight)
{
    if (_pRight != NULL && _pRight->m_pElement != NULL)
    {
        m_pPool                = _pRight->m_pPool;
        m_pElement             = _pRight->m_pElement;
        m_pUseCount            = _pRight->m_pUseCount;
        m_pDestroyMethod       = _pRight->m_pDestroyMethod;
    }
    else
    {
        m_pPool          = getClassPool();
        m_pElement       = NULL;
        m_pUseCount      = NULL;
        m_pDestroyMethod = NULL;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
managed_payload<element_type>::managed_payload()
:   m_pPool(getClassPool())
,   m_pElement(NULL)
,   m_pUseCount(NULL)
,   m_pDestroyMethod(NULL)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
managed_payload<element_type>::managed_payload(element_type* _pElement)
:   m_pPool(getClassPool())
,   m_pElement(_pElement)
,   m_pUseCount(NULL)
,   m_pDestroyMethod(NULL)
{
    if (m_pElement != NULL)
    {
        borrowCounter();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
managed_payload<element_type>::managed_payload(element_type* _pElement, destroy_method_type _destroyMethod)
:   m_pPool(getClassPool())
,   m_pElement(_pElement)
,   m_pUseCount(NULL)
,   m_pDestroyMethod(NULL)
{
    if (m_pElement != NULL)
    {
        borrowCounter();
    }

#ifdef HOST_WIN32
    // Disable memory allocation tracking.
    int flags = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    flags &= ~_CRTDBG_ALLOC_MEM_DF;
    _CrtSetDbgFlag(flags);
#endif  // HOST_WIN32

    m_pDestroyMethod = new DestroyMethod<element_type>(_destroyMethod, managed_weak_ptr<typename managed_payload::Element_type>(this));

#ifdef HOST_WIN32
    // Enable memory allocation tracking.
    flags = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    flags |= _CRTDBG_ALLOC_MEM_DF;
    _CrtSetDbgFlag(flags);
#endif  // HOST_WIN32
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
managed_payload<element_type>::managed_payload(const managed_payload& _right)
:   m_pPool(_right.m_pPool)
,   m_pElement(_right.m_pElement)
,   m_pUseCount(_right.m_pUseCount)
,   m_pDestroyMethod(_right.m_pDestroyMethod)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
template<typename another_element_type>
inline
managed_payload<element_type>::managed_payload(const managed_payload<another_element_type>& _right)
:   m_pPool(_right.m_pPool)
,   m_pElement(dynamic_cast<typename managed_payload::Element_type*>(_right.m_pElement))
,   m_pUseCount(_right.m_pUseCount)
,   m_pDestroyMethod(_right.m_pDestroyMethod)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
managed_payload<element_type>::~managed_payload()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
unsigned long
managed_payload<element_type>::incrementUseCount()
{
    CriticalSection guard(m_pPool->m_poolLock, Threading::requires_synchronization<element_type>());

    if (m_pElement != NULL)
    {
        ++ *m_pUseCount;
        return *m_pUseCount;
    }

    return 0;
}


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
unsigned long
managed_payload<element_type>::incrementUseCountIfNonZero()
{
    CriticalSection guard(m_pPool->m_poolLock, Threading::requires_synchronization<element_type>());

    if (m_pElement != NULL && *m_pUseCount != 0)
    {
        ++ *m_pUseCount;
        return *m_pUseCount;
    }
    else
    {
        m_pElement = NULL;
    }

    return 0;
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
managed_payload<element_type>*
managed_payload<element_type>::decrementUseCount()
{
    if (m_pElement != NULL)
    {
        CriticalSection guard(m_pPool->m_poolLock, Threading::requires_synchronization<element_type>());

        if (*m_pUseCount == 1L)
        {
            // One and only one reference

            *m_pUseCount = 0;

            return this;
        }
        else
        {
            -- *m_pUseCount;
        }

    }

    return NULL;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
managed_payload<element_type>::operator bool() const
{
    return m_pElement != NULL;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
void
managed_payload<element_type>::destroy(const boost::true_type&)
{
    BOOST_STATIC_ASSERT( is_managed_by_factory<typename managed_payload::Element_type>::value );

    if (m_pElement != NULL)
    {
        m_pDestroyMethod->destroy();

#ifdef HOST_WIN32
            // Disable memory allocation tracking.
            int flags = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
            flags &= ~_CRTDBG_ALLOC_MEM_DF;
            _CrtSetDbgFlag(flags);
#endif  // HOST_WIN32

        delete m_pDestroyMethod;

#ifdef HOST_WIN32
            // Enable memory allocation tracking.
            flags = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
            flags |= _CRTDBG_ALLOC_MEM_DF;
            _CrtSetDbgFlag(flags);
#endif  // HOST_WIN32

        m_pDestroyMethod = NULL;

        returnCounter();

        m_pElement = NULL;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
void
managed_payload<element_type>::destroy(const boost::false_type&)
{
    BOOST_STATIC_ASSERT(!(is_managed_by_factory<typename managed_payload::Element_type>::value));

    if (m_pElement != NULL)
    {
        // If you get a "cannot access protected member" then quite likely this
        // class needs to have a is_managed_by_factory class trait.
        // If you get "deletion of pointer to incomplete type" then quite likely you
        // didn't include the class mentioned in the following error message line:
        // "see declaration of (type)"
        delete m_pElement;

        returnCounter();

        m_pElement = NULL;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
void
managed_payload<element_type>::reset(element_type* const _pElement)
{
    if ((m_pElement = _pElement) != NULL)
    {
        borrowCounter();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
element_type*
managed_payload<element_type>::get() const
{
    return m_pElement;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
unsigned long
managed_payload<element_type>::getUseCount() const
{
    return (m_pElement != NULL) ? *m_pUseCount : 0L;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
bool
managed_payload<element_type>::isUnique() const
{
    return m_pElement != NULL && (*m_pUseCount == 1L);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
bool
managed_payload<element_type>::isValid() const
{
    return m_pElement != NULL;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
void
managed_payload<element_type>::serialize(boost::archive::polymorphic_iarchive& _archive, const int _version, const boost::true_type&)
{
    if (m_pElement == NULL)
    {
        m_pElement == element_type::create();
        borrowCounter();

#ifdef HOST_WIN32
        // Disable memory allocation tracking.
        int flags = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
        flags &= ~_CRTDBG_ALLOC_MEM_DF;
        _CrtSetDbgFlag(flags);
#endif  // HOST_WIN32

        m_pDestroyMethod = new DestroyMethod<element_type>(element_type::destroy, managed_weak_ptr<typename managed_payload::Element_type>(this));

#ifdef HOST_WIN32
        // Enable memory allocation tracking.
        flags = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
        flags |= _CRTDBG_ALLOC_MEM_DF;
        _CrtSetDbgFlag(flags);
#endif  // HOST_WIN32

    }

    _archive & *m_pElement;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
void
managed_payload<element_type>::serialize(boost::archive::polymorphic_iarchive& _archive, const int _version, const boost::false_type&)
{
    if (m_pElement == NULL)
    {
        m_pElement = element_type::create();
        borrowCounter();
    }

    _archive & *m_pElement;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
void
managed_payload<element_type>::serialize(boost::archive::polymorphic_oarchive& _archive, const int _version, const boost::true_type&)
{
    if (m_pElement == NULL)
    {
        m_pElement = element_type::create();
        borrowCounter();

#ifdef HOST_WIN32
        // Disable memory allocation tracking.
        int flags = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
        flags &= ~_CRTDBG_ALLOC_MEM_DF;
        _CrtSetDbgFlag(flags);
#endif  // HOST_WIN32

        m_pDestroyMethod = new DestroyMethod<element_type>(element_type::destroy, managed_weak_ptr<typename managed_payload::Element_type>(this));

#ifdef HOST_WIN32
        // Enable memory allocation tracking.
        flags = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
        flags |= _CRTDBG_ALLOC_MEM_DF;
        _CrtSetDbgFlag(flags);
#endif  // HOST_WIN32

    }

    _archive & *m_pElement;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
void
managed_payload<element_type>::serialize(boost::archive::polymorphic_oarchive& _archive, const int _version, const boost::false_type&)
{
    if (m_pElement == NULL)
    {
        m_pElement = element_type::create();
        borrowCounter();
    }

    _archive & *m_pElement;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
void
managed_payload<element_type>::borrowCounter()
{
    #ifdef ENABLE_SYNCH_PTR_PRIVATE_ALLOC
    CriticalSection guard(m_pPool->m_poolLock, Threading::requires_synchronization<element_type>());

    if(m_pPool->m_pAvailList != NULL)
    {
        m_pUseCount = m_pPool->m_pAvailList;
        m_pPool->m_pAvailList = m_pUseCount->m_pNextAvailableCounter;
    }
    else
    {
        // Check for an underflow
        if (m_pPool->m_pNextInArray <= m_pPool->m_pAvailArray)
        {
            // Notice that we don't (can't!) deallocate the old array.
            // This isn't a "leak", we're permanently allocating the array's
            // storage.

#ifdef HOST_WIN32
            // Disable memory allocation tracking.
            int flags = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
            flags &= ~_CRTDBG_ALLOC_MEM_DF;
            _CrtSetDbgFlag(flags);
#endif  // HOST_WIN32

            m_pPool->m_pNextInArray = &(
                m_pPool->m_pAvailArray = new UseCount_type[SYNCH_PTR_PRIVATE_ALLOC_BLOCKCOUNT]
            )[SYNCH_PTR_PRIVATE_ALLOC_BLOCKCOUNT];

#ifdef HOST_WIN32
            // Enable memory allocation tracking.
            flags = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
            flags |= _CRTDBG_ALLOC_MEM_DF;
            _CrtSetDbgFlag(flags);
#endif  // HOST_WIN32

        }
        m_pUseCount = -- m_pPool->m_pNextInArray;
    }
    *m_pUseCount = 1L;

    #else

    *(m_pUseCount = new long) = 1L;

    #endif
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
void
managed_payload<element_type>::returnCounter()
{
    #ifdef ENABLE_SYNCH_PTR_PRIVATE_ALLOC
    CriticalSection guard(m_pPool->m_poolLock, Threading::requires_synchronization<element_type>());

    m_pUseCount->m_pNextAvailableCounter = m_pPool->m_pAvailList;
    m_pPool->m_pAvailList = m_pUseCount;
    *m_pUseCount = 0;
    #else

    delete m_pUseCount;

    #endif
}


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename element_type>
inline
AllocationPool_type*
managed_payload<element_type>::getClassPool()
{
    #ifdef ENABLE_SYNCH_PTR_PRIVATE_ALLOC
    // Rather than constantly churning the memory allocator by allocating & deleting 
    // long ints, we maintain a pool of already-allocated long ints.
    //  sm_Pool.m_AvailList -- previously-used reference counters, which are not 
    //                         presently active and are thus available.
    //  sm_Pool.m_InUseList -- those reference counters which are presently in use.
    //  sm_Pool.m_PoolLock  -- spin lock for protecting the integrity of pool updates.
    // We move list nodes (containing our long ints) from one list to the other, and 
    // back again.
    #endif  // ENABLE_SYNCH_PTR_PRIVATE_ALLOC
    //
    //  Note:
    //
    // It is not guaranteed that there will exist only one copy of an instantiated 
    //  template within a process.  Similarly, there may be more than one copy of 
    // any static class data, or even more than one copy of the vtable or RTTI data.
    // Dynamically loading libraries "by hand" -- e.g. using dlopen() -- rather than
    // linking to those same shared libraries, is especially likely to create more 
    // than 1 copy of the same class, static, or template data within the process.

    static AllocationPool_type  sm_Pool;
    return &sm_Pool;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_MEMORY_MANAGED_PAYLOAD_HPP_INCLUDED
