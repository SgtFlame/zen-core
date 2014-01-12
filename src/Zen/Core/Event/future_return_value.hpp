//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Core Framework
//
// Copyright (C) 2001 - 2010 Tony Richards
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
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_EVENT_FUTURE_HPP_INCLUDED
#define ZEN_EVENT_FUTURE_HPP_INCLUDED

#include <Zen/Core/Event/Event.hpp>

#include <Zen/Core/Threading/I_Condition.hpp>
#include <Zen/Core/Threading/ConditionFactory.hpp>

#include <boost/noncopyable.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Event {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

/// Future Return Value.
/// A "future" is a construct that allows for deferred responses.
/// This class represents a return value from a call to a method
/// where the return value may or may not be immediately available.
///
/// Application developers can use getValue() to synchronously wait until the
/// return value is ready, or subscribe to an event using connect() and be
/// notified asynchronously when the value is ready.
/// 
/// @param Return_type is the return value that will be returned in the "future".
///
/// @see http://www.torquepowered.com/community/blogs/view/14899/1#comment-96568
///     for a good discussion about futures and promises.
template<class Return_type>
class future_return_value
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef Threading::I_Condition*                         pCondition_type;
    typedef Threading::I_Mutex*                             pMutex_type;
    typedef Event<Return_type>                              Event_type;
    typedef typename Event_type::pConnection_type           pConnection_type;
    typedef typename Event_type::delegate_type              delegate_type;
    /// @}

    /// @name future_return_value implementation
    /// @{
public:
    /// Set the return value.  This should only be called by the method that
    /// created the future_return_value.
    void setValue(Return_type _value);

    /// Get the return value.  This method waits until the value is available.
    /// This is a synchronous method.  
    /// @see If you prefer an asynchronous method
    ///     where you specify a method that gets called when the return value is
    ///     ready, see future_return_value<>::connect().
    Return_type getValue();

    /// Used to typecast this future_return_value to the Return_type type.
    operator Return_type();

    /// Used to overload the -> operator.
    /// This method assumes the underlying Return_type is a smart pointer.
    Return_type operator->();

    /// Connect a listener for asynchronous notification that the return
    /// value is ready to be used.  This works much like the Event::connect()
    /// method, except it is guaranteed to fire once and only once for each
    /// listener, even if connect() is called after the event is fired.
    /// @see If you prefer a synchronos method for getting the return value,
    ///     see future_return_value<>::getValue().
    pConnection_type connect(delegate_type _listener);
private:
    /// @}

    /// @name 'Structors
    /// @{
public:
             future_return_value<Return_type>();
    virtual ~future_return_value<Return_type>();
    /// @}

    /// @name Member Variables
    /// @{
private:
    /// True when the return value is ready.
    bool                        m_valueIsReady;

    /// Guards the combination of the m_valueIsReady and subscribing to events.
    pMutex_type                 m_pMutex;

    /// Asserted when the return value is ready.
    pCondition_type             m_pCondition;

    /// The return value
    Return_type                 m_returnValue;

    /// An event that is used by the connect() method so that multiple
    /// delegates can be registered to receive this return value.
    Event_type                  m_event;
    /// @}

};  // template future_return_value

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<class Return_type>
inline
future_return_value<Return_type>::future_return_value()
:   m_valueIsReady(false)
,   m_pMutex(Threading::MutexFactory::create())
,   m_pCondition(Threading::ConditionFactory::create())
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<class Return_type>
inline
future_return_value<Return_type>::~future_return_value()
{
    Threading::ConditionFactory::destroy(m_pCondition);
    Threading::MutexFactory::destroy(m_pMutex);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<class Return_type>
inline
void
future_return_value<Return_type>::setValue(Return_type _value)
{
    // Guard the combination of m_valueIsReady and new event connections.
    // If we didn't do this, we could set m_valueIsReady to be true after
    // connect() has already checked it and then this method could fire
    // the event before connect() has subscribed to it.
    Threading::CriticalSection guard(m_pMutex);

    // Set the return value.
    m_returnValue = _value;

    // Indicate that the return value is ready
    m_valueIsReady = true;

    // Assert the condition to release the synchronous listeners waiting 
    // in getValue()
    m_pCondition->assertCondition();

    // Fire the event to notify all of the asynchronous listeners that 
    // the value is ready.
    m_event(m_returnValue);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<class Return_type>
inline
Return_type
future_return_value<Return_type>::getValue()
{
    // Short circuit this, just in case it's ready.
    if (m_valueIsReady)
    {
        return m_returnValue;
    }

    // If it wasn't ready, wait until the value is ready and then return it.
    m_pCondition->requireCondition();

    return m_returnValue;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<class Return_type>
inline
future_return_value<Return_type>::operator Return_type()
{
    return getValue();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<class Return_type>
inline
Return_type
future_return_value<Return_type>::operator->()
{
    // Return the derefrenced return value.  This essentially cascades this 
    // object is if it were a smart pointer (which it essentially can be)
    return *getValue();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<class Return_type>
inline
typename Event<Return_type>::pConnection_type
future_return_value<Return_type>::connect(typename Event<Return_type>::delegate_type _listener)
{
    Threading::CriticalSection guard(m_pMutex);

    // If the value is already ready, invoke the listener now, otherwise
    // it'll be invoked when the return value is ready.
    if (m_valueIsReady)
    {
        _listener(m_returnValue);
    }

    return m_event.connect(_listener);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename Payload_type>
class managed_future_ptr
: public Memory::managed_ptr
    <
          future_return_value<Memory::managed_ptr<Payload_type> >
    >
{
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Event
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_EVENT_EVENT_HPP_INCLUDED
