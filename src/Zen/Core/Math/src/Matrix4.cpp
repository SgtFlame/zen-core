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

#include "../Matrix4.hpp"
#include <Zen/Core/Math/Quaternion4.hpp>

#include <math.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Math {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

Matrix4::Matrix4(const Quaternion4& _q)
{
    // this constructor derived by Arcanor from the following website:
    //   http:/www.j3d.org/matrix_faq/matrfaq_latest.html#Q54
    Real xx = _q.m_x * _q.m_x;
    Real xy = _q.m_x * _q.m_y;
    Real xz = _q.m_x * _q.m_z;
    Real xw = _q.m_x * _q.m_w;
    Real yy = _q.m_y * _q.m_y;
    Real yz = _q.m_y * _q.m_z;
    Real yw = _q.m_y * _q.m_w;
    Real zz = _q.m_z * _q.m_z;
    Real zw = _q.m_z * _q.m_w;
    m_matrix[0][0] = 1 - 2 * ( yy + zz );
    m_matrix[0][1] =     2 * ( xy - zw );
    m_matrix[0][2] =     2 * ( xz + yw );
    m_matrix[0][3] = 0;
    m_matrix[1][0] =     2 * ( xy + zw );
    m_matrix[1][1] = 1 - 2 * ( xx + zz );
    m_matrix[1][2] =     2 * ( yz - xw );
    m_matrix[1][3] = 0;
    m_matrix[2][0] =     2 * ( xz - yw );
    m_matrix[2][1] =     2 * ( yz + xw );
    m_matrix[2][2] = 1 - 2 * ( xx + yy );
    m_matrix[2][3] = 0;
    m_matrix[3][0] = 0;
    m_matrix[3][1] = 0;
    m_matrix[3][2] = 0;
    m_matrix[3][3] = 1;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Matrix4&(*Matrix4::multiply)(Matrix4&, const Matrix4&);
Matrix4&(*Matrix4::copy)(Matrix4&, const Matrix4&);

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Matrix4 Matrix_Zero
(
    0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0
);

const Matrix4 Matrix_Identity
(
    1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0
);

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Matrix4::Matrix4(init_type _init)
{
    switch(_init)
    {
    case INIT_NONE:
        return;
    case INIT_IDENTITY:
        memcpy(m_array, Matrix_Identity.m_array, sizeof(m_array));
        break;
    case INIT_ZERO:        
        memcpy(m_array, Matrix_Zero.m_array, sizeof(m_array));
        break;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Matrix4::~Matrix4()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Matrix4::identity()
{
    memcpy(m_array, Matrix_Identity.m_array, sizeof(m_array));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Matrix4::setXYZRotation(const Radian& _x, const Radian& _y, const Radian& _z)
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

    m_array[4] = -cosY * sinZ;
    m_array[5] = cosX * cosZ - sinX * sinY * sinZ;
    m_array[6] = cosZ * sinX + cosX * sinY + sinZ;

    m_array[8] = sinY;
    m_array[9] = -cosY * sinX;
    m_array[10] = cosX * cosY;

    m_array[15] = 1.0f;

    m_array[3] = m_array[7] 
        = m_array[11] = m_array[12] = m_array[13] = m_array[14] = 0.0f;

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Matrix4::setXRotation(const Radian& _x)
{
    Real sinX, cosX;

    sinX = sinCos(_x, &cosX);

    m_array[0] = m_array[15] = 1.0f;
    m_array[5] = cosX;
    m_array[6] = sinX;
    m_array[9] = -sinX;
    m_array[10] = cosX;

    m_array[1] = m_array[2] = m_array[3] = m_array[4] = m_array[7] = m_array[8] 
        = m_array[11] = m_array[12] = m_array[13] = m_array[14] = 0.0f;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Matrix4::setYRotation(const Radian& _y)
{
    Real sinY, cosY;

    sinY = sinCos(_y, &cosY);

    m_array[0] = m_array[10] = cosY;
    m_array[2] = -sinY;
    m_array[5] = m_array[15] = 1.0f;
    m_array[8] = sinY;

    m_array[1] = m_array[3] = m_array[4] = m_array[6] = m_array[7] = m_array[9] 
        = m_array[11] = m_array[12] = m_array[13] = m_array[14] = 0.0f;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Matrix4::setZRotation(const Radian& _z)
{
    Real sinZ, cosZ;

    sinZ = sinCos(_z, &cosZ);

    m_array[0] = m_array[5] = cosZ;
    m_array[1] = sinZ;
    m_array[4] = -sinZ;

    m_array[10] = m_array[15] = 1.0f;

    m_array[2] = m_array[3] = m_array[6] = m_array[7] = m_array[8] = m_array[9]
        = m_array[11] = m_array[12] = m_array[13] = m_array[14] = 0.0f;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Math
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
