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
#ifndef ZEN_MATH_RADIAN_HPP_INCLUDED
#define ZEN_MATH_RADIAN_HPP_INCLUDED

#include <math.h>

#include <Zen/Core/Math/Math.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Math {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class Degree;

class MATH_DLL_LINK Radian
{
    /// @name Static helpers
    /// @{
public:
    static inline Real radiansToDegrees(Real _radians) { return _radians * Math::RadiansToDegrees; }
    /// @}

    /// @name Radian implementation
    /// @{
public:
    inline Real toDegrees() const { return radiansToDegrees(m_radian); }
    /// @}

    /// @name operators
    /// @{
public:
    inline Radian& operator=(const Radian& _right) { m_radian = _right.m_radian; return *this; }
    inline Radian& operator=(const Zen::Math::Real& _right) { m_radian = _right; return *this; }
    inline Radian& operator+=(const Radian& _right) { m_radian += _right.m_radian; m_radian = fmod(m_radian,Math::TWO_PI); return *this; }
    inline Radian& operator+=(const Zen::Math::Real& _right) { m_radian += _right; m_radian = fmod(m_radian,Math::TWO_PI); return *this; }
    inline operator Real() const { return m_radian; }
    /// @}

    /// @name 'Structors
    /// @{
public:
    explicit Radian(Real _radian = 0) : m_radian(_radian) {}
             Radian(const Radian& _radian) : m_radian(_radian.m_radian) {}
             Radian(const Degree& _degree);
            ~Radian();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Real        m_radian;
    /// @}

};  // class Radian;

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Math
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_MATH_RADIAN_HPP_INCLUDED
