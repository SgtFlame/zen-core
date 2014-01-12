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
#if !defined(BOOST_PP_IS_ITERATING)

#ifndef ZEN_SCRIPTING_SCRIPT_METHOD_HELPER_HPP_INCLUDED
#define ZEN_SCRIPTING_SCRIPT_METHOD_HELPER_HPP_INCLUDED

#ifndef ZEN_SCRIPTING_MAX_SCRIPT_PARMS
#define ZEN_SCRIPTING_MAX_SCRIPT_PARMS 10
#endif

#include <Zen/Core/Scripting/script_convert_argument.hpp>

#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/facilities/expand.hpp>
#include <boost/preprocessor/arithmetic/add.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/iteration/local.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>

#include <boost/preprocessor/iterate.hpp>
#include <boost/preprocessor/cat.hpp>

#include <boost/any.hpp>

#include <sstream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Scripting {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace detail {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
/// Helper template for using the correct version of getRawObject or getObject
/// The first bool is true if the parameter is a managed pointer.
/// The second bool is true if the parameter is a raw pointer.
template<typename ScriptableClass_type, bool, bool>
struct get_script_object_pointer
{
    inline
    ScriptableClass_type*
    operator()(typename ScriptableClass_type::ScriptObjectReference_type* _pObject)
    {
        // If you get a syntax error here, _pObject is not a pointer to
        // ScriptableClass_type and you may need to create a template
        // specialization to handle your case.
        BOOST_STATIC_ASSERT_BOOL_CAST(false);
    }
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
/// Specialization for the case that _pObject is a
/// managed_pointer<ScriptableClass_type>
template<typename ScriptableClass_type>
struct get_script_object_pointer<ScriptableClass_type, true, false>
{
    inline
    ScriptableClass_type*
    operator()(typename ScriptableClass_type::ScriptObjectReference_type* _pObject)
    {
    	ScriptableClass_type* const pScriptable = dynamic_cast<ScriptableClass_type*>(_pObject->getRawObject());
    	assert(pScriptable != NULL);
        return pScriptable;
    }
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
/// Specialization for the case that _pObject is a ScriptableClass_type*
/// (raw pointer)
template<typename ScriptableClass_type>
struct get_script_object_pointer<ScriptableClass_type, false, true>
{
    inline
    ScriptableClass_type*
    operator()(typename ScriptableClass_type::ScriptObjectReference_type* _pObject)
    {
    	ScriptableClass_type* const pScriptable = dynamic_cast<ScriptableClass_type*>(_pObject->getObject());
    	assert(pScriptable != NULL);
        return pScriptable;
    }
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
/// Return_type, is_scriptable_type, is_reference, is_pointer
template<typename Return_type, bool, bool, bool, bool = false, bool = false>
struct
script_override_return_type
{
    typedef Return_type             type;
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
/// Specialization for when Return_type inherits from I_ScriptableType
/// and Return_type is a pointer.
template<typename Return_type>
struct
script_override_return_type<Return_type, true, false, true>
{
    typedef I_ScriptableType*       type;
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
/// Specialization for when Return_type inherits from I_ScriptableType
/// and Return_type is a reference.
template<typename Return_type>
struct
script_override_return_type<Return_type, true, true, false>
{
    typedef I_ScriptableType&       type;
};


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
/// Specialization for when Return_type is a managed_ptr<I_ScriptableType>
/// (or a managed pointer of something that derives from I_ScriptableType).
template<typename Return_type>
struct
script_override_return_type<Return_type, false, false, false, true>
{
    typedef Zen::Memory::managed_ptr<I_ScriptableType>  type;
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
/// Specialization for when Return_type is a managed_weak_ptr<I_ScriptableType>
/// (or a managed weak pointer of something that derives from I_ScriptableType).
template<typename Return_type>
struct
script_override_return_type<Return_type, false, false, false, false, true>
{
    typedef Zen::Memory::managed_weak_ptr<I_ScriptableType>  type;
};


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
} // namespace detail
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/// Determine the real return type to be exported to script.
/// This helps convert references, pointers, and smart pointers
/// to objects of types that inherit I_ScriptableType to I_ScriptObject*
template<typename Return_type>
struct
script_override_return_type
: public detail::script_override_return_type<Return_type,
    detail::is_scriptable_type<Return_type>::value,
    boost::is_reference<Return_type>::value,
    boost::is_pointer<Return_type>::value,
    detail::is_managed_scriptable_type<Return_type>::value,
    detail::is_weak_ptr_scriptable_type<Return_type>::value >
{
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/// This helper is used to get the correct detail::get_script_object_pointer
/// implementation which will convert from
/// ScriptableClass_type::ScriptObjectReference_type*
/// to the appropriate ScriptableClass_type*.
/// ScriptObjectReference_type wraps either a ScriptableClass_type* or a
/// managed_ptr<ScriptableClass_type>.
template<typename ScriptableClass_type, typename object_ptr_type>
struct
get_script_object_pointer
: public detail::get_script_object_pointer<ScriptableClass_type,
		detail::is_managed_scriptable_type<object_ptr_type>::value,
        boost::is_base_of<typename boost::remove_pointer<object_ptr_type>::type, ScriptableClass_type>::value &&
        boost::is_pointer<object_ptr_type>::value
    >
{
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
/// Base script dispatch helper template.
/// This template is used as the base class for derived_dispatch_helperN
/// to abstract the dispatch() and getParameterCount().
template<typename Method_type, typename Return_type, class ScriptableClass_type>
class script_dispatch_helper
{
public:
    typedef typename ScriptableClass_type::ScriptObjectReference_type   ScriptObjectReference_type;
    typedef typename ScriptObjectReference_type::object_ptr_type        object_ptr_type;

    typedef Return_type                                        MethodReturn_type;

    ScriptableClass_type*
    getRawObject(typename ScriptableClass_type::ScriptObjectReference_type* _pObject)
    {
        get_script_object_pointer<ScriptableClass_type, object_ptr_type> get_script_object_pointer;

        return get_script_object_pointer(_pObject);
    }

    virtual
    boost::any dispatch(Method_type _function, I_ObjectReference* _pObject, std::vector<boost::any> _parms) = 0;

    virtual
    unsigned
    getParameterCount() const = 0;
};


#define BOOST_PP_ITERATION_PARAMS_1 \
    (3, (0, ZEN_SCRIPTING_MAX_SCRIPT_PARMS, <Zen/Core/Scripting/script_dispatch_helper.hpp>))

#include BOOST_PP_ITERATE()

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Scripting
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_SCRIPTING_SCRIPT_METHOD_HELPER_HPP_INCLUDED


#else // BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()
#define M BOOST_PP_SUB(N, 1)

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename Method_type, typename Return_type, class ScriptableClass_type BOOST_PP_ENUM_TRAILING_PARAMS_Z(1, N, class parmType)>
class BOOST_PP_CAT(derived_dispatch_helper, N)
:   public script_dispatch_helper<Method_type, Return_type, ScriptableClass_type>
{
public:

    virtual
    boost::any dispatch(Method_type _function, I_ObjectReference* _pObject, std::vector<boost::any> _parms)
    {
        typename ScriptableClass_type::ScriptObjectReference_type* pObject = dynamic_cast<typename ScriptableClass_type::ScriptObjectReference_type*>(_pObject);

#define BOOST_PP_ITERATION_PARAMS_2 \
    (3, (0, M, <Zen/Core/Scripting/script_arity_conversion.hpp>))

// Only expand if M != 0
#if BOOST_PP_IF(N, 1, 0)
#include BOOST_PP_ITERATE()
#endif

        return // need this? (script_convert_argument<typename Return_type>::type)
            (this->getRawObject(pObject)->*_function)(BOOST_PP_ENUM_PARAMS_Z(1, N, parm));
    }

    virtual unsigned getParameterCount() const
    {
        return N;
    }
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

// Specialization for Return_type = void
template<typename Method_type, class ScriptableClass_type BOOST_PP_ENUM_TRAILING_PARAMS_Z(1, N, class parmType)>
class BOOST_PP_CAT(derived_dispatch_helper, N)<Method_type, void, ScriptableClass_type BOOST_PP_ENUM_TRAILING_PARAMS_Z(1, N, parmType)>
:   public script_dispatch_helper<Method_type, void, ScriptableClass_type>
{
public:

    virtual
    boost::any dispatch(Method_type _function, I_ObjectReference* _pObject, std::vector<boost::any> _parms)
    {
        typename ScriptableClass_type::ScriptObjectReference_type* pObject = dynamic_cast<typename ScriptableClass_type::ScriptObjectReference_type*>(_pObject);

#define BOOST_PP_ITERATION_PARAMS_2 \
    (3, (0, M, <Zen/Core/Scripting/script_arity_conversion.hpp>))

// Only expand if M != 0
#if BOOST_PP_IF(N, 1, 0)
#include BOOST_PP_ITERATE()
#endif

        boost::any returnValue;
        (this->getRawObject(pObject)->*_function)(BOOST_PP_ENUM_PARAMS_Z(1, N, parm));
        return returnValue;
    }

    virtual unsigned getParameterCount() const
    {
        return N;
    }
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Specialization for Return_type = I_ScriptableType&
template<typename Method_type, class ScriptableClass_type BOOST_PP_ENUM_TRAILING_PARAMS_Z(1, N, class parmType)>
class BOOST_PP_CAT(derived_dispatch_helper, N)<Method_type, I_ScriptableType&, ScriptableClass_type BOOST_PP_ENUM_TRAILING_PARAMS_Z(1, N, parmType)>
:   public script_dispatch_helper<Method_type, I_ScriptableType&, ScriptableClass_type>
{
public:

    virtual
    boost::any dispatch(Method_type _function, I_ObjectReference* _pObject, std::vector<boost::any> _parms)
    {
        typename ScriptableClass_type::ScriptObjectReference_type* pObject = dynamic_cast<typename ScriptableClass_type::ScriptObjectReference_type*>(_pObject);

#define BOOST_PP_ITERATION_PARAMS_2 \
    (3, (0, M, <Zen/Core/Scripting/script_arity_conversion.hpp>))

// Only expand if M != 0
#if BOOST_PP_IF(N, 1, 0)
#include BOOST_PP_ITERATE()
#endif

        // Return I_ObjectReference*
        return dynamic_cast<I_ScriptableType*>(&(this->getRawObject(pObject)->*_function)(BOOST_PP_ENUM_PARAMS_Z(1, N, parm)))->getScriptObject();
    }

    virtual unsigned getParameterCount() const
    {
        return N;
    }
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Specialization for Return_type = I_ScriptableType*
template<typename Method_type, class ScriptableClass_type BOOST_PP_ENUM_TRAILING_PARAMS_Z(1, N, class parmType)>
class BOOST_PP_CAT(derived_dispatch_helper, N)<Method_type, I_ScriptableType*, ScriptableClass_type BOOST_PP_ENUM_TRAILING_PARAMS_Z(1, N, parmType)>
:   public script_dispatch_helper<Method_type, I_ScriptableType*, ScriptableClass_type>
{
public:

    virtual
    boost::any dispatch(Method_type _function, I_ObjectReference* _pObject, std::vector<boost::any> _parms)
    {
        typename ScriptableClass_type::ScriptObjectReference_type* pObject = dynamic_cast<typename ScriptableClass_type::ScriptObjectReference_type*>(_pObject);

#define BOOST_PP_ITERATION_PARAMS_2 \
    (3, (0, M, <Zen/Core/Scripting/script_arity_conversion.hpp>))

// Only expand if M != 0
#if BOOST_PP_IF(N, 1, 0)
#include BOOST_PP_ITERATE()
#endif

        // Return I_ObjectReference*
        return dynamic_cast<I_ScriptableType*>((this->getRawObject(pObject)->*_function)(BOOST_PP_ENUM_PARAMS_Z(1, N, parm)))->getScriptObject();
    }

    virtual unsigned getParameterCount() const
    {
        return N;
    }
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Specialization for Return_type = Zen::Memory::managed_ptr<I_ScriptableType>
template<typename Method_type, class ScriptableClass_type BOOST_PP_ENUM_TRAILING_PARAMS_Z(1, N, class parmType)>
class BOOST_PP_CAT(derived_dispatch_helper, N)<Method_type, Zen::Memory::managed_ptr<I_ScriptableType>, ScriptableClass_type BOOST_PP_ENUM_TRAILING_PARAMS_Z(1, N, parmType)>
:   public script_dispatch_helper<Method_type, Zen::Memory::managed_ptr<I_ScriptableType>, ScriptableClass_type>
{
public:

    virtual
    boost::any dispatch(Method_type _function, I_ObjectReference* _pObject, std::vector<boost::any> _parms)
    {
        typename ScriptableClass_type::ScriptObjectReference_type* pObject = dynamic_cast<typename ScriptableClass_type::ScriptObjectReference_type*>(_pObject);

#define BOOST_PP_ITERATION_PARAMS_2 \
    (3, (0, M, <Zen/Core/Scripting/script_arity_conversion.hpp>))

// Only expand if M != 0
#if BOOST_PP_IF(N, 1, 0)
#include BOOST_PP_ITERATE()
#endif

        // Return I_ObjectReference*
        return (this->getRawObject(pObject)->*_function)(BOOST_PP_ENUM_PARAMS_Z(1, N, parm))->getScriptObject();
    }

    virtual unsigned getParameterCount() const
    {
        return N;
    }
};


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Specialization for Return_type = Zen::Memory::managed_ptr<I_ScriptableType>
template<typename Method_type, class ScriptableClass_type BOOST_PP_ENUM_TRAILING_PARAMS_Z(1, N, class parmType)>
class BOOST_PP_CAT(derived_dispatch_helper, N)<Method_type, Zen::Memory::managed_weak_ptr<I_ScriptableType>, ScriptableClass_type BOOST_PP_ENUM_TRAILING_PARAMS_Z(1, N, parmType)>
:   public script_dispatch_helper<Method_type, Zen::Memory::managed_weak_ptr<I_ScriptableType>, ScriptableClass_type>
{
public:

    virtual
    boost::any dispatch(Method_type _function, I_ObjectReference* _pObject, std::vector<boost::any> _parms)
    {
        typename ScriptableClass_type::ScriptObjectReference_type* pObject = dynamic_cast<typename ScriptableClass_type::ScriptObjectReference_type*>(_pObject);

#define BOOST_PP_ITERATION_PARAMS_2 \
    (3, (0, M, <Zen/Core/Scripting/script_arity_conversion.hpp>))

// Only expand if M != 0
#if BOOST_PP_IF(N, 1, 0)
#include BOOST_PP_ITERATE()
#endif

        // Return I_ObjectReference*
        return (this->getRawObject(pObject)->*_function)(BOOST_PP_ENUM_PARAMS_Z(1, N, parm)).lock()->getScriptObject();
    }

    virtual unsigned getParameterCount() const
    {
        return N;
    }
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

namespace detail {

/// mutable get_dispatch_helper.
// This version is used to get the dispatch helper for non-const methods.
template<typename Method_type, typename Return_type, typename OverrideReturn_type, class ScriptableClass_type BOOST_PP_ENUM_TRAILING_PARAMS_Z(1, N, class parmType)>
script_dispatch_helper<Method_type, OverrideReturn_type, ScriptableClass_type>&
get_dispatch_helper(Method_type _function, Return_type(ScriptableClass_type::*_method)(BOOST_PP_ENUM_PARAMS_Z(1, N, parmType)))
{
    static BOOST_PP_CAT(derived_dispatch_helper, N) <Method_type,
        OverrideReturn_type,
        ScriptableClass_type
        BOOST_PP_ENUM_TRAILING_PARAMS_Z(1, N, parmType)
    > tmp;
    return tmp;
}

/// const get_dispatch_helper.
/// This version is used for getting the dispatch helper for const methods.
template<typename Method_type, typename Return_type, typename OverrideReturn_type, class ScriptableClass_type BOOST_PP_ENUM_TRAILING_PARAMS_Z(1, N, class parmType)>
script_dispatch_helper<Method_type, OverrideReturn_type, ScriptableClass_type>&
get_dispatch_helper(Method_type _function, Return_type(ScriptableClass_type::*_method)(BOOST_PP_ENUM_PARAMS_Z(1, N, parmType)) const)
{
    static BOOST_PP_CAT(derived_dispatch_helper, N) <Method_type,
        OverrideReturn_type,
        ScriptableClass_type
        BOOST_PP_ENUM_TRAILING_PARAMS_Z(1, N, parmType)
    > tmp;
    return tmp;
}

} // namespace detail

/// This template function is used to define the OverrideReturn_type which is used by
/// detail::get_dispatch_helper.
/// OverrideReturn_type is script_override_return_type<Return_type>::type, which
/// will override the return type using some template specialization.  Any return type which
/// is a pointer or a reference to an I_ScriptableType will be changed to return
/// I_ScriptableType* instead of Return_type.
/// This is facilitated by derived_dispatch_helper template specialization.
template<typename Method_type, typename Return_type, class ScriptableClass_type BOOST_PP_ENUM_TRAILING_PARAMS_Z(1, N, class parmType)>
script_dispatch_helper<Method_type, typename script_override_return_type<Return_type>::type, ScriptableClass_type>&
get_dispatch_helper(Method_type _function, Return_type(ScriptableClass_type::*_method)(BOOST_PP_ENUM_PARAMS_Z(1, N, parmType)))
{
    return detail::get_dispatch_helper<Method_type, Return_type, typename script_override_return_type<Return_type>::type,
        ScriptableClass_type BOOST_PP_ENUM_TRAILING_PARAMS_Z(1, N, parmType)>(_function, _method);
}

template<typename Method_type, typename Return_type, class ScriptableClass_type BOOST_PP_ENUM_TRAILING_PARAMS_Z(1, N, class parmType)>
script_dispatch_helper<Method_type, typename script_override_return_type<Return_type>::type, ScriptableClass_type>&
get_dispatch_helper(Method_type _function, Return_type(ScriptableClass_type::*_method)(BOOST_PP_ENUM_PARAMS_Z(1, N, parmType)) const)
{
    return detail::get_dispatch_helper<Method_type, Return_type, typename script_override_return_type<Return_type>::type,
        ScriptableClass_type BOOST_PP_ENUM_TRAILING_PARAMS_Z(1, N, parmType)>(_function, _method);
}

#undef N
#undef M

#endif // BOOST_PP_IS_ITERATING
