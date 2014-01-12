//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
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
#ifndef ZEN_EVENT_MODELING_CONNECTION_HPP_INCLUDED
#define ZEN_EVENT_MODELING_CONNECTION_HPP_INCLUDED

#include <boost/function.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Event {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

template<class Parm1_type>
class Event;
    
template<class Parm1_type>
class Connection
{
    /// @name Types
    /// @{
public:
    typedef boost::function<void (Parm1_type)>      delegate_type;
    typedef Event<Parm1_type>*                      pEvent_type;
    /// @}

    /// @name Connection implementation
    /// @{
public:
    void operator()(Parm1_type _parm)
    {
        if( !m_isBlocked )
        {
            m_delegate(_parm);
        }
    }

    void block()
    {
        m_isBlocked = true;
    }

    void unblock()
    {
        m_isBlocked = false;
    }

    void disconnect()
    {
        if( !this->m_isDetached )
        {
            m_isDetached = true;
            m_parentEvent->disconnect(this);
        }
    }
    /// @}

    /// @name 'Structors
    /// @{
public:
    Connection<Parm1_type>(const pEvent_type _event, delegate_type _listener) 
    { 
        m_parentEvent = _event;
        m_delegate = _listener;
        m_isBlocked = false;
        m_isDetached = false;
    }

protected:
    friend class Event<Parm1_type>;

    /// This can only be called by Event<> disconnect()
    virtual ~Connection<Parm1_type>() 
    {
        if( !m_isDetached )
        {
            m_isDetached = true;
            m_parentEvent->disconnect(this);
        }
    }
    /// @}

    /// @name Member variables
    /// @{
private:
    pEvent_type     m_parentEvent;
    delegate_type   m_delegate;
    bool            m_isBlocked;
    bool            m_isDetached;
    /// @}

};  // class Connection

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Event
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_EVENT_MODELING_CONNECTION_HPP_INCLUDED
