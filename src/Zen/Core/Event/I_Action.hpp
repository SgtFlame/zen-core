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
#ifndef ZEN_EVENT_I_ACTION_HPP_INCLUDED
#define ZEN_EVENT_I_ACTION_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_self_ref.hpp>

#include <Zen/Core/Scripting/I_ScriptableService.hpp>
#include <Zen/Core/Scripting/ObjectReference.hpp>

#include <boost/any.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Event {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_Connection;

/// @brief Action interface
class EVENT_DLL_LINK I_Action
:   public Scripting::I_ScriptableType
,   public Memory::managed_self_ref<I_Action>
{
    /// @name Forward Declaration
    /// @{
public:
    struct I_ConnectionVisitor;
    /// @}

    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<I_Action>                   pScriptObject_type;
    typedef Scripting::ObjectReference<I_Action>            ScriptObjectReference_type;
    typedef Scripting::ObjectReference<I_Action>            ScriptWrapper_type;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual const std::string& getScriptTypeName();
    /// @}

    /// @name I_Action interface
    /// @{
public:
    /// Dispatch this action.
    virtual void dispatch(boost::any& _parameter) = 0;

    /// Add the action connection.
    virtual void addConnection(I_Connection& _pConnection) = 0;

    /// Remove the action connection.
    virtual void removeConnection(I_Connection& _pConnection) = 0;

    /// Get the action connection.
    virtual void getConnections(I_ConnectionVisitor& _visitor) = 0;
    /// @}

    /// @name Inner Structures
    /// @{
public:
    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    struct I_ConnectionVisitor
    {
        virtual void begin() = 0;
        virtual void visit(I_Connection& _connection) = 0;
        virtual void end() = 0;
    };  // interface I_ConnectionVisitor
    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_Action();
    virtual ~I_Action();
    /// @}

};  // interface I_Action

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Event
namespace Memory {
    // I_Action is managed by factory.
    template<>
    struct is_managed_by_factory<Event::I_Action> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_EVENT_I_ACTION_HPP_INCLUDED
