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
#ifndef ZEN_SCRIPTING_SCRIPT_CONVERT_ARGUMENT_HPP_INCLUDED
#define ZEN_SCRIPTING_SCRIPT_CONVERT_ARGUMENT_HPP_INCLUDED

#include "script_type_traits.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Core/Math/Math.hpp>
#include <Zen/Core/Math/Vector3.hpp>
#include <Zen/Core/Math/Vector4.hpp>
#include <Zen/Core/Math/Matrix3.hpp>
#include <Zen/Core/Math/Matrix4.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <boost/cstdint.hpp>

#include <string>
#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Scripting {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// rename to argument_from_script?

class bad_script_convert_argument
:   public Utility::runtime_exception
{
public:
    bad_script_convert_argument(const std::string& _e) : Zen::Utility::runtime_exception(_e) {}
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace detail {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

// bool - is_scriptable_type
// bool - is_managed_scriptable_type
// bool - is_pointer
// bool - is_reference
template<typename Argument_type, bool, bool, bool, bool>
struct script_convert_argument
{
    /// By default Argument_type is ArgumentNoRef_type;
    typedef typename boost::remove_const<
                typename boost::remove_reference<Argument_type>::type
            >::type
                    type;

    inline
    void
    operator()(boost::any& _parm)
    {
        m_value = boost::any_cast<type>(_parm);
    }

    inline
    operator type()
    {
        return m_value;
    }

    type    m_value;
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
/// Reference to a class derived from I_ScriptableType
template<typename Argument_type>
struct script_convert_argument<Argument_type, true, false, false, true>
{
    /// I_ScriptableType& is converted to I_ScriptableType*
    typedef typename boost::remove_reference<Argument_type>::type*  type;
    typedef typename boost::remove_pointer
        <
            typename boost::remove_reference<Argument_type>::type
        >::type base_type;

    typedef typename base_type::ScriptObjectReference_type*          pScriptObjectReference_type;
    inline
    void
    operator()(boost::any& _parm)
    {
        // Scriptable objects passed as an argument to a method are passed
        // as I_ObjectReference*.  Get this value and save it for later use.
        m_pValue = dynamic_cast<pScriptObjectReference_type>
            (boost::any_cast<I_ObjectReference*>(_parm));
    }

    inline base_type* getObject(base_type* _pObject)
    {
        return _pObject;
    }

    inline base_type* getObject(Memory::managed_ptr<base_type> _pObject)
    {
        return _pObject.get();
    }

    inline
    operator Argument_type()
    {
        // Get the point to an object of the Argument_type using the
        // I_ObjectReference* converted from the boost::any.
        base_type* pValue =
            dynamic_cast<base_type*>(this->getObject(this->m_pValue->getObject()));

        // Make sure the value is valid and return it.
        if( pValue != NULL )
        {
            return *pValue;
        }

        // Throw an exception if the argument is of the wrong type.
        std::stringstream errorMessage;
        errorMessage << "Invalid script argument.  Expected " 
                     << typeid(base_type*).name()
                     << ", got "
                     << this->m_pValue->getObject()->getScriptTypeName()
                     ;
        throw bad_script_convert_argument(errorMessage.str());
    }

    pScriptObjectReference_type m_pValue;
};


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
/// managed_ptr of a class derived from I_ScriptableType
template<typename Argument_type>
struct script_convert_argument<Memory::managed_ptr<Argument_type>, false, true, false, false>
{
    /// managed_ptr<I_ScriptableType> is kept as a managed_ptr<I_ScriptableType>
    typedef Memory::managed_ptr<Argument_type>                      type;
    typedef Argument_type                                           base_type;
    typedef typename base_type::ScriptObjectReference_type*         pScriptObjectReference_type;

    inline
    void
    operator()(boost::any& _parm)
    {
        this->m_pValue = dynamic_cast<pScriptObjectReference_type>
            (boost::any_cast<I_ObjectReference*>(_parm));
    }

    inline
    operator type()
    {
        // Get a managed pointer to an object of the argument type.
        // This handles cases where the passed type isn't exactly
        // the same as the expected type, but can be dynamic_cast<> to
        // get the correct type. as<>() handles the conversion correctly.
        type pValue =
            (this->m_pValue->getObject()); // .as<type>();

        // Make sure the value is valid and return it.
        if(pValue.isValid())
        {
            return pValue;
        }

        // Throw an exception if the passed value is of the wrong type.
        std::stringstream errorMessage;
        errorMessage << "Invalid script argument.  Expected " 
                     << typeid(base_type).name()
                     << ", got "
                     << this->m_pValue->getObject()->getScriptTypeName()
                     ;
        throw bad_script_convert_argument(errorMessage.str());
    }

    pScriptObjectReference_type m_pValue;
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
/// naked pointer of a class derived from I_ScriptableType
template<typename Argument_type>
struct script_convert_argument<Argument_type, true, false, true, false>
{
    /// I_ScriptableType* is converted to I_ScriptableType
    typedef typename boost::remove_pointer
        <
            Argument_type
        >::type base_type;
    typedef typename base_type::ScriptObjectReference_type* pScriptObjectReference_type;

    inline
    void
    operator()(boost::any& _parm)
    {
        m_pValue = dynamic_cast<pScriptObjectReference_type>
            (boost::any_cast<I_ObjectReference*>(_parm));
    }

    inline
    operator Argument_type()
    {
        // Get the point to an object of the Argument_type using the
        // I_ObjectReference* converted from the boost::any.
        base_type* pValue =
            dynamic_cast<base_type*>(m_pValue->getObject());

        // Make sure the value is valid and return it.
        // TODO Do we need to do this since we know we're 
        // dealing with a naked pointer?
        if( pValue != NULL )
        {
            return pValue;
        }

        // Throw an exception if the argument is of the wrong type.
        std::stringstream errorMessage;
        errorMessage << "Invalid script argument.  Expected " 
                     << typeid(base_type*).name()
                     << ", got "
                     << m_pValue->getObject()->getScriptTypeName()
                     ;
        throw bad_script_convert_argument(errorMessage.str());
    }

    pScriptObjectReference_type m_pValue;
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
/// (const?) reference to a copyable data type not derived from I_ScriptableType
/// TODO Need to add is_copyable or something?
template<typename Argument_type>
struct script_convert_argument<Argument_type, false, false, false, true>
{
    /// most types as a reference converts to a non-const non-ref type.
    typedef typename boost::remove_const<
                typename boost::remove_reference<Argument_type>::type
            >::type
                                            type;

    inline
    void
    operator()(boost::any& _parm)
    {
        /// If this causes an error, we need to enhance this to support
        /// objects that aren't copyable.
        m_value = boost::any_cast<type>(_parm);
    }

    inline
    operator type&()
    {
        return m_value;
    }

    type        m_value;
};


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
/// (const?) reference an int
template<>
struct script_convert_argument<int&, false, false, false, true>
{
    /// most types as a reference converts to a non-const non-ref type. 
    typedef int type;

    inline
    void
    operator()(boost::any& _parm)
    {
        /// If this causes an error, we need to enhance this to support
        /// objects that aren't copyable.
        m_value = (int)boost::any_cast<Math::Real>(_parm);
    }

    inline
    operator int&()
    {
        return m_value;
    }

    int m_value;
};


typedef std::map<std::string,std::string>   ScriptConfig_type;

/// Convert an intrinsic script type (which is always passed
/// by type*) to a value.
/// First template argument is the argument type, the
/// bool is true fi the argument type is a reference.
/// The default is false and the true (is a reference)
/// is handled by a template specialization.
template<typename ArgumentType_type, bool>
struct script_convert_intrinsic_type
{
	/// @see http://www.boost.org/doc/libs/1_42_0/libs/type_traits/doc/html/boost_typetraits/reference/remove_reference.html
    typedef typename boost::remove_const< typename boost::remove_reference<ArgumentType_type>::type >::type type;

    inline
    void
    operator()(boost::any& _parm)
    {
        m_pValue = boost::any_cast<type*>(_parm);
    }

    inline
    operator ArgumentType_type()
    {
        return *m_pValue;
    }

    type* m_pValue;

};	// struct script_convert_intrinsic_type

/// Template specialization to convert an intrinsic script type
/// from type* to type&
template<typename ArgumentType_type>
struct script_convert_intrinsic_type<ArgumentType_type, true>
{
    typedef typename boost::remove_const< typename boost::remove_reference<ArgumentType_type>::type >::type type;

    inline
    void
    operator()(boost::any& _parm)
    {
        m_pValue = boost::any_cast<type*>(_parm);
    }

    inline
    operator type&()
    {
        return *m_pValue;
    }

    type* m_pValue;

};	// struct script_convert_intrinsic_type<ArgumentType_type, true>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
} // namespace detail
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

template<typename Argument_type>
struct script_convert_argument
:   public detail::script_convert_argument
    <
        Argument_type,
        detail::is_scriptable_type<
            typename boost::remove_reference<
                typename boost::remove_pointer<Argument_type>::type
            >::type
        >::value,
        detail::is_managed_scriptable_type<Argument_type>::value,
        boost::is_pointer<Argument_type>::value,
        boost::is_reference<Argument_type>::value
    >
{
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<>
struct script_convert_argument<int>
{
    /// int argument types are converted to Real
    typedef int                         type;

    inline
    void
    operator()(boost::any& _parm)
    {
        // Convert from a Real to an int
        m_value = (int)boost::any_cast<Zen::Math::Real>(_parm);
    }

    inline
    operator int()
    {
        return m_value;
    }

    int m_value;
};

template<typename ArgumentType_type>
struct script_convert_intrinsic_type
:   public detail::script_convert_intrinsic_type
        <
            ArgumentType_type,
            boost::is_reference<ArgumentType_type>::value
        >
{
};

template<>
struct script_convert_argument<Math::Radian>
:   public script_convert_intrinsic_type<Math::Radian>
{
};

template<>
struct script_convert_argument<Math::Radian&>
:   public script_convert_intrinsic_type<Math::Radian&>
{
};

template<>
struct script_convert_argument<const Math::Radian&>
:   public script_convert_intrinsic_type<const Math::Radian&>
{
};

template<>
struct script_convert_argument<Math::Degree>
:   public script_convert_intrinsic_type<Math::Degree>
{
};

template<>
struct script_convert_argument<Math::Degree&>
:   public script_convert_intrinsic_type<Math::Degree&>
{
};

template<>
struct script_convert_argument<const Math::Degree&>
:   public script_convert_intrinsic_type<const Math::Degree&>
{
};

template<>
struct script_convert_argument<Math::Vector3>
:   public script_convert_intrinsic_type<Math::Vector3>
{
};

template<>
struct script_convert_argument<Math::Vector3&>
:   public script_convert_intrinsic_type<Math::Vector3&>
{
};

template<>
struct script_convert_argument<const Math::Vector3&>
:   public script_convert_intrinsic_type<const Math::Vector3&>
{
};

template<>
struct script_convert_argument<Math::Quaternion4>
:   public script_convert_intrinsic_type<Math::Quaternion4>
{
};

template<>
struct script_convert_argument<Math::Quaternion4&>
:   public script_convert_intrinsic_type<Math::Quaternion4&>
{
};

template<>
struct script_convert_argument<const Math::Quaternion4&>
:   public script_convert_intrinsic_type<const Math::Quaternion4&>
{
};

template<>
struct script_convert_argument<Math::Vector4>
:   public script_convert_intrinsic_type<Math::Vector4>
{
};

template<>
struct script_convert_argument<Math::Vector4&>
:   public script_convert_intrinsic_type<Math::Vector4&>
{
};

template<>
struct script_convert_argument<Math::Matrix3>
:   public script_convert_intrinsic_type<Math::Matrix3>
{
};

template<>
struct script_convert_argument<Math::Matrix3&>
:   public script_convert_intrinsic_type<Math::Matrix3&>
{
};

template<>
struct script_convert_argument<Math::Matrix4>
:   public script_convert_intrinsic_type<Math::Matrix4>
{

};

template<>
struct script_convert_argument<Math::Matrix4&>
:   public script_convert_intrinsic_type<Math::Matrix4&>
{
};

template<>
struct script_convert_argument<detail::ScriptConfig_type>
:   public script_convert_intrinsic_type<detail::ScriptConfig_type>
{
};

template<>
struct script_convert_argument<detail::ScriptConfig_type&>
:   public script_convert_intrinsic_type<detail::ScriptConfig_type&>
{
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<>
struct script_convert_argument<boost::any>
{
    /// boost::any converts to boost::any
    typedef boost::any                  type;

    inline
    void
    operator()(boost::any& _parm)
    {
        m_value = _parm;
    }

    inline
    operator boost::any()
    {
        return m_value;
    }

    boost::any m_value;
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Scripting
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_SCRIPTING_SCRIPT_CONVERT_ARGUMENT_HPP_INCLUDED
