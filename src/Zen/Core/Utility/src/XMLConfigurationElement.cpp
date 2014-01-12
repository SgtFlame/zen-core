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
#include "XMLConfigurationElement.hpp"
#include "XMLConfiguration.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Utility {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

XMLConfigurationElement::XMLConfigurationElement(XMLConfiguration& _topParent, XMLConfigurationElement* _pParent, xmlNodePtr const _pNode)
:   m_topParent(_topParent)
,   m_pParent(_pParent)
,   m_isValid(false)
{
    m_name = (char*)_pNode->name;
    m_value = (char*)(_pNode->content ? _pNode->content : (xmlChar*)"");

    xmlAttrPtr pProperty = _pNode->properties;
    while(pProperty != NULL)
    {
        std::string name = (char*)pProperty->name;

        // Get the property value in a kind of roundabout way.
        char* const pRawValue = (char*)xmlGetProp(_pNode, pProperty->name);

        std::string value = pRawValue ? pRawValue : "";

        m_properties.insert(property_pair_type(name, value));

        pProperty = pProperty->next;

        xmlFree(pRawValue);
    }

    // Iterate through the children
    xmlNodePtr pChild = _pNode->children;

    while(pChild != NULL)
    {
        ptr_type pNewChild(new XMLConfigurationElement(m_topParent, this, pChild));
        this->addChild(pNewChild);

        m_topParent.dispatchEvent(pNewChild);

        pChild = pChild->next;
    }

    m_isValid = true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
XMLConfigurationElement::~XMLConfigurationElement()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#if 0
I_ClassFactory*
XMLConfigurationElement::createExecutableExtension(const std::string& _propertyName)
{
    const std::string& className = getAttribute(_propertyName);

    // TODO Implement
    throw Utility::runtime_exception("XMLConfigurationElement::createExecutableExtension(): Not Implemented");
}
#endif

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
XMLConfigurationElement::getAttribute(const std::string& _name) const
{
    static const std::string emptyAttribute("");

    property_collection_type::const_iterator iter = m_properties.find(_name);

    if (iter == m_properties.end())
    {
        return emptyAttribute;
    }
    else
    {
        return iter->second;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
XMLConfigurationElement::getChildren(const std::string& _name, I_ConfigurationElementVisitor& _visitor) const
{
    _visitor.begin();

   children_collection_type::const_iterator iter = m_children.find(_name);

    while(iter != m_children.end() && strcmp(iter->first.c_str(), _name.c_str()) == 0)
    {
        _visitor.visit(*iter->second.get());
        ++iter;
    }

    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const I_ConfigurationElement*
XMLConfigurationElement::getChild(const std::string& _name) const
{
    children_collection_type::const_iterator iter = m_children.find(_name);

    if (iter != m_children.end() && strcmp(iter->first.c_str(), _name.c_str()) == 0)
    {
        return dynamic_cast<I_ConfigurationElement*>(iter->second.get());
    }

    return NULL;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
XMLConfigurationElement::getName() const
{
    return m_name;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#if 0
const std::string&
XMLConfigurationElement::getNamespaceIdentifier()
{
}
#endif

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
XMLConfigurationElement::addChild(ptr_type _pChild)
{
    m_children.insert(children_pair_type(_pChild->getName(), _pChild));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const I_ConfigurationElement*
XMLConfigurationElement::getParent() const
{
    return m_pParent;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Utility
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
