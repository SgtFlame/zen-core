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

#include "../Math.hpp"
#include "../Matrix4.hpp"
#include "../Matrix3.hpp"
#include "../Vector4.hpp"

#include <math.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Math {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Real Math::PI                 = atan(1.0f) * 4.0f;
const Real Math::TWO_PI             = 2.0f * PI;
const Real Math::DegreesToRadians   = PI / 180.0f;
const Real Math::RadiansToDegrees   = 180.0f / PI;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifdef USE_FSINCOS
inline Real __fastcall sinCos(Real _radian, Real* _pCosRet)
{
    __asm
    {
        fld     dword ptr[_radian]
        fsincos
        fstp    dword ptr[_pCosRet]
    }
}
#else
Real sinCos(Real _radian, Real* _pCosRet)
{
    *_pCosRet = cos(_radian);
    return sin(_radian);
}
#endif

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static
Zen::Math::Matrix4&
cppMatrixMultiply(Zen::Math::Matrix4& _left, const Zen::Math::Matrix4& _right)
{
    Zen::Math::Real tmp[16];
    memcpy(tmp, _left.m_array, sizeof(Zen::Math::Real)*16);
    
    _left.m_array[0] = tmp[0]*_right.m_array[0] + tmp[1]*_right.m_array[4] + tmp[2]*_right.m_array[8]  + tmp[3]*_right.m_array[12];
    _left.m_array[1] = tmp[0]*_right.m_array[1] + tmp[1]*_right.m_array[5] + tmp[2]*_right.m_array[9]  + tmp[3]*_right.m_array[13];
    _left.m_array[2] = tmp[0]*_right.m_array[2] + tmp[1]*_right.m_array[6] + tmp[2]*_right.m_array[10] + tmp[3]*_right.m_array[14];
    _left.m_array[3] = tmp[0]*_right.m_array[3] + tmp[1]*_right.m_array[7] + tmp[2]*_right.m_array[11] + tmp[3]*_right.m_array[15];

    _left.m_array[4] = tmp[4]*_right.m_array[0] + tmp[5]*_right.m_array[4] + tmp[6]*_right.m_array[8]  + tmp[7]*_right.m_array[12];
    _left.m_array[5] = tmp[4]*_right.m_array[1] + tmp[5]*_right.m_array[5] + tmp[6]*_right.m_array[9]  + tmp[7]*_right.m_array[13];
    _left.m_array[6] = tmp[4]*_right.m_array[2] + tmp[5]*_right.m_array[6] + tmp[6]*_right.m_array[10] + tmp[7]*_right.m_array[14];
    _left.m_array[7] = tmp[4]*_right.m_array[3] + tmp[5]*_right.m_array[7] + tmp[6]*_right.m_array[11] + tmp[7]*_right.m_array[15];

    _left.m_array[8] = tmp[8]*_right.m_array[0] + tmp[9]*_right.m_array[4] + tmp[10]*_right.m_array[8] + tmp[11]*_right.m_array[12];
    _left.m_array[9] = tmp[8]*_right.m_array[1] + tmp[9]*_right.m_array[5] + tmp[10]*_right.m_array[9] + tmp[11]*_right.m_array[13];
    _left.m_array[10]= tmp[8]*_right.m_array[2] + tmp[9]*_right.m_array[6] + tmp[10]*_right.m_array[10]+ tmp[11]*_right.m_array[14];
    _left.m_array[11]= tmp[8]*_right.m_array[3] + tmp[9]*_right.m_array[7] + tmp[10]*_right.m_array[11]+ tmp[11]*_right.m_array[15];

    _left.m_array[12]= tmp[12]*_right.m_array[0]+ tmp[13]*_right.m_array[4]+ tmp[14]*_right.m_array[8] + tmp[15]*_right.m_array[12];
    _left.m_array[13]= tmp[12]*_right.m_array[1]+ tmp[13]*_right.m_array[5]+ tmp[14]*_right.m_array[9] + tmp[15]*_right.m_array[13];
    _left.m_array[14]= tmp[12]*_right.m_array[2]+ tmp[13]*_right.m_array[6]+ tmp[14]*_right.m_array[10]+ tmp[15]*_right.m_array[14];
    _left.m_array[15]= tmp[12]*_right.m_array[3]+ tmp[13]*_right.m_array[7]+ tmp[14]*_right.m_array[11]+ tmp[15]*_right.m_array[15];

    return _left;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static
Zen::Math::Matrix3&
cppMatrixMultiply(Zen::Math::Matrix3& _left, const Zen::Math::Matrix3& _right)
{
    Zen::Math::Real tmp[9];
    memcpy(tmp, _left.m_array, sizeof(Zen::Math::Real)*9);
    
    _left.m_array[0] = tmp[0]*_right.m_array[0] + tmp[1]*_right.m_array[3] + tmp[2]*_right.m_array[6];
    _left.m_array[1] = tmp[0]*_right.m_array[1] + tmp[1]*_right.m_array[4] + tmp[2]*_right.m_array[7];
    _left.m_array[2] = tmp[0]*_right.m_array[2] + tmp[1]*_right.m_array[5] + tmp[2]*_right.m_array[8];

    _left.m_array[3] = tmp[3]*_right.m_array[0] + tmp[4]*_right.m_array[3] + tmp[5]*_right.m_array[6];
    _left.m_array[4] = tmp[3]*_right.m_array[1] + tmp[4]*_right.m_array[4] + tmp[5]*_right.m_array[7];
    _left.m_array[5] = tmp[3]*_right.m_array[2] + tmp[4]*_right.m_array[5] + tmp[5]*_right.m_array[8];

    _left.m_array[6] = tmp[6]*_right.m_array[0] + tmp[7]*_right.m_array[3] + tmp[8]*_right.m_array[6];
    _left.m_array[7] = tmp[6]*_right.m_array[1] + tmp[7]*_right.m_array[4] + tmp[8]*_right.m_array[7];
    _left.m_array[8] = tmp[6]*_right.m_array[2] + tmp[7]*_right.m_array[5] + tmp[8]*_right.m_array[8];

    return _left;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static
Zen::Math::Vector3&
cppMatrixVectorMultiply(const Zen::Math::Matrix3& _left, Zen::Math::Vector3& _right)
{
    Zen::Math::Real tmp[3];
    memcpy(tmp, _right.m_array, sizeof(Zen::Math::Real) * 3);

    _right.m_array[0] = _left.m_array[0]*tmp[0] + _left.m_array[1]*tmp[1] + _left.m_array[2]*tmp[2];
    _right.m_array[1] = _left.m_array[3]*tmp[0] + _left.m_array[4]*tmp[1] + _left.m_array[5]*tmp[2];
    _right.m_array[2] = _left.m_array[6]*tmp[0] + _left.m_array[7]*tmp[1] + _left.m_array[8]*tmp[2];

    return _right;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static
Zen::Math::Vector4&
cppVector4Matrix4Multiply(Zen::Math::Vector4& _left, const Zen::Math::Matrix4& _right)
{
    Zen::Math::Real tmp[4];
    memcpy(tmp, _left.m_array, sizeof(Zen::Math::Real) * 4);

    _left.m_array[0] = tmp[0]*_right.m_array[0] + tmp[1]*_right.m_array[4] + tmp[2]*_right.m_array[8]  + tmp[3]*_right.m_array[12];
    _left.m_array[1] = tmp[0]*_right.m_array[1] + tmp[1]*_right.m_array[5] + tmp[2]*_right.m_array[9]  + tmp[3]*_right.m_array[13];
    _left.m_array[2] = tmp[0]*_right.m_array[2] + tmp[1]*_right.m_array[6] + tmp[2]*_right.m_array[10] + tmp[3]*_right.m_array[14];
    _left.m_array[3] = tmp[0]*_right.m_array[3] + tmp[1]*_right.m_array[7] + tmp[2]*_right.m_array[11] + tmp[3]*_right.m_array[15];

    return _left;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static
Zen::Math::Matrix4&
cppMatrixCopy(Zen::Math::Matrix4& _left, const Zen::Math::Matrix4& _right)
{
    memcpy(_left.m_array, _right.m_array, sizeof(Zen::Math::Real) * 16);
    return _left;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static
Zen::Math::Matrix3&
cppMatrixCopy(Zen::Math::Matrix3& _left, const Zen::Math::Matrix3& _right)
{
    memcpy(_left.m_array, _right.m_array, sizeof(Zen::Math::Real) * 9);
    return _left;
}

class MathInit
{
public:
    MathInit()
    {
        // Install default math library
        Matrix4::multiply = cppMatrixMultiply;
        Matrix4::copy = cppMatrixCopy;
        Matrix3::multiply = cppMatrixMultiply;
        Matrix3::vectorMultiply = cppMatrixVectorMultiply;
        Matrix3::copy = cppMatrixCopy;
        Vector4::multiply = cppVector4Matrix4Multiply;
    }
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static MathInit;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Math
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
