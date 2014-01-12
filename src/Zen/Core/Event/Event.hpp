//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Core Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C) 2008        Matthew Alan Gray
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
//  Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_EVENT_EVENT_HPP_INCLUDED
#define ZEN_EVENT_EVENT_HPP_INCLUDED

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Event/Connection.hpp>
#include <Zen/Core/Threading/I_Mutex.hpp>
#include <Zen/Core/Threading/MutexFactory.hpp>
#include <Zen/Core/Threading/CriticalSection.hpp>

#include <boost/function.hpp>

#include <list>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Event {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

/// FIFO Synchronous Event template.
///
/// This Event template provides a mechanism for publishing and
/// subscribing to events.  The parameter to this template is
/// the type of the parameter that is passed to the delegate when
/// the event is fired.
///
/// The Event is thread safe in that you can safely connect, disconnect
/// and publish events in multiple threads.
///
/// Delegates are registered using the Event::connect() method, which returns
/// a pointer to a connection.  
///
/// An event is published by invoking the operator() method on the
/// Event object, passing the payload as a parameter to the method.
///
/// The payload is then passed as a parameter to every delegate's operator(),
/// synchronously and in the order that the delegate was connected. (FIFO).
///
/// @param Parm1_type Payload type that is passed to the delegate
///     when the event fires.
template<class Parm1_type>
class Event
{
    /// @name Types
    /// @{
public:
    typedef Threading::I_Mutex*                         pMutex_type;
    typedef boost::function<void (Parm1_type)>          delegate_type;
    typedef Connection<Parm1_type>                      connection_type;
    typedef connection_type*                            pConnection_type;
    typedef std::list<pConnection_type>	                container_type;
    /// @}

    /// @name Event implementation
    /// @{
public:

    /// Connect a delegate to an Event.
    ///
    /// To disconnect the event, call the Connection::disconnect() 
    /// method on the returned connection.
    ///
    /// @return pointer to a Connection<>
    pConnection_type connect(delegate_type _listener)
    {
        pConnection_type pConnection = 
            new connection_type(this, _listener);

        Threading::CriticalSection guard(m_pMutex);

        m_connections.push_back(pConnection);

        return pConnection;
    }

    /// Fires an Event.
    /// This should not be called by anything other than the object that owns the event.
    void operator()(Parm1_type _parm)
    {
        Threading::CriticalSection guard(m_pMutex);

        for( typename container_type::iterator iter = m_connections.begin(); 
            iter != m_connections.end(); 
            iter++)
        {
            (**iter)(_parm);
        }
    }

private:
    /// For internal use only.  This should only be called by the
    /// connection.  Use Connection::disconnect() instead.
    /// @see Connection::disconnect()
    template<class ConnectionParm1_type> friend class Connection;
    void disconnect(pConnection_type _pConnection)
    {
        Threading::CriticalSection guard(m_pMutex);

        // TODO Optimize.  This requires a linear lookup
        // It would be better to use a boost intrusive list
        // of some sort.
        m_connections.remove(_pConnection);

        // This is the only place where a connection is deleted.
        delete _pConnection;
    }
    /// @}

    /// @name 'Structors
    /// @{
public:
             Event<Parm1_type>();
    virtual ~Event<Parm1_type>();
    /// @}

    /// @name Member Variables
    /// @{
private:
    container_type	m_connections;
    pMutex_type     m_pMutex;
    /// @}

};  // class Event

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<class Parm1_type>
inline
Event<Parm1_type>::Event()
:   m_connections()
,   m_pMutex(Threading::MutexFactory::create())
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<class Parm1_type>
inline
Event<Parm1_type>::~Event()
{
    /// When a connection is disconnected, it is removed from
    /// the m_connections, so we cannot be iterating through the
    /// list and disconnecting at the same time.  To solve this
    /// problem, we splice the m_connections list onto tempConnections
    /// and use tempConnections for iterating instead of m_connections.
    container_type tempConnections;

    // This section must be synchronized
    {
        Threading::CriticalSection guard(m_pMutex);

        // splice() is an efficient way to remove items from one list
        // and put it in another list without churning the memory allocator.
        tempConnections.splice(tempConnections.begin(), m_connections);
    }

    // Iterate through the original list of connections and disconnect them.
    if( !tempConnections.empty() )
    {
        for( typename container_type::iterator iter = tempConnections.begin();
             iter != tempConnections.end() ;
             iter++ )
        {
            (*iter)->disconnect();
        }
    }

    // And finally, destroy the mutex.
    Threading::MutexFactory::destroy(m_pMutex);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Event
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_EVENT_EVENT_HPP_INCLUDED
