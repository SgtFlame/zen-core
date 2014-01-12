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
#ifndef ZEN_MATH_MATRIX3_HPP_INCLUDED
#define ZEN_MATH_MATRIX3_HPP_INCLUDED

#include <Zen/Core/Math/Math.hpp>

#include <Zen/Core/Math/Radian.hpp>
#include <Zen/Core/Math/Degree.hpp>
#include <Zen/Core/Math/Vector3.hpp>

#include <boost/function.hpp>
#include <boost/archive/polymorphic_iarchive.hpp>
#include <boost/archive/polymorphic_oarchive.hpp>
#include <boost/serialization/nvp.hpp>

#include <iostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Math {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class MATH_DLL_LINK Matrix3
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

    Real* data();
    const Real& getElement(int _row, int _col) const;

    Vector3 getForwardVector(void);
    Vector3 getUpVector(void);
    Vector3 getLeftVector(void);

    /// @}

    /// @name operators
    /// @{
public:
    Matrix3& operator=(const Matrix3& _right);

    Matrix3 operator*(const Matrix3& _right);

    Vector3 operator*(const Vector3& _right);

    Matrix3& operator*=(const Matrix3& _right);
    /// @}

    /// @name Serialization methods
    /// @{
public:
    void serialize(boost::archive::polymorphic_iarchive& _archive, const int _version);

    void serialize(boost::archive::polymorphic_oarchive& _archive, const int _version);
    /// @}

    /// @name 'Structors
    /// @{
public:
        Matrix3(init_type _init = INIT_NONE);
        Matrix3(const Matrix3& _right);
        Matrix3(Real _real00, Real _real01, Real _real02,
                Real _real03, Real _real04, Real _real05,
                Real _real06, Real _real07, Real _real08);

       ~Matrix3();

    /// @}

    /// @name Methods
    /// @{
public:
    static Matrix3&(*multiply)(Matrix3&, const Matrix3&);
    static Vector3&(*vectorMultiply)(const Matrix3&, Vector3&);
    static Matrix3&(*copy)(Matrix3&, const Matrix3&);
    /// @}

    /// @name Member Variables
    /// @{
public:
    /// This is public so that anything can manipulate it.
    union
    {
        /// Arranged in [row][column] order
        Real    m_matrix[3][3];
        Real    m_array[9];
    };
    /// @}

};  // class Matrix3

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Inlines
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline Matrix3::Matrix3(const Matrix3& _right)
{
    memcpy(&m_array, &_right.m_array, sizeof(m_array));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Matrix3&
Matrix3::operator=(const Matrix3& _right) 
{ 
    return copy(*this, _right); 
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Matrix3
Matrix3::operator*(const Matrix3& _right)
{ 
    Matrix3 tmp = *this;
    multiply(tmp, _right);

    return tmp;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Vector3
Matrix3::operator*(const Vector3& _right)
{
    Vector3 tmp = _right;
    vectorMultiply(*this, tmp);

    return tmp;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Matrix3&
Matrix3::operator*=(const Matrix3& _right)
{ 
    return multiply(*this, _right);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Matrix3::Matrix3(Real _real00, Real _real01, Real _real02,
                 Real _real03, Real _real04, Real _real05,
                 Real _real06, Real _real07, Real _real08)
{
    m_matrix[0][0] = _real00; m_matrix[0][1] = _real01; m_matrix[0][2] = _real02;
    m_matrix[1][0] = _real03; m_matrix[1][1] = _real04; m_matrix[1][2] = _real05;
    m_matrix[2][0] = _real06; m_matrix[2][1] = _real07; m_matrix[2][2] = _real08;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline 
void 
Matrix3::setXYZRotation(const Degree& _x, const Degree& _y, const Degree& _z)
{
    const Radian x(_x);
    const Radian y(_y);
    const Radian z(_z);

    setXYZRotation(x, y, z);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline 
Real* 
Matrix3::data()
{
    return m_array;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline 
const Real&
Matrix3::getElement(int _row, int _col) const
{
    return m_matrix[_row][_col];
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline 
Vector3
Matrix3::getForwardVector(void)
{
    return Vector3(m_array[2], m_array[5], m_array[8]);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline 
Vector3
Matrix3::getUpVector(void)
{
    return Vector3(m_array[1], m_array[4], m_array[7]);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline 
Vector3
Matrix3::getLeftVector(void)
{
    return Vector3(m_array[0], m_array[3], m_array[6]);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
void
Matrix3::transpose(void)
{
    //http://www.songho.ca/opengl/gl_transform.html#matrix (for use with systems that use different column or row major setups)
    Real temp;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            temp = m_matrix[i][j];
            m_matrix[i][j] = m_matrix[j][i];
            m_matrix[j][i] = temp;
        }
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
void
Matrix3::serialize(boost::archive::polymorphic_iarchive& _archive, const int _version)
{
    _archive & boost::serialization::make_nvp("00",m_matrix[0][0]);
    _archive & boost::serialization::make_nvp("01",m_matrix[0][1]);
    _archive & boost::serialization::make_nvp("02",m_matrix[0][2]);
    _archive & boost::serialization::make_nvp("10",m_matrix[1][0]);
    _archive & boost::serialization::make_nvp("11",m_matrix[1][1]);
    _archive & boost::serialization::make_nvp("12",m_matrix[1][2]);
    _archive & boost::serialization::make_nvp("20",m_matrix[2][0]);
    _archive & boost::serialization::make_nvp("21",m_matrix[2][1]);
    _archive & boost::serialization::make_nvp("22",m_matrix[2][2]);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
void
Matrix3::serialize(boost::archive::polymorphic_oarchive& _archive, const int _version)
{
    _archive & boost::serialization::make_nvp("00",m_matrix[0][0]);
    _archive & boost::serialization::make_nvp("01",m_matrix[0][1]);
    _archive & boost::serialization::make_nvp("02",m_matrix[0][2]);
    _archive & boost::serialization::make_nvp("10",m_matrix[1][0]);
    _archive & boost::serialization::make_nvp("11",m_matrix[1][1]);
    _archive & boost::serialization::make_nvp("12",m_matrix[1][2]);
    _archive & boost::serialization::make_nvp("20",m_matrix[2][0]);
    _archive & boost::serialization::make_nvp("21",m_matrix[2][1]);
    _archive & boost::serialization::make_nvp("22",m_matrix[2][2]);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Math
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

BOOST_CLASS_IMPLEMENTATION(Zen::Math::Matrix3, boost::serialization::object_serializable);
BOOST_CLASS_TRACKING(Zen::Math::Matrix3, boost::serialization::track_never);

#endif // ZEN_MATH_MATRIX3_HPP_INCLUDED
