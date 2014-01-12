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
#ifndef ZEN_MATH_POINT3_HPP_INCLUDED
#define ZEN_MATH_POINT3_HPP_INCLUDED

#include <Zen/Core/Math/Math.hpp>

#include <Zen/Core/Math/Vector3.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Math {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/// Faster, inline implementation of I_Point3
class MATH_DLL_LINK Point3
{
    /// @name Point3 implementation
    /// @{
public:
    Real getX() const;
    void setX(const Real _x);

    Real getY() const;
    void setY(const Real _y);

    Real getZ() const;
    void setZ(const Real _z);

    Point3& operator=(const Point3& _right);
    bool operator==(const Point3& _right) const;
    bool operator!=(const Point3& _right) const;

    Point3 operator*(Real _right) const;
    Point3 operator/(Real _right) const;

    Point3 operator+(const Vector3& _right) const;
    Point3& operator+=(const Vector3& _right);

    // Subtract one point from another to get a vector
    Vector3 operator-(const Point3& _right) const;

    // Add one point to another to get a vector
    Vector3 operator+(const Point3& _right) const;

    // Cast a point to a vector
    operator Vector3()
    {
        return Vector3(m_array);
    }

    Real dot(const Vector3& _right) const;

    /// Get the index of the lowest value (x = 0; y = 1; z = 2;)
    /// This is useful when combined with barycentric coordinates
    unsigned lowest() const;

    /// Get the index of the highest value (x = 0; y = 1; z = 2;)
    /// This is useful when combined with barycentric coordinates
    unsigned highest() const;
    /// @}

    /// @name Static methods
    /// @{
public:
    static const Point3 zero() { return Point3(0.0, 0.0, 0.0); }

    static const Point3 ones() { return Point3(1.0, 1.0, 1.0); }
    /// @}

    /// @name 'Structors
    /// @{
    /// @todo Code Review - public or protected?
public:
             Point3();
             Point3(Real _x, Real _y, Real _z);
             Point3(const Point3& _right);
             Point3(const Real* _array);
    virtual ~Point3();
    /// @}

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

};  // class Point3


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Inlines
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Point3::Point3() 
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Point3::Point3(Real _x, Real _y, Real _z)
{ 
    m_x = _x; 
    m_y = _y; 
    m_z = _z; 
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Point3::Point3(const Real* _array)
:   m_x(_array[0])
,   m_y(_array[1])
,   m_z(_array[2])
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Point3::Point3(const Point3& _right) 
{ 
    m_x = _right.m_x; 
    m_y = _right.m_y; 
    m_z = _right.m_z; 
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Point3::~Point3()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline 
Real
Point3::getX() const
{ 
    return m_x; 
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline 
void
Point3::setX(const Real _x) 
{ 
    m_x = _x; 
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline 
Real
Point3::getY() const 
{ 
    return m_y; 
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
void
Point3::setY(const Real _y)
{ 
    m_y = _y; 
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Real
Point3::getZ() const 
{ 
    return m_z; 
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
void
Point3::setZ(const Real _z) 
{ 
    m_z = _z;  
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Point3&
Point3::operator=(const Point3& _right)
{
    m_x = _right.m_x;
    m_y = _right.m_y;
    m_z = _right.m_z;
    return *this;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
bool
Point3::operator==(const Point3& _right) const
{
    return m_x == _right.m_x && 
        m_y == _right.m_y &&
        m_z == _right.m_z;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
bool
Point3::operator!=(const Point3& _right) const
{
    return !(*this==_right);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Point3
Point3::operator*(Real _right) const
{
    return Point3(m_x * _right, m_y * _right, m_z * _right);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Point3 
Point3::operator/(Real _right) const
{
    const Real oneOverRight = 1.0f / _right;
    return Point3(m_x * oneOverRight, m_y * oneOverRight, m_z * oneOverRight);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Vector3
Point3::operator-(const Point3& _right) const
{
    return Vector3(m_x - _right.m_x, m_y - _right.m_y, m_z - _right.m_z);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Vector3
Point3::operator+(const Point3& _right) const
{
    return Vector3(m_x + _right.m_x, m_y + _right.m_y, m_z + _right.m_z);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Real
Point3::dot(const Vector3& _right) const
{
    return m_x * _right.m_x + m_y * _right.m_y + m_z * _right.m_z;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Point3
Point3::operator+(const Vector3& _right) const
{
    return Point3(m_x + _right.m_x, m_y + _right.m_y, m_z + _right.m_z);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Point3&
Point3::operator+=(const Vector3& _right)
{
    m_x += _right.m_x;
    m_y += _right.m_y;
    m_z += _right.m_z;

    return *this;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
unsigned
Point3::lowest() const
{
    if (m_x <= m_y)
    {
        if (m_x <= m_z)
        {
            return 0;
        }
        else
        {
            return 2;
        }
    }
    else if (m_y <= m_z)
    {
        return 1;
    }
    else
    {
        return 2;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
unsigned
Point3::highest() const
{
    if (m_x > m_y)
    {
        if (m_x > m_z)
        {
            return 0;
        }
        else
        {
            return 2;
        }
    }
    else if (m_y > m_z)
    {
        return 1;
    }
    else
    {
        return 2;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Math
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_MATH_POINT3_HPP_INCLUDED
