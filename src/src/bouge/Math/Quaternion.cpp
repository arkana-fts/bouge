/**
 * \file quaternion.cpp
 * \author Pompei2
 * \date 2 February 2010
 * \brief This file contains the implementation of the quaternion class. This class defines a 4-component vector.
 **/

#include <bouge/Math/Quaternion.hpp>
#include <bouge/Math/Vector.hpp>
#include <bouge/Math/Util.hpp>

#include <sstream>
#include <cmath>

namespace bouge {

////////////////////////////////////////////
// Constructors and assignment operators. //
////////////////////////////////////////////

Quaternion::Quaternion()
    : m_q(4)
{
    m_q[0] = 0.0f;
    m_q[1] = 0.0f;
    m_q[2] = 0.0f;
    m_q[3] = 1.0f;
}

Quaternion::Quaternion(float in_v[4])
    : m_q(4)
{
    m_q[0] = in_v[0];
    m_q[1] = in_v[1];
    m_q[2] = in_v[2];
    m_q[3] = in_v[3];
}

Quaternion::Quaternion(const Quaternion& in_q)
    : m_q(4)
{
    m_q[0] = in_q.x();
    m_q[1] = in_q.y();
    m_q[2] = in_q.z();
    m_q[3] = in_q.w();
}

const Quaternion& Quaternion::operator=(const Quaternion& in_q)
{
    m_q[0] = in_q.x();
    m_q[1] = in_q.y();
    m_q[2] = in_q.z();
    m_q[3] = in_q.w();

    return *this;
}

Quaternion::Quaternion(float in_fX, float in_fY, float in_fZ, float in_fW)
    : m_q(4)
{
    m_q[0] = in_fX;
    m_q[1] = in_fY;
    m_q[2] = in_fZ;
    m_q[3] = in_fW;
}

#ifdef BOUGE_COMPILE_CPP0X
Quaternion::Quaternion(Quaternion&& in_q)
    : m_q(std::move(in_q.m_q))
{ }

const Quaternion& Quaternion::operator=(Quaternion&& in_q)
{
    m_q = std::move(in_q.m_q);
    return *this;
}
#endif // BOUGE_COMPILE_CPP0X

Quaternion::~Quaternion()
{ }

//////////////////////////////////////
// Special Quaternion constructors. //
//////////////////////////////////////

Quaternion Quaternion::rotation(float in_fX, float in_fY, float in_fZ, float in_fRadians)
{
    return Quaternion::rotation(Vector(in_fX, in_fY, in_fZ), in_fRadians);
}

Quaternion Quaternion::rotation(const Vector& in_v, float in_fRadians)
{
    float omega = 0.5f*in_fRadians;
    Vector v = sin(omega) * in_v.normalized();
    return Quaternion(v.x(), v.y(), v.z(), cos(omega));
}

///////////////////////////////////////
// Conversion methods and operators. //
///////////////////////////////////////

std::string Quaternion::to_s(unsigned int in_iDecimalPlaces) const
{
    std::stringstream ss;
    ss.precision(in_iDecimalPlaces);
    ss.fill(' ');
    ss <<  "(" << this->x() << ", " << this->y() << ", " << this->z() << ", " << this->w() << ")";
    return ss.str();
}

Quaternion::operator std::string() const
{
    return this->to_s();
}

Vector Quaternion::axis() const
{
/*
    float s = sqrt(1-this->w()*this->w());

    // If s is close to 0, it means a 0-degree rotation.
    // For a 0-degree rotation, the axis is arbitrary.
    if(nearZero(s)) {
        return Vector(1.0f, 0.0f, 0.0f);
    } else {
        float factor = 1.0f/s;
        return Vector(this->x()*factor,
                      this->y()*factor,
                      this->z()*factor).normalize();
    }
*/
    return Vector(this->x(), this->y(), this->z()).normalize();
}

float Quaternion::angle() const
{
    return 2.0f*acos(this->w());
}

/////////////////////////////////////
// Accessors, getters and setters. //
/////////////////////////////////////

float& Quaternion::operator[](unsigned int idx)
{
    return m_q[idx];
}

float Quaternion::operator[](unsigned int idx) const
{
    return m_q[idx];
}

////////////////////////////////////
// Basic Quaternion calculations. //
////////////////////////////////////

Quaternion Quaternion::operator -() const
{
    return Quaternion(-this->x(),
                      -this->y(),
                      -this->z(),
                      -this->w());
}

Quaternion Quaternion::operator +(const Quaternion& in_q) const
{
    return Quaternion(this->x() + in_q.x(),
                      this->y() + in_q.y(),
                      this->z() + in_q.z(),
                      this->w() + in_q.w());
}

Quaternion Quaternion::operator -(const Quaternion& in_q) const
{
    return (*this) + (-in_q);
}

Quaternion Quaternion::operator *(float in_f) const
{
    return Quaternion(this->x() * in_f,
                      this->y() * in_f,
                      this->z() * in_f,
                      this->w() * in_f);
}

Quaternion Quaternion::operator /(float in_f) const
{
    return Quaternion(this->x() / in_f,
                      this->y() / in_f,
                      this->z() / in_f,
                      this->w() / in_f);
}

void Quaternion::operator +=(const Quaternion& in_q)
{
    this->operator=((*this) + in_q);
}

void Quaternion::operator -=(const Quaternion& in_q)
{
    this->operator+=(-in_q);
}

void Quaternion::operator *=(float in_f)
{
    this->operator=((*this) * in_f);
}

Quaternion Quaternion::operator *(const Quaternion& in_q) const
{
    return Quaternion(this->w() * in_q.x() + this->x() * in_q.w() + this->y() * in_q.z() - this->z() * in_q.y(),
                      this->w() * in_q.y() + this->y() * in_q.w() + this->z() * in_q.x() - this->x() * in_q.z(),
                      this->w() * in_q.z() + this->z() * in_q.w() + this->x() * in_q.y() - this->y() * in_q.x(),
                      this->w() * in_q.w() - this->x() * in_q.x() - this->y() * in_q.y() - this->z() * in_q.z());
}

Quaternion Quaternion::operator /(const Quaternion & in_q) const
{
    return *this * in_q.inv();
}

float Quaternion::dot(const Quaternion& in_q) const
{
    return this->x() * in_q.x()
         + this->y() * in_q.y()
         + this->z() * in_q.z()
         + this->w() * in_q.w();
}

Quaternion Quaternion::cnj() const
{
    return Quaternion(-this->x(),
                      -this->y(),
                      -this->z(),
                       this->w());
}

Quaternion Quaternion::inv() const
{
    float l = this->len();
    return this->cnj()/(l*l);
}

const Quaternion& Quaternion::operator *=(const Quaternion & in_q)
{
    return this->operator=((*this) * in_q);
}

const Quaternion& Quaternion::operator /=(const Quaternion & in_q)
{
    return this->operator=((*this) / in_q);
}

///////////////////////////////////////////
// Quaternion length related operations. //
///////////////////////////////////////////

float Quaternion::len() const
{
    return sqrt(this->x()*this->x()
              + this->y()*this->y()
              + this->z()*this->z()
              + this->w()*this->w());
}

Quaternion& Quaternion::normalize()
{
    // The zero-quaternion stays the zero-quaternion.
    if(nearZero(this->x()) &&
       nearZero(this->y()) &&
       nearZero(this->z()) &&
       nearZero(this->w()) ) {
        return this->x(0.0f).y(0.0f).z(0.0f).w(0.0f);
    }

    float l = this->len();

    // Very little quaternion will be stretched to a unit quaternion in one direction.
    if(nearZero(l)) {
        if((this->x() >= this->y())
        && (this->x() >= this->z())
        && (this->x() >= this->w())
        && (this->x() >= 0.0f)) {
            return this->x(1.0f).y(0.0f).z(0.0f).w(0.0f);
        } else if((this->x() <= this->y())
               && (this->x() <= this->z())
               && (this->x() <= this->w())
               && (this->x() <= 0.0f)) {
            return this->x(-1.0f).y(0.0f).z(0.0f).w(0.0f);
        } else {
            if(this->y() >= this->z()
            && this->y() >= this->w()
            && this->y() >= 0.0f) {
                return this->x(0.0f).y(1.0f).z(0.0f).w(0.0f);
            } else if(this->y() <= this->z()
                   && this->y() <= this->w()
                   && this->y() <= 0.0f) {
                return this->x(0.0f).y(-1.0f).z(0.0f).w(0.0f);
            } else {
                if(this->z() >= this->w()
                && this->z() >= 0.0f) {
                    return this->x(0.0f).y(0.0f).z(1.0f).w(0.0f);
                } else if(this->z() <= this->w()
                       && this->z() <= 0.0f) {
                    return this->x(0.0f).y(0.0f).z(-1.0f).w(0.0f);
                } else {
                    return this->x(0.0f).y(0.0f).z(0.0f).w(this->w() >= 0.0f ? 1.0f : -1.0f);
                }
            }
        }
    } else {
        // Follows the usual normalization rule.
        float m = 1.0f / l;
        return this->x(this->x()*m).y(this->y()*m).z(this->z()*m).w(this->w()*m);
    }
}

Quaternion Quaternion::normalized() const
{
    Quaternion copy(*this);
    return copy.normalize();
}

//////////////////////////////////////////
// Quaternion interpolation operations. //
//////////////////////////////////////////
Quaternion Quaternion::nlerp(const Quaternion& q2, float between) const
{
    return (*this + (q2 - *this)*between).normalize();
}

Quaternion Quaternion::slerp(const Quaternion& q2, float between) const
{
    float cosTheta = this->dot(q2);
    cosTheta = std::min(cosTheta, 1.0f);
    cosTheta = std::max(cosTheta, -1.0f); // Clamp to [-1, 1] for the acos.
    float theta    = acos(cosTheta);
    float sinTheta = sin(theta);

    float w1, w2;

    if(nearZero(sinTheta)) {
        // Quaternions a and b are nearly the same, do linear interpolation.
        w1 = 1.0f - between;
        w2 = between;
    } else {
        w1 = float(sin((1.0f-between)*theta) / sinTheta);
        w2 = float(sin(between*theta) / sinTheta);
    }

    return ((*this)*w1 + q2*w2).normalize();
}

///////////////////////////////////////
// Quaternion comparison operations. //
///////////////////////////////////////
bool Quaternion::operator ==(const Quaternion &in_q) const
{
    Quaternion diff = *this - in_q;
    return nearZero(diff.x()) && nearZero(diff.y()) && nearZero(diff.z());
}

//////////////////////////////
// Rotating by quaternions. //
//////////////////////////////

Vector Quaternion::rotate(const bouge::Vector& in_v) const
{
    Quaternion vOrig = Quaternion(in_v.x(), in_v.y(), in_v.z(), 0.0f);
    Quaternion vRotated = *this * vOrig * this->cnj();
    return Vector(vRotated.x(), vRotated.y(), vRotated.z());
}

} // namespace bouge
