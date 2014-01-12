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
#ifndef ZEN_MATH_QUATERNION4_HPP_INCLUDED
#define ZEN_MATH_QUATERNION4_HPP_INCLUDED

#include <Zen/Core/Math/Math.hpp>
#include <Zen/Core/Math/Vector3.hpp>
#include <Zen/Core/Math/Matrix4.hpp>

//#define _USE_MATH_DEFINES
//#include <math.h>
//#undef _USE_MATH_DEFINES

#include <float.h>
#include <stdlib.h>
#include <algorithm>

#ifdef HOST_WIN32
#define COPYSIGN _copysign
#else
#define COPYSIGN copysign
#endif

#ifdef HOST_WIN32
#define FINITE _finite
#else
#define FINITE finite
#endif

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Math {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class MATH_DLL_LINK Quaternion4
{
    /// @name Quaternion4 implementation
    /// @{
public:
    bool isFinite() const;
    static Quaternion4 zero();
    static Quaternion4 identity();
    static Quaternion4 random();

    /// @brief initializer function setting the orientation of the quaternion
    ///        based on an input angle (in degrees) and axis.
    void fromAngleAxis(Real _angle, const Vector3& _axis);

    /// @brief same as fromAngleAxis() above, except assumes the axis is already
    ///        normalized, and the angle is in radians instead of degrees.
    void fromAngleAxisFast(Real _radians, const Vector3& _axis);

    /// @brief gives the angle of rotation about the axis defined by the x, y,
    ///        and z terms of the quaternion, as derived from the w term.
    Real getAngle() const;

    /// @brief gives the angle between the current quaternion and the input
    ///        quaternion in 4-space (which is important for a slerp operation)
    Real getAngle(const Quaternion4& _quaternion) const;

    /// @brief gets the XYZ axis (normalized) without any W rotation
    Vector3 getAxis() const;

    Real magnitudeSquared() const;
    Real dot(const Quaternion4& _right) const;
    void normalize();
    void conjugate();

    /// @brief linear interpolation from one quaternion to another (rotation
    ///        only).  't' is a value between 0.0f and 1.0f representing the
    ///        percentage along the way from the current quaternion to the
    ///        target (input) quaternion.  returns the interpolated quaternion.
    Quaternion4 slerp(const Real t, const Quaternion4& _right) const;

    /// @brief rotates an input vector along the quaternion axis by the angle of
    ///        rotation of the quaternion.
    ///        Input vector is a 3-space point and return vector is that same point
    ///        rotated around in accordance with the orientation of the quaternion.
    /// @todo change input/output types from vector3 to point3
    Vector3 rotate(const Vector3& _v) const;

    /// @brief same as rotate(), but opposite direction of rotation
    /// @todo change input/output types from vector3 to point3
    Vector3 invRotate(const Vector3& _v) const;
    Vector3 transform(const Vector3& _v, const Vector3& _t) const;
    Vector3 invTransform(const Vector3& _v, const Vector3& _t) const;
    /// @}

    /// @name Operators
    /// @{
public:
    Quaternion4& operator=(const Quaternion4& _right);
    Quaternion4& operator=(const Vector3& _right);

    Quaternion4 operator-() const;
    Quaternion4 operator+(const Quaternion4& _right) const;
    Quaternion4 operator-(const Quaternion4& _right) const;
    Quaternion4 operator*(const Quaternion4& _right) const;
    Quaternion4 operator*(const Vector3& _right) const;
    Quaternion4 operator*(const Real _right) const;
    Quaternion4 operator/(const Real _right) const;

    bool operator==(const Quaternion4& _right) const;

    /// @}

    /// @name 'Structors
    /// @{
public:
             /// This constructor DOES NOT initialize anything!
             Quaternion4();
             /// Initialize the vector to the specified w, x, y, z
             Quaternion4(Real _w, Real _x, Real _y, Real _z);
             Quaternion4(const Quaternion4& _right);
             Quaternion4(const Matrix4& _mat);
             Quaternion4(const Radian& _angle, const Vector3& _axis);
            ~Quaternion4();
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
            Real    m_w;
            Real    m_x;
            Real    m_y;
            Real    m_z;
        };
    };

    static Real m_PI_2;
    /// @}
};  // class Quaternion4

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Inlines
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
bool 
Quaternion4::isFinite() const
{
    return FINITE(m_w) && FINITE(m_x) && FINITE(m_y) && FINITE(m_z);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Quaternion4 
Quaternion4::zero()
{
    return Quaternion4(0.0f, 0.0f, 0.0f, 0.0f);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Quaternion4 
Quaternion4::identity()
{
    return Quaternion4(1.0f, 0.0f, 0.0f, 0.0f);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Quaternion4
Quaternion4::random()
{
    Quaternion4 q = Quaternion4(
        (Real)(2.0*((double)rand())/((double)RAND_MAX) - 1.0),
        (Real)(2.0*((double)rand())/((double)RAND_MAX) - 1.0),
        (Real)(2.0*((double)rand())/((double)RAND_MAX) - 1.0),
        (Real)(2.0*((double)rand())/((double)RAND_MAX) - 1.0));
    q.normalize();
    return q;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
void 
Quaternion4::fromAngleAxis(Real _angle, const Vector3& _axis)
{
    Vector3 axis = _axis;
    axis.normalize();
    m_x = _axis.m_x;
    m_y = _axis.m_y;
    m_z = _axis.m_z;

    Real half = (Real)m_PI_2 * _angle / 180.0f;
    m_w = cos(half);
    Real sinThetaOverTwo = sin(half);
    m_x *= sinThetaOverTwo;
    m_y *= sinThetaOverTwo;
    m_z *= sinThetaOverTwo;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
void 
Quaternion4::fromAngleAxisFast(Real _radians, const Vector3& _axis)
{
    m_w = cos( _radians * 0.5f );
    Real sinThetaOverTwo = sin( _radians * 0.5f );
    m_x = _axis.m_x * sinThetaOverTwo;
    m_y = _axis.m_y * sinThetaOverTwo;
    m_z = _axis.m_z * sinThetaOverTwo;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Real 
Quaternion4::getAngle() const
{
    return 2.0f * acos(m_w);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Real 
Quaternion4::getAngle(const Quaternion4& _quaternion) const
{
    return 2.0f * acos(this->dot(_quaternion));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Vector3 
Quaternion4::getAxis() const
{
    Real sa = sqrt(1.0f - m_w*m_w);
    if( sa )
    {
        return Vector3( m_x/sa, m_y/sa, m_z/sa );
    }
    return Vector3( 1.0f, 0.0f, 0.0f );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Real 
Quaternion4::magnitudeSquared() const
{
    return dot(*this);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Real 
Quaternion4::dot(const Quaternion4& _right) const
{
    return m_x*_right.m_x + m_y*_right.m_y + m_z*_right.m_z + m_w*_right.m_w;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
void 
Quaternion4::normalize()
{
    const Real magnitude = sqrt(magnitudeSquared());
    if( magnitude )
    {
        const Real invMagnitude = 1.0f / magnitude;
        m_x *= invMagnitude;
        m_y *= invMagnitude;
        m_z *= invMagnitude;
        m_w *= invMagnitude;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
void 
Quaternion4::conjugate() 
{
    m_x = -m_x;
    m_y = -m_y;
    m_z = -m_z;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Quaternion4 
Quaternion4::slerp(const Real t, const Quaternion4& _right) const
{
    const Real epsilon = 1.0e-8f;

    Real cosine = dot(_right);
    
    Real sign = 1.0f;
    if( cosine < 0.0f )
    {
        cosine = -cosine;
        sign = -1.0f;
    }

    Real sine = 1.0f - cosine*cosine;

    if( sine >= epsilon*epsilon )
    {
        sine = sqrt(sine);
        const Real angle = atan2(sine,cosine);
        const Real invSine = 1.0f / sine;

        Real lowerWeight = sin( angle * ( 1.0f - t ) ) * invSine;
        Real upperWeight = sin( angle * t ) * invSine * sign;

        return (*this)*lowerWeight + _right*upperWeight;
    }
    else
    {
        return (*this);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Vector3 
Quaternion4::rotate(const Vector3& _v) const
{
    const Quaternion4 left = (*this)*_v;
    Quaternion4 right = *this;
    right.conjugate();
    Vector3 v;
    v.m_x = left.m_w*right.m_x + left.m_x*right.m_w + left.m_y*right.m_z - left.m_z*right.m_y;
    v.m_y = left.m_w*right.m_y + left.m_y*right.m_w + left.m_z*right.m_x - left.m_x*right.m_z;
    v.m_z = left.m_w*right.m_z + left.m_z*right.m_w + left.m_x*right.m_y - left.m_y*right.m_x;
    return v;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Vector3 
Quaternion4::invRotate(const Vector3& _v) const
{
    Quaternion4 left = (*this)*_v;
    left.conjugate();
    Vector3 v;
    v.m_x = left.m_w*m_x + left.m_x*m_w + left.m_y*m_z - left.m_z*m_y;
    v.m_y = left.m_w*m_y + left.m_y*m_w + left.m_z*m_x - left.m_x*m_z;
    v.m_z = left.m_w*m_z + left.m_z*m_w + left.m_x*m_y - left.m_y*m_x;
    return v;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Vector3 
Quaternion4::transform(const Vector3& _v, const Vector3& _t) const
{
    return rotate(_v) + _t;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Vector3 
Quaternion4::invTransform(const Vector3& _v, const Vector3& _t) const
{
    return invRotate(_v - _t);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Quaternion4&
Quaternion4::operator=(const Quaternion4& _right)
{
    m_w = _right.m_w;
    m_x = _right.m_x;
    m_y = _right.m_y;
    m_z = _right.m_z;

    return *this;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Quaternion4& 
Quaternion4::operator=(const Vector3& _right)
{
    m_x = _right.m_x;
    m_y = _right.m_y;
    m_z = _right.m_z;
    m_w = 1.0f;
    return *this;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Quaternion4 
Quaternion4::operator-() const
{
    return Quaternion4(-m_w,-m_x,-m_y,-m_z);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Quaternion4 
Quaternion4::operator+(const Quaternion4& _right) const
{
    return Quaternion4(m_w+_right.m_w,
                       m_x+_right.m_x,
                       m_y+_right.m_y,
                       m_z+_right.m_z);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Quaternion4 
Quaternion4::operator-(const Quaternion4& _right) const
{
    return Quaternion4(m_w-_right.m_w,
                       m_x-_right.m_x,
                       m_y-_right.m_y,
                       m_z-_right.m_z);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Quaternion4 
Quaternion4::operator*(const Quaternion4& _right) const
{
    return Quaternion4(m_w*_right.m_w - m_x*_right.m_x - m_y*_right.m_y - m_z*_right.m_z,
                       m_w*_right.m_x + m_x*_right.m_w + m_y*_right.m_z - m_z*_right.m_y,
                       m_w*_right.m_y + m_y*_right.m_w + m_z*_right.m_x - m_x*_right.m_z,
                       m_w*_right.m_z + m_z*_right.m_w + m_x*_right.m_y - m_y*_right.m_x);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Quaternion4 
Quaternion4::operator*(const Vector3& _right) const
{
    return Quaternion4(-m_x*_right.m_x - m_y*_right.m_y - m_z*_right.m_z,
                        m_w*_right.m_x + m_y*_right.m_z - m_z*_right.m_y,
                        m_w*_right.m_y + m_z*_right.m_x - m_x*_right.m_z,
                        m_w*_right.m_z + m_x*_right.m_y - m_y*_right.m_x);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Quaternion4 
Quaternion4::operator*(const Real _right) const
{
    return Quaternion4(m_w*_right,
                       m_x*_right,
                       m_y*_right,
                       m_z*_right);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Quaternion4 
Quaternion4::operator/(const Real _right) const
{
    Real inv = 1.0f / _right;
    return Quaternion4(m_w*inv,
                       m_x*inv,
                       m_y*inv,
                       m_z*inv);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
bool 
Quaternion4::operator==(const Quaternion4& _right) const
{
    return (m_w == _right.m_w)
        && (m_x == _right.m_x)
        && (m_y == _right.m_y)
        && (m_z == _right.m_z);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Quaternion4::Quaternion4()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Quaternion4::Quaternion4(Real _w, Real _x, Real _y, Real _z)
:   m_w(_w)
,   m_x(_x)
,   m_y(_y)
,   m_z(_z)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Quaternion4::Quaternion4(const Quaternion4& _right)
:   m_w(_right.m_w)
,   m_x(_right.m_x)
,   m_y(_right.m_y)
,   m_z(_right.m_z)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Quaternion4::Quaternion4(const Matrix4& _mat)
#ifdef ZEN_MATH_USE_DOUBLE
:   m_w(sqrt((std::max)(0.0, 1.0 + _mat.getElement(0, 0) + _mat.getElement(1, 1) + _mat.getElement(2, 2))) / 2)
,   m_x(sqrt((std::max)(0.0, 1.0 + _mat.getElement(0, 0) - _mat.getElement(1, 1) - _mat.getElement(2, 2))) / 2)
,   m_y(sqrt((std::max)(0.0, 1.0 - _mat.getElement(0, 0) + _mat.getElement(1, 1) - _mat.getElement(2, 2))) / 2)
,   m_z(sqrt((std::max)(0.0, 1.0 - _mat.getElement(0, 0) - _mat.getElement(1, 1) + _mat.getElement(2, 2))) / 2)
#else
:   m_w(sqrt((std::max)(0.0f, 1.0f + _mat.getElement(0, 0) + _mat.getElement(1, 1) + _mat.getElement(2, 2))) / 2)
,   m_x(sqrt((std::max)(0.0f, 1.0f + _mat.getElement(0, 0) - _mat.getElement(1, 1) - _mat.getElement(2, 2))) / 2)
,   m_y(sqrt((std::max)(0.0f, 1.0f - _mat.getElement(0, 0) + _mat.getElement(1, 1) - _mat.getElement(2, 2))) / 2)
,   m_z(sqrt((std::max)(0.0f, 1.0f - _mat.getElement(0, 0) - _mat.getElement(1, 1) + _mat.getElement(2, 2))) / 2)
#endif // ZEN_MATH_USE_DOUBLE
{
    // TR - I reviewed this code and the (float) casts are acceptable
    // At this point we are not losing any data due to the precision downcast
    m_x = (Real)COPYSIGN(m_x, _mat.getElement(2, 1) - _mat.getElement(1, 2));
    m_y = (Real)COPYSIGN(m_y, _mat.getElement(0, 2) - _mat.getElement(2, 0));
    m_z = (Real)COPYSIGN(m_z, _mat.getElement(1, 0) - _mat.getElement(0, 1));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Quaternion4::Quaternion4(const Radian& _angle, const Vector3& _axis)
{
    Radian fHalfAngle(0.5f * _angle);
    Real fSin = sin(fHalfAngle);
    m_w = cos(fHalfAngle);
    m_x = fSin * _axis.m_x;
    m_y = fSin * _axis.m_y;
    m_z = fSin * _axis.m_z;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Quaternion4::~Quaternion4()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Math
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_MATH_VECTOR3_HPP_INCLUDED
