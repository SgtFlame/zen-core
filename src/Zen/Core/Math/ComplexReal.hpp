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
#ifndef ZEN_MATH_COMPLEX_REAL_HPP_INCLUDED
#define ZEN_MATH_COMPLEX_REAL_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Math/Math.hpp>

#include <boost/archive/polymorphic_iarchive.hpp>
#include <boost/archive/polymorphic_oarchive.hpp>
#include <boost/serialization/nvp.hpp>

#include <math.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Math {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class MATH_DLL_LINK ComplexReal
{
    /// @name ComplexReal implementation
    /// @{
public:
    static Real magnitude(const ComplexReal& _cr);
    static Real magnitudeSquared(const ComplexReal& _cr);
    static Real phase(const ComplexReal& _cr);
    static ComplexReal conjugate(const ComplexReal& _cr);
    /// @}

    /// @name Operators
    /// @{
public:
    ComplexReal& operator=(const ComplexReal& _right);

    ComplexReal operator*(const Real _right) const;
    ComplexReal operator*(const ComplexReal& _right) const;
    ComplexReal& operator*=(const Real _right);
    ComplexReal& operator*=(const ComplexReal& _right);

    ComplexReal operator/(const Real _right) const;
    ComplexReal operator/(const ComplexReal& _right) const;
    ComplexReal& operator/=(const Real _right);
    ComplexReal& operator/=(const ComplexReal& _right);

    ComplexReal operator+(const Real _right) const;
    ComplexReal operator+(const ComplexReal& _right) const;
    ComplexReal& operator+=(const Real _right);
    ComplexReal& operator+=(const ComplexReal& _right);

    ComplexReal operator-() const;
    ComplexReal operator-(const Real _right) const;
    ComplexReal operator-(const ComplexReal& _right) const;
    ComplexReal& operator-=(const Real _right);
    ComplexReal& operator-=(const ComplexReal& _right);

    bool operator==(const ComplexReal& _right) const;
    bool operator!=(const ComplexReal& _right) const;
    bool operator==(const Real _right) const;
    bool operator!=(const Real _right) const;
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
             /// This constructor DOES NOT initialize anything!
             ComplexReal();
             /// Initialize the vector to the specified (real + j*imag)
             ComplexReal(Real _real, Real _imag);
             ComplexReal(const ComplexReal& _right);
            ~ComplexReal();
    /// @}

    /// @name Member variables
    /// @{
public:
    /// This is public so that anything can manipulate it
    union
    {
        Real        m_array[2];
        struct
        {
            Real    m_real;
            Real    m_imag;
        };
    };
};  // class ComplexReal

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Inlines
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
ComplexReal&
ComplexReal::operator=(const ComplexReal& _right)
{
    m_real = _right.m_real;
    m_imag = _right.m_imag;

    return *this;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
ComplexReal::ComplexReal()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
ComplexReal::ComplexReal(Real _real, Real _imag)
:   m_real(_real)
,   m_imag(_imag)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
ComplexReal::ComplexReal(const ComplexReal& _right)
:   m_real(_right.m_real)
,   m_imag(_right.m_imag)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
ComplexReal::~ComplexReal()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Real
ComplexReal::magnitude(const ComplexReal& _cr)
{
    return sqrt(magnitudeSquared(_cr));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Real
ComplexReal::magnitudeSquared(const ComplexReal& _cr)
{
    return _cr.m_real * _cr.m_real + _cr.m_imag * _cr.m_imag;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Real
ComplexReal::phase(const ComplexReal& _cr)
{
    return atan2(_cr.m_imag, _cr.m_real);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
ComplexReal
ComplexReal::conjugate(const ComplexReal& _cr)
{
    return ComplexReal(_cr.m_real, -_cr.m_imag);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
ComplexReal
ComplexReal::operator*(const Real _right) const
{
    return ComplexReal(this->m_real * _right, this->m_imag * _right);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Symmetric operator*()
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
ComplexReal
operator*(const Real _left, const ComplexReal& _right)
{
    return _right * _left;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
ComplexReal
ComplexReal::operator*(const ComplexReal& _right) const
{
    return ComplexReal(this->m_real * _right.m_real - this->m_imag * _right.m_imag,
                       this->m_real * _right.m_imag + this->m_imag * _right.m_real);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
ComplexReal&
ComplexReal::operator*=(const Real _right)
{
    this->m_real *= _right;
    this->m_imag *= _right;

    return *this;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
ComplexReal&
ComplexReal::operator*=(const ComplexReal& _right)
{
    Real leftReal = this->m_real;
    Real leftImag = this->m_imag;

    this->m_real = leftReal * _right.m_real - leftImag * _right.m_imag;
    this->m_imag = leftReal * _right.m_imag + leftImag * _right.m_real;

    return *this;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
ComplexReal
ComplexReal::operator/(const Real _right) const
{
    Real oneOver = Real( 1.0 / _right );

    return ComplexReal( this->m_real * oneOver, this->m_imag * oneOver );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Symmetric operator/()
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
ComplexReal
operator/(const Real _left, const ComplexReal& _right)
{
    return ComplexReal(_left, 0.0) / _right;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
ComplexReal
ComplexReal::operator/(const ComplexReal& _right) const
{
    Real oneOver = (Real)( 1.0 / ( _right.m_real * _right.m_real + _right.m_imag * _right.m_imag ) );

    return ComplexReal( (this->m_real * _right.m_real + this->m_imag * _right.m_imag) * oneOver,
                        (this->m_imag * _right.m_real - this->m_real * _right.m_imag) * oneOver );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
ComplexReal&
ComplexReal::operator/=(const Real _right)
{
    this->m_real /= _right;
    this->m_imag /= _right;

    return *this;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
ComplexReal&
ComplexReal::operator/=(const ComplexReal& _right)
{
    Real leftReal = this->m_real;
    Real leftImag = this->m_imag;
    Real oneOver = (Real)( 1.0 / ( _right.m_real * _right.m_real + _right.m_imag * _right.m_imag ) );

    this->m_real = (leftReal * _right.m_real + leftImag * _right.m_imag) * oneOver;
    this->m_imag = (leftImag * _right.m_real - leftReal * _right.m_imag) * oneOver;

    return *this;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
ComplexReal
ComplexReal::operator+(const Real _right) const
{
    return ComplexReal( this->m_real + _right, this->m_imag );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Symmetric operator+()
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
ComplexReal
operator+(const Real _left, const ComplexReal& _right)
{
    return _right + _left;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
ComplexReal
ComplexReal::operator+(const ComplexReal& _right) const
{
    return ComplexReal( this->m_real + _right.m_real,
                        this->m_imag + _right.m_imag );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
ComplexReal&
ComplexReal::operator+=(const Real _right)
{
    this->m_real += _right;

    return *this;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
ComplexReal&
ComplexReal::operator+=(const ComplexReal& _right)
{
    this->m_real += _right.m_real;
    this->m_imag += _right.m_imag;

    return *this;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
ComplexReal
ComplexReal::operator-() const
{
    return ComplexReal( -this->m_real, -this->m_imag );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
ComplexReal
ComplexReal::operator-(const Real _right) const
{
    return ComplexReal( this->m_real - _right, this->m_imag );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Symmetric operator-()
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
ComplexReal
operator-(const Real _left, const ComplexReal& _right)
{
    return ComplexReal( _left - _right.m_real, -_right.m_imag );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
ComplexReal
ComplexReal::operator-(const ComplexReal& _right) const
{
    return ComplexReal( this->m_real - _right.m_real,
                        this->m_imag - _right.m_imag );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
ComplexReal&
ComplexReal::operator-=(const Real _right)
{
    this->m_real -= _right;

    return *this;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
ComplexReal&
ComplexReal::operator-=(const ComplexReal& _right)
{
    this->m_real -= _right.m_real;
    this->m_imag -= _right.m_imag;

    return *this;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
bool
ComplexReal::operator==(const ComplexReal& _right) const
{
    return ( m_real == _right.m_real) && (m_imag == _right.m_imag);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
bool
ComplexReal::operator!=(const ComplexReal& _right) const
{
    return !(*this == _right);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
bool
ComplexReal::operator==(const Real _right) const
{
    return (m_imag == 0.0f) && (m_real == _right);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
bool
ComplexReal::operator!=(const Real _right) const
{
    return !(*this == _right);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
void
ComplexReal::serialize(boost::archive::polymorphic_iarchive& _archive, const int _version)
{
    _archive & boost::serialization::make_nvp("real",m_real);
    _archive & boost::serialization::make_nvp("imag",m_imag);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
void
ComplexReal::serialize(boost::archive::polymorphic_oarchive& _archive, const int _version)
{
    _archive & boost::serialization::make_nvp("real",m_real);
    _archive & boost::serialization::make_nvp("imag",m_imag);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Math
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

BOOST_CLASS_IMPLEMENTATION(Zen::Math::ComplexReal, boost::serialization::object_serializable);
BOOST_CLASS_TRACKING(Zen::Math::ComplexReal, boost::serialization::track_never);

#endif // ZEN_MATH_COMPLEX_REAL_HPP_INCLUDED
