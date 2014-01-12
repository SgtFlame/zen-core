//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Core Framework
//
// Copyright (C) 2001 - 2010 Tony Richards
// Copyright (C) 2008 - 2010 Matthew Alan Gray
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
#ifndef ZEN_MATH_VECTOR2_HPP_INCLUDED
#define ZEN_MATH_VECTOR2_HPP_INCLUDED

#include <Zen/Core/Math/Math.hpp>

#include <boost/archive/polymorphic_iarchive.hpp>
#include <boost/archive/polymorphic_oarchive.hpp>
#include <boost/serialization/nvp.hpp>

#include <math.h>

#include <string>
#include <sstream>
#include <iostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Math {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class MATH_DLL_LINK Vector2
{
    /// @name Vector2 implementation
    /// @{
public:
    void normalize();

    /// @return the magnitude or length of this vector
    const Real magnitude() const;

    static Real dot(const Vector2& _v1, const Vector2& _v2);

    std::string toString() const;
    /// @}

    /// @name Operators
    /// @{
public:
    Vector2& operator=(const Vector2& _right);

    Vector2 operator*(const Real _right) const;
    Vector2& operator*=(const Real _right);

    Vector2 operator/(const Real _right) const;
    Vector2 operator+(const Vector2& _right) const;
    Vector2& operator+=(const Vector2& _right);
    Vector2 operator-(const Vector2& _right) const;
    Vector2& operator-=(const Vector2& _right);

    bool operator==(const Vector2& _right) const;
    bool operator!=(const Vector2& _right) const;
    /// @}

    /// @name Serialization methods
    /// @{
public:
    void serialize(boost::archive::polymorphic_iarchive& _archive, const int _version);

    void serialize(boost::archive::polymorphic_oarchive& _archive, const int _version);
    /// @}

    /// @name Static methods
    /// @{
public:
    static Vector2 ones() { return Vector2(1.0, 1.0); }
    /// @}

    /// @name 'Structors
    /// @{
public:
         /// This constructor DOES NOT initialize anything!
         Vector2();
         /// Initialize the vector to the specified x, y, z
         Vector2(Real _x, Real _y);
         Vector2(const Vector2& _right);
         Vector2(const Real* _array);
        ~Vector2();
    /// @}

    /// @name Member Variables
    /// @{
public:
    /// This is public so that anything can manipulate it.
    union
    {
        Real    m_array[2];
        struct
        {
            Real    m_x;
            Real    m_y;
        };
    };
    /// @}
};  // class Vector2

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Inlines
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

inline
Vector2&
Vector2::operator=(const Vector2& _right)
{
    m_x = _right.m_x;
    m_y = _right.m_y;

    return *this;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Vector2::Vector2()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Vector2::Vector2(Real _x, Real _y)
:   m_x(_x)
,   m_y(_y)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Vector2::Vector2(const Real* _array)
:   m_x(_array[0])
,   m_y(_array[1])
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Vector2::Vector2(const Vector2& _right)
:   m_x(_right.m_x)
,   m_y(_right.m_y)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Vector2::~Vector2()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline 
void
Vector2::normalize()
{
    const Real magnitudeSquared = m_x * m_x + m_y * m_y;
    if (magnitudeSquared > 0.0f)
    {
        const Real oneOverMagnitude = 1.0f / sqrt(magnitudeSquared);

        m_x *= oneOverMagnitude;
        m_y *= oneOverMagnitude;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline 
const Real
Vector2::magnitude() const
{
    const Real magnitudeSquared = m_x * m_x + m_y * m_y;
    if (magnitudeSquared > 0.0f)
    {
        return sqrt(magnitudeSquared);
    }
    else
    {
        return 0;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Real
Vector2::dot(const Vector2& _v1, const Vector2& _v2)
{
    return _v1.m_x * _v2.m_x + _v1.m_y * _v2.m_y;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Vector2
Vector2::operator*(const Real _right) const
{
    return Vector2(m_x * _right, m_y * _right);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Vector2
operator*(const Real _left, const Vector2& _right)
{
    return _right*_left;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Vector2&
Vector2::operator*=(const Real _right)
{
    m_x *= _right;
    m_y *= _right;
    return *this;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Vector2 
Vector2::operator/(const Real _right) const
{
    const Real oneOverRight = 1.0f / _right;
    return Vector2(m_x * oneOverRight, m_y * oneOverRight);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Vector2
Vector2::operator+(const Vector2& _right) const
{
    Vector2 v = *this;
    v.m_x = m_x + _right.m_x;
    v.m_y = m_y + _right.m_y;
    return v;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Vector2&
Vector2::operator+=(const Vector2& _right)
{
    m_x += _right.m_x;
    m_y += _right.m_y;
    return *this;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Vector2
Vector2::operator-(const Vector2& _right) const
{
    Vector2 v = *this;
    v.m_x = m_x - _right.m_x;
    v.m_y = m_y - _right.m_y;
    return v;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Vector2&
Vector2::operator-=(const Vector2& _right)
{
    m_x -= _right.m_x;
    m_y -= _right.m_y;
    return *this;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
bool
Vector2::operator==(const Vector2& _right) const
{
    return (m_x == _right.m_x) && (m_y == _right.m_y);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
bool
Vector2::operator!=(const Vector2& _right) const
{
    return !(*this == _right);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
void
Vector2::serialize(boost::archive::polymorphic_iarchive& _archive, const int _version)
{
    _archive & boost::serialization::make_nvp("x",m_x);
    _archive & boost::serialization::make_nvp("y",m_y);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
void
Vector2::serialize(boost::archive::polymorphic_oarchive& _archive, const int _version)
{
    _archive & boost::serialization::make_nvp("x",m_x);
    _archive & boost::serialization::make_nvp("y",m_y);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
std::string
Vector2::toString() const
{
    std::stringstream stream;
    stream << "[Math::Vector2]{m_x=" << m_x << ",m_y=" << m_y << "}";
    return stream.str();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
std::ostream&
operator<<(std::ostream& _os, const Vector2& _vector2)
{
    _os << _vector2.toString();
    return _os;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Math
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

BOOST_CLASS_IMPLEMENTATION(Zen::Math::Vector2, boost::serialization::object_serializable);
BOOST_CLASS_TRACKING(Zen::Math::Vector2, boost::serialization::track_never);

#endif // ZEN_MATH_VECTOR2_HPP_INCLUDED
