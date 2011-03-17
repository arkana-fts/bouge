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
#ifndef BOUGE_QUATERNION_H
#define BOUGE_QUATERNION_H

#include <bouge/Config.hpp>

#include <string>
#include <vector>

namespace bouge {
    class Vector;

class BOUGE_API Quaternion {
public:
    ////////////////////////////////////////////
    // Constructors and assignment operators. //
    ////////////////////////////////////////////

    /// Creates a quaternion with all components set to 0
    Quaternion();
    /// Creates a quaternion based on the contents of a float array.
    /// \param in_q The four components of the quaternion.
    Quaternion(float in_q[4]);
    /// Creates a quaternion.
    /// \param in_fX The value of the first component of the quaternion.
    /// \param in_fY The value of the second component of the quaternion.
    /// \param in_fZ The value of the third component of the quaternion.
    /// \param in_fW The value of the fourth component of the quaternion.
    Quaternion(float in_fX, float in_fY, float in_fZ, float in_fW = 1.0f);
    /// Creates a quaternion using the data from a stl vector.
    Quaternion(const std::vector<float>& in_q);
    /// Creates a quaternion using the floats coming out of an iterator.
    /// \param in_begin The iterator producing the floats we want.
    /// \param in_end An iterator pointing to one element past the last we can use.
    template<class FloatIterator>
    Quaternion(FloatIterator in_begin, const FloatIterator& in_end);
    /// Copies a quaternion.
    /// \param in_q The quaternion to be copied.
    Quaternion(const Quaternion& in_q);
    /// Copies a quaternion.
    /// \param in_q The quaternion to be copied.
    /// \return a const reference to myself that might be used as a rvalue.
    const Quaternion& operator=(const Quaternion& in_q);
#ifdef BOUGE_COMPILE_CPP0X
    /// Moves a quaternion.
    /// \param in_q The quaternion to be moved.
    Quaternion(Quaternion&& in_q);
    /// Moves a quaternion.
    /// \param in_q The quaternion to be moved.
    /// \return a const reference to myself that might be used as a rvalue.
    const Quaternion& operator=(Quaternion&& in_q);
#endif // BOUGE_COMPILE_CPP0X
    ~Quaternion();

    //////////////////////////////////////
    // Special Quaternion constructors. //
    //////////////////////////////////////

    /// Creates a quaternion that represents a rotation of \a in_fPhi radians
    /// about an arbitrary axis going from the origin to the point (\a in_fX,
    /// \a in_fY, \a in_fZ).
    /// \param in_fX The x-coordinate of the endpoint of the rotation axis.
    /// \param in_fY The y-coordinate of the endpoint of the rotation axis.
    /// \param in_fZ The z-coordinate of the endpoint of the rotation axis.
    /// \param in_fRadians The angle of rotation, in \e radians.
    static Quaternion rotation(float in_fX, float in_fY, float in_fZ, float in_fRadians);
    /// Creates a quaternion that represents a rotation of \a in_fPhi radians
    /// about an arbitrary axis going from the origin to the point \a in_v.
    /// \param in_v The other endpoint of the rotation axis.
    /// \param in_fRadians The angle of rotation, in \e radians.
    static Quaternion rotation(const Vector& in_v, float in_fRadians);

    ///////////////////////////////////////
    // Conversion methods and operators. //
    ///////////////////////////////////////

    /// \return A read-only array of four floats holding the values of the
    ///         four components of this quaternion.
    inline const float *array4f() const {return &m_q[0];};

    /// \return A string-representation of the quaternion.
    /// \param in_iDecimalPlaces The amount of numbers to print behind the dot.
    std::string to_s(unsigned int in_iDecimalPlaces = 2) const;
    /// \return A string-representation of the quaternion.
    operator std::string() const;

    /// \return The axis around which the rotation takes place.
    Vector axis() const;

    /// \return The angle of rotation.
    float angle() const;

    /////////////////////////////////////
    // Accessors, getters and setters. //
    /////////////////////////////////////

    /// \return The X coordinate of the quaternion.
    inline float x() const { return m_q[0]; };
    /// \return The Y coordinate of the quaternion.
    inline float y() const { return m_q[1]; };
    /// \return The Z coordinate of the quaternion.
    inline float z() const { return m_q[2]; };
    /// \return The W coordinate of the quaternion.
    inline float w() const { return m_q[3]; };
    /// \param in_fX The new X coordinate of the quaternion.
    inline Quaternion& x(float in_fX) { m_q[0] = in_fX; return *this; };
    /// \param in_fY The new Y coordinate of the quaternion.
    inline Quaternion& y(float in_fY) { m_q[1] = in_fY; return *this; };
    /// \param in_fZ The new Z coordinate of the quaternion.
    inline Quaternion& z(float in_fZ) { m_q[2] = in_fZ; return *this; };
    /// \param in_fW The new W coordinate of the quaternion.
    inline Quaternion& w(float in_fW) { m_q[3] = in_fW; return *this; };

    /// Access the elements of this quaternion.
    /// \param idx The index of the element of this quaternion. This may only be a
    ///            value between 0 and 3.
    /// \throws std::out_of_range if \a idx is >3.
    float& operator[](unsigned int idx);
    /// Access the elements of this quaternion.
    /// \param idx The index of the element of this quaternion. This may only be a
    ///            value between 0 and 3.
    /// \throws std::out_of_range if \a idx is >3.
    float operator[](unsigned int idx) const;

    ////////////////////////////////////
    // Basic Quaternion calculations. //
    ////////////////////////////////////

    /// \return A negated copy of this quaternion.
    Quaternion operator -() const;
    /// Adds two quaternions.
    /// \param in_q The quaternion to add to this quaternion.
    /// \returns the quaternion resulting from this + \a in_q
    /// \note This does NOT concatenate rotations! For that, see the multiplication operator.
    Quaternion operator +(const Quaternion& in_q) const;
    /// Subtracts two quaternion.
    /// \param in_q The quaternion to subtract from this quaternion.
    /// \returns the quaternion resulting from this - \a in_q
    /// \note This does NOT get the difference in rotations! For that, see the division operator.
    Quaternion operator -(const Quaternion& in_q) const;
    /// Creates a scaled quaternion.
    /// \param in_f The scaling factor.
    /// \returns the quaternion resulting from this * \a in_f (this multiplied component-wise by f).
    Quaternion operator *(float in_f) const;
    /// Creates a shrinked quaternion.
    /// \param in_f The shrinking factor.
    /// \returns the quaternion resulting from this / \a in_f (this divided component-wise by f).
    Quaternion operator /(float in_f) const;

    /// \param in_q The quaternion to add to this quaternion. The result is stored in this quaternion.
    void operator +=(const Quaternion & in_q);
    /// \param in_q The quaternion to subtract from this quaternion. The result is stored in this quaternion.
    void operator -=(const Quaternion & in_q);
    /// \param in_f The factor to scale this quaternion. The result is stored in this quaternion.
    void operator *=(float in_f);

    /// Calculate the product of two quaternions. This concatenates their rotation.
    /// Note that the order matters, just as with matrices.
    /// \param in_q The second quaternion of the product.
    /// \return The resulting quaternion from *this * \a in_q.
    Quaternion operator *(const Quaternion & in_q) const;
    /// Calculate the quotient of this divided by \a in_q. This is the same as
    /// multiplying this by the inverse of \a in_q, which actually means
    /// "the difference of the rotations".
    /// \param in_q The denominator. (Lower part of the fraction.)
    /// \return The resulting quaternion from *this / \a in_q.
    Quaternion operator /(const Quaternion & in_q) const;
    /// Calculate the dot product of two quaternions.
    /// \param in_q The second quaternion of the dot product.
    /// \return The resulting quaternion from *this DOT \a in_q.
    float dot(const Quaternion & in_q) const;
    /// Calculate the conjugate of this quaternion. That is the first three components negated.
    /// i.e. The rotation axis is inverted and thus it rotates the other way around.
    /// \return The conjugate of this quaternion.
    Quaternion cnj() const;
    /// Calculate the inverse of this quaternion. This is the conjugate, but with
    /// length 1/L where L is the length of this. Thus for unit quaternions
    /// it is the same as the conjugate.
    Quaternion inv() const;

    /// Calculate the product of two quaternions. This concatenates their rotation.
    /// Note that the order matters, just as with matrices.
    /// \param in_q The second quaternion of the product.
    /// \return A reference to this.
    const Quaternion& operator *=(const Quaternion & in_q);
    /// Calculate the quotient of this divided by \a in_q. This is the same as
    /// multiplying this by the inverse of \a in_q.
    /// \param in_q The denominator. (Lower part of the fraction.)
    /// \return A reference to this.
    const Quaternion& operator /=(const Quaternion & in_q);

    ///////////////////////////////////////////
    // Quaternion length related operations. //
    ///////////////////////////////////////////

    /// \return The length of this quaternion, using the euclides norm.
    float len() const;
    /// Normalizes this quaternion: makes it have unit length.
    /// \return a reference to *this
    Quaternion& normalize();
    /// \return A normalized copy of this quaternion. It has unit length.
    Quaternion normalized() const;

    //////////////////////////////////////////
    // Quaternion interpolation operations. //
    //////////////////////////////////////////

    /// Normalized Linear interpolation between this and v2
    /// \param v2 The other quaternion with which to interpolate.
    /// \param between The time of interpolation. 0.0f results in this, 1.0f results in \a v2.
    /// \return A quaternion resulting from the normalized linear interpolation of this and \a v2, at time \a between.
    ///         This is especially useful to interpolate softly between two rotation angles.
    /// \note nlerp travels along the curve with non-constant speed but it IS commutative
    ///       and it is FAST to compute.
    Quaternion nlerp(const Quaternion& v2, float between) const;

    /// Spherical Linear interpolation between this and v2
    /// \param v2 The other quaternion with which to interpolate.
    /// \param between The time of interpolation. 0.0f results in this, 1.0f results in \a v2.
    /// \return A quaternion resulting from the spherical linear interpolation of this and \a v2, at time \a between.
    ///         This is especially useful to interpolate softly between two rotation angles.
    /// \note Slerp travels along the curve with constant speed but it is NOT
    ///       commutative and it is SLOW. Prefer using nlerp. See this link to know why:
    ///       http://number-none.com/product/Understanding%20Slerp,%20Then%20Not%20Using%20It/
    Quaternion slerp(const Quaternion& v2, float between) const;

    ///////////////////////////////////////
    // Quaternion comparison operations. //
    ///////////////////////////////////////

    /// \return true if this is longer than \a in_q.
    inline bool operator >(const Quaternion &in_q) const {return this->len() > in_q.len();};
    /// \return true if this is shorter than \a in_q.
    inline bool operator <(const Quaternion &in_q) const {return this->len() < in_q.len();};
    /// \return true if this is longer or has the same length as \a in_q.
    inline bool operator >=(const Quaternion &in_q) const {return this->len() >= in_q.len();};
    /// \return true if this is shorter or has the same length as \a in_q.
    inline bool operator <=(const Quaternion &in_q) const {return this->len() <= in_q.len();};
    /// \return true if this is \e nearly the same as \a in_q.
    bool operator ==(const Quaternion &in_q) const;
    /// \return true if this is \e not \e nearly the same as \a in_q.
    inline bool operator !=(const Quaternion &in_q) const {return !this->operator==(in_q);};

    //////////////////////////////
    // Rotating by quaternions. //
    //////////////////////////////

    /// Rotates a vector by this quaternion.
    /// \param in_v The vector to rotate.
    /// \return A new vector that is the result of having rotated the given
    ///         vector by this quaternion. (ret = this * in_v * this.inv)
    Vector rotate(const Vector& in_v) const;

private:
    /// The four components of the quaternion.
    std::vector<float> m_q;
};

#include "Quaternion.inl"

} // namespace bouge

#endif // BOUGE_QUATERNION_H
