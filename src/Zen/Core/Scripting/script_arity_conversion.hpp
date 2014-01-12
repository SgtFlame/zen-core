//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Only to be included by script_method_helper

#define Q BOOST_PP_ITERATION()
script_convert_argument<BOOST_PP_CAT(parmType, Q)> BOOST_PP_CAT(parm, Q);
    try
    {
        BOOST_PP_CAT(parm, Q)(_parms[Q]);
    }
    catch(boost::bad_any_cast& _e)
    {
        std::stringstream errorMessage;
        errorMessage << "Bad cast to " << typeid(typename boost::remove_reference<BOOST_PP_CAT(parmType, Q)>::type).name()
            << " for argument number " << Q << " (" << _e.what() << ")";
        throw Zen::Utility::runtime_exception(errorMessage.str());
    }
    catch(bad_script_convert_argument& _e)
    {
        std::stringstream errorMessage;
        errorMessage << _e.what() << " for argument number " << Q << " (" << _e.what() << ")";
        throw Zen::Utility::runtime_exception(errorMessage.str());
    }
#undef Q
