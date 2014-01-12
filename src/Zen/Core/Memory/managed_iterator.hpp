//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Core Framework
//
// Copyright (C) 2006 - 2007 John Givler
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
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_MEMORY_MANAGED_ITERATOR_HPP_INCLUDED
#define ZEN_MEMORY_MANAGED_ITERATOR_HPP_INCLUDED

#include <Zen/Core/Memory/managed_ptr.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Memory {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/// Managed Iterator
/// A smart pointer to an iterator and a container
template <typename iterator_type, typename container_type>
class managed_iterator
{
    /// @name Types
    /// @{
public:
    typedef managed_iterator<iterator_type, container_type> self;
    typedef typename iterator_type::element_type            element_type;

    typedef boost::function<void(iterator_type*)>           destroy_iterator_method_type;
    typedef boost::function<void(container_type*)>          destroy_container_method_type;
    /// @}

    /// @name managed_iterator implementation
    /// @{
public:
    bool hasNext();
    element_type get();
    /// @}

    /// @name Operators
    /// @{
public:
    /// Deep copy
    managed_iterator& operator=(const managed_iterator& _right);

    /// Post increment operator
    /// The pre-increment operator is not implemented 
    managed_iterator& operator++();
    /// @}

    /// @name 'Structors
    /// @{
public:
     managed_iterator(iterator_type* _pIterator, destroy_iterator_method_type _destroyIteratorMethod,
                      container_type* _pContainer, destroy_container_method_type _destroyContainerMethod);
     managed_iterator(const managed_iterator& _right);
    ~managed_iterator();

private:
    /// Not implemented
    managed_iterator() {}
    /// @}

    /// @name Member Variables
    /// @{
private:
    managed_ptr<iterator_type>      m_pIterator;
    managed_ptr<container_type>     m_pContainer;

    destroy_iterator_method_type    m_destroyIteratorMethod;
    destroy_container_method_type   m_destroyContainerMethod;
    /// @}

};  // template managed_iterator

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template <typename iterator_type, typename container_type>
inline
managed_iterator<iterator_type, container_type>::managed_iterator(iterator_type* _pIterator, destroy_iterator_method_type _destroyIteratorMethod,
                      container_type* _pContainer, destroy_container_method_type _destroyContainerMethod)
:   m_pIterator(_right.m_pIterator, _right.m_destroyIteratorMethod)
,   m_pContainer(_right.m_pContainer, _right.m_destroyContainerMethod)
,   m_destroyIteratorMethod(_destroyIteratorMethod)
,   m_destroyContainerMethod(_destroyContainerMethod)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template <typename iterator_type, typename container_type>
inline
managed_iterator<iterator_type, container_type>::managed_iterator(const managed_iterator& _right)
:   m_pIterator(_right.m_pIterator, _right.m_destroyIteratorMethod)
,   m_pContainer(_right.m_pContainer, _right.m_destroyContainerMethod)
,   m_destroyIteratorMethod(_right.m_destroyIteratorMethod)
,   m_destroyContainerMethod(_right.m_destroyContainerMethod)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template <typename iterator_type, typename container_type>
inline
managed_iterator<iterator_type, container_type>::~managed_iterator()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template <typename iterator_type, typename container_type>
inline
bool
managed_iterator<iterator_type, container_type>::hasNext()
{
    return m_pIterator->hasNext();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template <typename iterator_type, typename container_type>
inline
typename managed_iterator<iterator_type, container_type>::element_type
managed_iterator<iterator_type, container_type>::get()
{
    return m_pIterator->get();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template <typename iterator_type, typename container_type>
inline
typename managed_iterator<iterator_type, container_type>&
managed_iterator<iterator_type, container_type>::operator=(const managed_iterator& _right)
{
    m_pIterator = _right.m_pIterator;
    m_pContainer = _right.m_pContainer;

    m_destroyIteratorMethod  = _right.m_destroyIteratorMethod;
    m_destroyContainerMethod = _right.m_destroyContainerMethod;

    return *this;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template <typename iterator_type, typename container_type>
inline
typename managed_iterator<iterator_type, container_type>&
managed_iterator<iterator_type, container_type>::operator++()
{
    ++(*m_pIterator); 
    return *this;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~


#endif // ZEN_MEMORY_MANAGED_ITERATOR_HPP_INCLUDED
