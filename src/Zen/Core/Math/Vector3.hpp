//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Core Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
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
#ifndef ZEN_MATH_VECTOR3_HPP_INCLUDED
#define ZEN_MATH_VECTOR3_HPP_INCLUDED

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
class MATH_DLL_LINK Vector3
{
    /// @name Vector3 implementation
    /// @{
public:
    void normalize();

    /// @return the magnitude or length of this vector
    const Real magnitude() const;

    static Real dot(const Vector3& _v1, const Vector3& _v2);
    static Vector3 crossProduct(const Vector3& _v1, const Vector3& _v2);

    std::string toString() const;
    /// @}

    /// @name Operators
    /// @{
public:
    Vector3& operator=(const Vector3& _right);

    Vector3 operator*(const Real _right) const;
    Vector3& operator*=(const Real _right);

    Vector3 operator/(const Real _right) const;
    Vector3 operator+(const Vector3& _right) const;
    Vector3& operator+=(const Vector3& _right);
    Vector3 operator-(const Vector3& _right) const;
    Vector3& operator-=(const Vector3& _right);

    bool operator==(const Vector3& _right) const;
    bool operator!=(const Vector3& _right) const;
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
    static Vector3 ones() { return Vector3(1.0, 1.0, 1.0); }
    /// @}

    /// @name 'Structors
    /// @{
public:
         /// This constructor DOES NOT initialize anything!
         Vector3();
         /// Initialize the vector to the specified x, y, z
         Vector3(Real _x, Real _y, Real _z);
         Vector3(const Vector3& _right);
         Vector3(const Real* _array);
        ~Vector3();
    /// @}

    /// @name Member Variables
    /// @{
public:
    /// This is public so that anything can manipulate it.
    union
    {
        Real    m_array[3];
        struct
        {
            Real    m_x;
            Real    m_y;
            Real    m_z;
        };
    };
    /// @}
};  // class Vector3

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Inlines
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

inline
Vector3&
Vector3::operator=(const Vector3& _right)
{
    m_x = _right.m_x;
    m_y = _right.m_y;
    m_z = _right.m_z;

    return *this;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Vector3::Vector3()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Vector3::Vector3(Real _x, Real _y, Real _z)
:   m_x(_x)
,   m_y(_y)
,   m_z(_z)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Vector3::Vector3(const Real* _array)
:   m_x(_array[0])
,   m_y(_array[1])
,   m_z(_array[2])
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Vector3::Vector3(const Vector3& _right)
:   m_x(_right.m_x)
,   m_y(_right.m_y)
,   m_z(_right.m_z)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Vector3::~Vector3()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline 
void
Vector3::normalize()
{
    const Real magnitudeSquared = m_x * m_x + m_y * m_y + m_z * m_z;
    if (magnitudeSquared > 0.0f)
    {
        const Real oneOverMagnitude = 1.0f / sqrt(magnitudeSquared);

        m_x *= oneOverMagnitude;
        m_y *= oneOverMagnitude;
        m_z *= oneOverMagnitude;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline 
const Real
Vector3::magnitude() const
{
    const Real magnitudeSquared = m_x * m_x + m_y * m_y + m_z * m_z;
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
Vector3::dot(const Vector3& _v1, const Vector3& _v2)
{
    return _v1.m_x * _v2.m_x + _v1.m_y * _v2.m_y + _v1.m_z * _v2.m_z;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Vector3
Vector3::crossProduct(const Vector3& _v1, const Vector3& _v2)
{
    return Vector3(_v1.m_y * _v2.m_z - _v1.m_z * _v2.m_y,
                   _v1.m_z * _v2.m_x - _v1.m_x * _v2.m_z,
                   _v1.m_x * _v2.m_y - _v1.m_y * _v2.m_x);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Vector3
Vector3::operator*(const Real _right) const
{
    return Vector3(m_x * _right, m_y * _right, m_z * _right);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Vector3
operator*(const Real _left, const Vector3& _right)
{
    return _right*_left;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Vector3&
Vector3::operator*=(const Real _right)
{
    m_x *= _right;
    m_y *= _right;
    m_z *= _right;
    return *this;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Vector3 
Vector3::operator/(const Real _right) const
{
    const Real oneOverRight = 1.0f / _right;
    return Vector3(m_x * oneOverRight, m_y * oneOverRight, m_z * oneOverRight);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Vector3
Vector3::operator+(const Vector3& _right) const
{
    Vector3 v = *this;
    v.m_x = m_x + _right.m_x;
    v.m_y = m_y + _right.m_y;
    v.m_z = m_z + _right.m_z;
    return v;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Vector3&
Vector3::operator+=(const Vector3& _right)
{
    m_x += _right.m_x;
    m_y += _right.m_y;
    m_z += _right.m_z;
    return *this;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Vector3
Vector3::operator-(const Vector3& _right) const
{
    Vector3 v = *this;
    v.m_x = m_x - _right.m_x;
    v.m_y = m_y - _right.m_y;
    v.m_z = m_z - _right.m_z;
    return v;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Vector3&
Vector3::operator-=(const Vector3& _right)
{
    m_x -= _right.m_x;
    m_y -= _right.m_y;
    m_z -= _right.m_z;
    return *this;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
bool
Vector3::operator==(const Vector3& _right) const
{
    return (m_x == _right.m_x) && (m_y == _right.m_y) && (m_z == _right.m_z);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
bool
Vector3::operator!=(const Vector3& _right) const
{
    return !(*this == _right);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
void
Vector3::serialize(boost::archive::polymorphic_iarchive& _archive, const int _version)
{
    _archive & boost::serialization::make_nvp("x",m_x);
    _archive & boost::serialization::make_nvp("y",m_y);
    _archive & boost::serialization::make_nvp("z",m_z);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
void
Vector3::serialize(boost::archive::polymorphic_oarchive& _archive, const int _version)
{
    _archive & boost::serialization::make_nvp("x",m_x);
    _archive & boost::serialization::make_nvp("y",m_y);
    _archive & boost::serialization::make_nvp("z",m_z);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
std::string
Vector3::toString() const
{
    std::stringstream stream;
    stream << "[Math::Vector3]{m_x=" << m_x << ",m_y=" << m_y << ",m_z=" << m_z << "}";
    return stream.str();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
std::ostream&
operator<<(std::ostream& _os, const Vector3& _vector3)
{
    _os << _vector3.toString();
    return _os;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Math
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

BOOST_CLASS_IMPLEMENTATION(Zen::Math::Vector3, boost::serialization::object_serializable);
BOOST_CLASS_TRACKING(Zen::Math::Vector3, boost::serialization::track_never);

#endif // ZEN_MATH_VECTOR3_HPP_INCLUDED
