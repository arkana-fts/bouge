/**
 * \file Math.h
 * \author Pompei2
 * \date 2 February 2010
 * \brief This file contains some mathematical utility functions.
 **/
////////////////////////////////////////////////////////////
//
// Bouge - Modern and flexible skeletal animation library
// Copyright (C) 2010 Lucas Beyer (pompei2@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////
#ifndef BOUGE_MATHUTIL_H
#define BOUGE_MATHUTIL_H

#include <algorithm>
#include <cmath>

/// Threshold used for floating point comparisons. You may want to redefine it.
#ifndef D_BOUGE_EPSILON
#  define D_BOUGE_EPSILON 0.0001f
#endif // D_BOUGE_EPSILON

namespace bouge {
    // angles
    static const float pi = 3.141592f;
    static const float rad2deg = 57.29577f;
    static const float deg2rad = 0.01745329f;

/// Checks if a floating point value is nearly zero.
/// \param val The value to check if it is near zero.
/// \returns true if \a val is nearly zero.
inline bool nearZero(const float& val, const float& epsilon = D_BOUGE_EPSILON) {
    return ((val > 0.0f && val < epsilon)
         || (val < 0.0f && val > -epsilon)
         || (val == 0.0f));
}

/// Clamps a value between two limits, that is sets it to the limit if it is beyond it.
/// \param out_val The value to be clamped, will be modified!
/// \param in_min The minimum the value should have. If it is less than this, it will be set to this.
/// \param in_max The maximum the value should have. If it is more than this, it will be set to this.
/// \return The same as \a out_val after this function call.
template<class T>
inline T clamp(T& out_val, T in_min, T in_max) {
    return out_val = std::max(std::min(out_val, in_max), in_min);
}

/// Returns a value clamped between two limits, that is the limit if it is beyond it.
/// \param in_val The value to be clamped, will \e not be modified!
/// \param in_min The minimum the value should have. If it is less than this, this will be returned.
/// \param in_max The maximum the value should have. If it is more than this, this will be returned.
/// \return \a in_val clamped to the minimum and maximum.
template<class T>
inline T clamp(const T& in_val, T in_min, T in_max) {
    return std::max(std::min(in_val, in_max), in_min);
}

/// Perform a linear interpolation between two values.
/// \param in_from The starting value. If \a in_percent is zero, this will be returned.
/// \param in_to The ending value. If \a in_percent is one, this will be returned.
/// \param in_percent Where between \a in_from and \a in_to we are.
/// \return A value between \a in_from and \a in_to, depending on \a in_percent.
template<class T>
inline T lerp(T in_from, T in_to, float in_percent) {
    return in_from + (in_to - in_from) * in_percent;
}

/// \return the fraction part of full, that is only the part behind the comma,
///         like if the non-fractional part (in front of the comma) was set to 0.
template<class T>
inline T fract(T full) {
    return full - (T)((long)full);
}

/// Returns the next power of two value from \a in_i
/** Returns the next bigger power of two value from \a in_i
 *
 * \param in_i The value which to get the bigger power of 2 from.
 *
 * \return a power of two.
 *
 * \Note If \a in_i is 6, this function returns 8, because 8 is the next power of two.\n
 *       If \a in_i is 16, this function returns 16, because 16 is already a power of two.
 *
 * \author Pompei2
 */
template<typename T>
T power_of_two(T in_i)
{
    T iValue = 1;

    while(iValue < in_i)
        iValue <<= 1;

    return iValue;
}

} // namespace bouge

#endif // BOUGE_MATHUTIL_H
