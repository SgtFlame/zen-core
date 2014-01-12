//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Core Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C)        2009 Johan Verbelen
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
/// @author Tony Richards trichards@indiezen.com
/// @author Johan Verbelen johan.verbelen@telenet.be
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_UTILITY_I_CONFIGURATION_ELEMENT_HPP_INCLUDED
#define ZEN_UTILITY_I_CONFIGURATION_ELEMENT_HPP_INCLUDED

#include "Configuration.hpp"

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <string>
#include <list>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Utility {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

class UTILITY_DLL_LINK I_ConfigurationElement
:   boost::noncopyable
{
    /// @name Types
    /// @{
public:
    /// Defined below
    class I_ConfigurationElementVisitor;
    /// @}

    /// @name I_ConfigurationElement interface
    /// @{
public:
    /// Gets the name of this element.
    virtual const std::string& getName() const = 0;

    virtual const std::string& getAttribute(const std::string& _name) const = 0;

    /// Gets all of the children of this element with the specified name
    /// using a visitor.
    virtual void getChildren(const std::string& _name, I_ConfigurationElementVisitor& _visitor) const = 0;

    /// Get a single child with the specified name.
    /// If more than one child with the specified name exists, the first one is returned.
    /// @return NULL if the child does not exist
    virtual const I_ConfigurationElement* getChild(const std::string& _name) const = 0;

    /// Get the parent element
    virtual const I_ConfigurationElement* getParent() const = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ConfigurationElement();
    virtual ~I_ConfigurationElement();
    /// @}

    /// @name Inner Classes
    /// @{
public:
    class UTILITY_DLL_LINK I_ConfigurationElementVisitor
    {
    public:
        virtual void begin() = 0;
        virtual void visit(const I_ConfigurationElement& _element) = 0;
        virtual void end() = 0;
    public:
                 I_ConfigurationElementVisitor();
        virtual ~I_ConfigurationElementVisitor();
    };  // interface I_ConfigurationElementVisitor
};	// interface I_ConfigurationElement

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Plugins
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_UTILITY_I_CONFIGURATION_ELEMENT_HPP_INCLUDED
