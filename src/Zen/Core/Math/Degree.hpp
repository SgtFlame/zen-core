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
#ifndef ZEN_MATH_DEGREE_HPP_INCLUDED
#define ZEN_MATH_DEGREE_HPP_INCLUDED

#include <Zen/Core/Math/Math.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Math {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class Radian;

class MATH_DLL_LINK Degree
{
    /// @name Static helpers
    /// @{
public:
    static inline Real degreesToRadians(Real _degrees) { return _degrees * Math::DegreesToRadians; }
    /// @}

    /// @name Radian implementation
    /// @{
public:
    inline Real toRadians() const { return degreesToRadians(m_degree); }
    inline Real toDegrees() const { return m_degree; }
    /// @}

    /// @name operators
    /// @{
public:
    inline Degree& operator=(const Degree& _right) { m_degree = _right.m_degree; return *this; }
    inline Degree& operator=(const Real& _right) { m_degree = _right; return *this; }
    inline operator Real() const { return m_degree; }
    /// @}

    /// @name 'Structors
    /// @{
public:
    explicit Degree(Real _degree = 0) : m_degree(_degree) {}
             Degree(const Degree& _degree) : m_degree(_degree.m_degree) {}
             Degree(const Radian& _radian);
            ~Degree();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Real        m_degree;
    /// @}

};  // class Degree;

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Math
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_MATH_DEGREE_HPP_INCLUDED
