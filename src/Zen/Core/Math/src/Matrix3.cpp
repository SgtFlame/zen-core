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

#include "../Matrix3.hpp"

#include <math.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Math {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Matrix3&(*Matrix3::multiply)(Matrix3&, const Matrix3&);
Vector3&(*Matrix3::vectorMultiply)(const Matrix3&, Vector3&);
Matrix3&(*Matrix3::copy)(Matrix3&, const Matrix3&);

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Matrix3 Matrix_Zero
(
    0.0, 0.0, 0.0,
    0.0, 0.0, 0.0,
    0.0, 0.0, 0.0
);

const Matrix3 Matrix_Identity
(
    1.0, 0.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 0.0, 1.0
);

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Matrix3::Matrix3(init_type _init)
{
    switch(_init)
    {
    case INIT_NONE:
        return;
    case INIT_IDENTITY:
        memcpy(&m_array, &Matrix_Identity.m_array, sizeof(m_array));
        break;
    case INIT_ZERO:        
        memcpy(&m_array, &Matrix_Zero.m_array, sizeof(m_array));
        break;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Matrix3::~Matrix3()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Matrix3::identity()
{
    memcpy(&m_array, &Matrix_Identity.m_array, sizeof(m_array));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Matrix3::setXYZRotation(const Radian& _x, const Radian& _y, const Radian& _z)
{
    Real sinX, cosX;
    Real sinY, cosY;
    Real sinZ, cosZ;

    sinX = sinCos(_x, &cosX);
    sinY = sinCos(_y, &cosY);
    sinZ = sinCos(_z, &cosZ);

    m_array[0] = cosY * cosZ;
    m_array[1] = -cosY * sinZ;
    m_array[2] = -cosX * cosZ * sinY + sinX * sinZ;

    m_array[3] = -cosY * sinZ;
    m_array[4] = cosX * cosZ - sinX * sinY * sinZ;
    m_array[5] = cosZ * sinX + cosX * sinY + sinZ;

    m_array[6] = sinY;
    m_array[7] = -cosY * sinX;
    m_array[8] = cosX * cosY;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Matrix3::setXRotation(const Radian& _x)
{
    Real sinX, cosX;

    sinX = sinCos(_x, &cosX);

    m_array[0] = 1.0f;
    m_array[4] = m_array[8] = cosX;
    m_array[5] = sinX;
    m_array[7] = -sinX;

    m_array[1] = m_array[2] = m_array[3] = m_array[6] = 0.0f;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Matrix3::setYRotation(const Radian& _y)
{
    Real sinY, cosY;

    sinY = sinCos(_y, &cosY);

    m_array[0] = m_array[8] = cosY;
    m_array[2] = -sinY;
    m_array[4] = 1.0f;
    m_array[6] = sinY;

    m_array[1] = m_array[3] = m_array[5] = m_array[7] = 0.0f;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Matrix3::setZRotation(const Radian& _z)
{
    Real sinZ, cosZ;

    sinZ = sinCos(_z, &cosZ);

    m_array[0] = m_array[4] = cosZ;
    m_array[1] = sinZ;
    m_array[3] = -sinZ;

    m_array[8] = 1.0f;

    m_array[2] = m_array[5] = m_array[6] = m_array[7] = 0.0f;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Math
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
