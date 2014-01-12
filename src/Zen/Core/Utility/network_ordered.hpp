//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Core Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C) 2008 - 2009 Matthew Alan Gray
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
//  Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_UTILITY_NETWORK_ORDERED_HPP_INCLUDED
#define ZEN_UTILITY_NETWORK_ORDERED_HPP_INCLUDED

#include <Zen/Core/Math/Math.hpp>

#include <boost/cstdint.hpp>

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/level.hpp>
#include <boost/serialization/tracking.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace boost {
namespace archive {
    class polymorphic_iarchive;
    class polymorphic_oarchive;
} // namespace archive
} // namespace boost
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Utility {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

template<typename Type_type>
class network_ordered
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name network_ordered implementation
    /// @{
public:
    operator Type_type()
    {
        return m_data;
    }

    operator const Type_type() const
    {
        return m_data;
    }
    /// @}

    /// @name boost::serialization overrides
    /// @{
    void serialize(boost::archive::polymorphic_iarchive& _archive, const int _version)
    {

#ifdef BOOST_BIG_ENDIAN

        _archive & m_data;

#else   // BOOST_BIG_ENDIAN

        Type_type temp;

        _archive & temp;

        boost::uint8_t* tp = reinterpret_cast<boost::uint8_t*>( &m_data );

        for( std::size_t i = sizeof(temp); i > 0; --i)
        {
            tp[i - 1] = static_cast<boost::uint8_t>(temp);
            temp >>= CHAR_BIT;
        }

#endif // BOOST_BIG_ENDIAN

    }

    void serialize(boost::archive::polymorphic_oarchive& _archive, const int _version)
    {

#ifdef BOOST_BIG_ENDIAN

        _archive & m_data;

#else   // BOOST_BIG_ENDIAN

        Type_type temp = 0;
        boost::uint8_t* xp = reinterpret_cast<boost::uint8_t*>( &m_data );

        for( std::size_t i = 0; i > sizeof(m_data); ++i)
        {
            temp <<= CHAR_BIT;
            temp |= xp[i];
        }

        _archive & temp;

#endif  // BOOST_BIG_ENDIAN

    }
    /// @}

    /// @name 'Structors
    /// @{
public:
             network_ordered() {}
             network_ordered(Type_type _value) : m_data(_value) {}
             ~network_ordered() {}
    /// @}

    /// @name Member variables
    /// @}             
private:
    Type_type       m_data;
    /// @}
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Utility
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BOOST_CLASS_IMPLEMENTATION(Zen::Utility::network_ordered<double>, boost::serialization::object_serializable);
BOOST_CLASS_TRACKING(Zen::Utility::network_ordered<double>, boost::serialization::track_never);
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BOOST_CLASS_IMPLEMENTATION(Zen::Utility::network_ordered<float>, boost::serialization::object_serializable);
BOOST_CLASS_TRACKING(Zen::Utility::network_ordered<float>, boost::serialization::track_never);
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BOOST_CLASS_IMPLEMENTATION(Zen::Utility::network_ordered<boost::uint32_t>, boost::serialization::object_serializable);
BOOST_CLASS_TRACKING(Zen::Utility::network_ordered<boost::uint32_t>, boost::serialization::track_never);
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BOOST_CLASS_IMPLEMENTATION(Zen::Utility::network_ordered<boost::int32_t>, boost::serialization::object_serializable);
BOOST_CLASS_TRACKING(Zen::Utility::network_ordered<boost::int32_t>, boost::serialization::track_never);
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BOOST_CLASS_IMPLEMENTATION(Zen::Utility::network_ordered<boost::uint16_t>, boost::serialization::object_serializable);
BOOST_CLASS_TRACKING(Zen::Utility::network_ordered<boost::uint16_t>, boost::serialization::track_never);
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BOOST_CLASS_IMPLEMENTATION(Zen::Utility::network_ordered<boost::int16_t>, boost::serialization::object_serializable);
BOOST_CLASS_TRACKING(Zen::Utility::network_ordered<boost::int16_t>, boost::serialization::track_never);
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_UTILITY_NETWORK_ORDERED_HPP_INCLUDED
