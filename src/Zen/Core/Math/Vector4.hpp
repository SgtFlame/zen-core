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
//  Tony Richards trichards@indiezen.com
//  Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_MATH_VECTOR4_HPP_INCLUDED
#define ZEN_MATH_VECTOR4_HPP_INCLUDED

#include <Zen/Core/Math/Math.hpp>

#include <math.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Math {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class Matrix4;

class MATH_DLL_LINK Vector4
{
    /// @name Vector4 implementation
    /// @{
public:
    void normalize();

    static Real dot(const Vector4& _v1, const Vector4& _v2);
    /// @}

    /// @name Operators
    /// @{
public:
    Vector4& operator=(const Vector4& _right);

    Vector4 operator*(const Real _right) const;
    Vector4 operator*(const Zen::Math::Matrix4& _right) const;
    Vector4 operator/(const Real _right) const;
    Vector4 operator+(const Vector4& _right) const;
    Vector4 operator-(const Vector4& _right) const;

    /// @}

    /// @name 'Structors
    /// @{
public:
         /// This constructor DOES NOT initialize anything!
         Vector4();
         /// Initialize the vector to the specified x, y, z, w
         Vector4(Real _x, Real _y, Real _z, Real _w);
         Vector4(const Vector4& _right);
        ~Vector4();
    /// @}

    /// @name Static methods
    /// @{
public:
    static Vector4&(*multiply)(Vector4&, const Matrix4&);
    /// @}

    /// @name Member Variables
    /// @{
public:
    /// This is public so that anything can manipulate it.
    union
    {
        Real    m_array[4];
        struct
        {
            Real    m_x;
            Real    m_y;
            Real    m_z;
            Real    m_w;
        };
    };
    /// @}
};  // class Vector4

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Inlines
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

inline
Vector4&
Vector4::operator=(const Vector4& _right)
{
    m_x = _right.m_x;
    m_y = _right.m_y;
    m_z = _right.m_z;
    m_w = _right.m_w;

    return *this;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Vector4::Vector4()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Vector4::Vector4(Real _x, Real _y, Real _z, Real _w)
:   m_x(_x)
,   m_y(_y)
,   m_z(_z)
,   m_w(_w)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Vector4::Vector4(const Vector4& _right)
:   m_x(_right.m_x)
,   m_y(_right.m_y)
,   m_z(_right.m_z)
,   m_w(_right.m_w)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Vector4::~Vector4()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline 
void
Vector4::normalize()
{
    const Real magnitudeSquared = m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w;
    if (magnitudeSquared > 0.0f)
    {
        const Real oneOverMagnitude = 1.0f / sqrt(magnitudeSquared);

        m_x *= oneOverMagnitude;
        m_y *= oneOverMagnitude;
        m_z *= oneOverMagnitude;
        m_w *= oneOverMagnitude;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Real
Vector4::dot(const Vector4& _v1, const Vector4& _v2)
{
    return _v1.m_x * _v2.m_x + _v1.m_y * _v2.m_y + _v1.m_z * _v2.m_z + _v1.m_w * _v2.m_w;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Vector4
Vector4::operator*(const Real _right) const
{
    return Vector4(m_x * _right, m_y * _right, m_z * _right, m_z * _right);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Vector4
Vector4::operator*(const Matrix4& _right) const
{
    Vector4 tmp = *this;
    multiply(tmp, _right);

    return tmp;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Vector4 
Vector4::operator/(const Real _right) const
{
    const Real oneOverRight = 1.0f / _right;
    return Vector4(m_x * oneOverRight, m_y * oneOverRight, m_z * oneOverRight, m_z * oneOverRight);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Vector4
Vector4::operator+(const Vector4& _right) const
{
    Vector4 v = *this;
    v.m_x = m_x + _right.m_x;
    v.m_y = m_y + _right.m_y;
    v.m_z = m_z + _right.m_z;
    v.m_w = m_w + _right.m_w;
    return v;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Vector4
Vector4::operator-(const Vector4& _right) const
{
    Vector4 v = *this;
    v.m_x = m_x - _right.m_x;
    v.m_y = m_y - _right.m_y;
    v.m_z = m_z - _right.m_z;
    v.m_w = m_w - _right.m_w;
    return v;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Math
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_MATH_VECTOR4_HPP_INCLUDED
