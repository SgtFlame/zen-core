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
#ifndef ZEN_MATH_MATRIX4_HPP_INCLUDED
#define ZEN_MATH_MATRIX4_HPP_INCLUDED

#include <Zen/Core/Math/Math.hpp>

#include <Zen/Core/Math/Radian.hpp>
#include <Zen/Core/Math/Degree.hpp>
#include <Zen/Core/Math/Vector3.hpp>
#include <Zen/Core/Math/Point3.hpp>

#include <boost/function.hpp>
#include <iostream>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Math {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class Quaternion4;
class MATH_DLL_LINK Matrix4
{
    /// @name Types, Constants and Enums 
    /// @{
public:
    enum init_type
    {
        INIT_ZERO,
        INIT_IDENTITY,
        INIT_NONE
    };
    /// @}

    /// @name Matrix4 Implementation
    /// @{
public:
    void identity();
    void transpose();

    void setXRotation(const Radian& _x);
    void setYRotation(const Radian& _y);
    void setZRotation(const Radian& _z);

    void setXYZRotation(const Radian& _x, const Radian& _y, const Radian& _z);
    void setXYZRotation(const Degree& _x, const Degree& _y, const Degree& _z);

    void setPosition(Real _x, Real _y, Real _z);
    void setPosition(const Point3& _position);
    void getPosition(Point3& _position) const;

    Vector3 getForwardVector(void);
    Vector3 getUpVector(void);
    Vector3 getLeftVector(void);


    Real* data();
    const Real& getElement(int _row, int _col) const;
    /// @}

    /// @name operators
    /// @{
public:
    Matrix4& operator=(const Matrix4& _right);

    Matrix4 operator*(const Matrix4& _right);

    Matrix4& operator*=(const Matrix4& _right);

    /// Move the position of the matrix by the vector amount
    Matrix4& operator+=(const Vector3& _offset);
    /// @}

    /// @name 'Structors
    /// @{
public:
        Matrix4(init_type _init = INIT_NONE);
        Matrix4(const Matrix4& _right);
        Matrix4(const Quaternion4& _q);
        Matrix4(Real _real00, Real _real01, Real _real02, Real _real03,
                Real _real04, Real _real05, Real _real06, Real _real07,
                Real _real08, Real _real09, Real _real10, Real _real11,
                Real _real12, Real _real13, Real _real14, Real _real15 );

       ~Matrix4();

    /// @}

    /// @name Methods
    /// @{
public:
    static Matrix4&(*multiply)(Matrix4&, const Matrix4&);
    static Matrix4&(*copy)(Matrix4&, const Matrix4&);
    /// @}

    /// @name Member Variables
    /// @{
public:
    /// This is public so that anything can manipulate it.
    union
    {
        /// Arranged in [row][column] order
        Real    m_matrix[4][4];
        Real    m_array[16];
    };
    /// @}

};  // class Matrix4

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Inlines
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline Matrix4::Matrix4(const Matrix4& _right)
{
    memcpy(&m_array, &_right.m_array, sizeof(m_array));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Matrix4&
Matrix4::operator=(const Matrix4& _right) 
{ 
    return copy(*this, _right); 
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Matrix4
Matrix4::operator*(const Matrix4& _right)
{ 
    Matrix4 tmp = *this;
    multiply(tmp, _right);

    return tmp;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Matrix4&
Matrix4::operator*=(const Matrix4& _right)
{ 
    return multiply(*this, _right);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Matrix4::Matrix4(Real _real00, Real _real01, Real _real02, Real _real03,
                   Real _real04, Real _real05, Real _real06, Real _real07,
                   Real _real08, Real _real09, Real _real10, Real _real11,
                   Real _real12, Real _real13, Real _real14, Real _real15 )
{
    m_matrix[0][0] = _real00; m_matrix[0][1] = _real01; m_matrix[0][2] = _real02; m_matrix[0][3] = _real03;
    m_matrix[1][0] = _real04; m_matrix[1][1] = _real05; m_matrix[1][2] = _real06; m_matrix[1][3] = _real07;
    m_matrix[2][0] = _real08; m_matrix[2][1] = _real09; m_matrix[2][2] = _real10; m_matrix[2][3] = _real11;
    m_matrix[3][0] = _real12; m_matrix[3][1] = _real13; m_matrix[3][2] = _real14; m_matrix[3][3] = _real15;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline 
void 
Matrix4::setXYZRotation(const Degree& _x, const Degree& _y, const Degree& _z)
{
    const Radian x(_x);
    const Radian y(_y);
    const Radian z(_z);

    setXYZRotation(x, y, z);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline 
void 
Matrix4::setPosition(Real _x, Real _y, Real _z)
{
    m_array[12] = _x;
    m_array[13] = _y;
    m_array[14] = _z;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline 
void 
Matrix4::setPosition(const Point3& _position)
{
    m_array[12] = _position.m_x;
    m_array[13] = _position.m_y;
    m_array[14] = _position.m_z;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline 
void 
Matrix4::getPosition(Point3& _position) const
{
    _position.m_x = m_array[12];
    _position.m_y = m_array[13];
    _position.m_z = m_array[14];
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline 
Matrix4& 
Matrix4::operator+=(const Vector3 &_offset)
{
    m_array[12] += _offset.m_x;
    m_array[13] += _offset.m_y;
    m_array[14] += _offset.m_z;

    return *this;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline 
Real* 
Matrix4::data()
{
    return m_array;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline 
const Real& 
Matrix4::getElement(int _row, int _col) const
{
    return m_matrix[_row][_col];
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline 
Vector3
Matrix4::getForwardVector(void)
{
    return Vector3(m_array[2], m_array[6], m_array[10]);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline 
Vector3
Matrix4::getUpVector(void)
{
    return Vector3(m_array[1], m_array[5], m_array[9]);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline 
Vector3
Matrix4::getLeftVector(void)
{
    return Vector3(m_array[0], m_array[4], m_array[8]);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
void
Matrix4::transpose(void)
{
    //http://www.songho.ca/opengl/gl_transform.html#matrix (for use with systems that use different column or row major setups)
    Real temp;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            temp = m_matrix[i][j];
            m_matrix[i][j] = m_matrix[j][i];
            m_matrix[j][i] = temp;
        }
    }
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Math
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_MATH_MATRIX4_HPP_INCLUDED
