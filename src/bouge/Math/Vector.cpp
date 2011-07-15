/**
 * \file vector.cpp
 * \author Pompei2
 * \date 1 February 2010
 * \brief This file contains the implementation of the vector class. This class defines a 3-component vector.
 **/

#include <bouge/Math/Vector.hpp>
#include <bouge/Math/Matrix.hpp>
#include <bouge/Math/Util.hpp>

#include <sstream>
#include <cmath>

namespace bouge {

////////////////////////////////////////////
// Constructors and assignment operators. //
////////////////////////////////////////////

Vector::Vector()
    : m_v(4, 0.0f)
{
    m_v[3] = 1.0f;
}

Vector::Vector(const float in_v[3])
    : m_v(4)
{
    m_v[0] = in_v[0];
    m_v[1] = in_v[1];
    m_v[2] = in_v[2];
    m_v[3] = 1.0f;
}

Vector::Vector(const Vector& in_v)
    : m_v(4)
{
    m_v[0] = in_v.x();
    m_v[1] = in_v.y();
    m_v[2] = in_v.z();
    m_v[3] = 1.0f;
}

const Vector& Vector::operator=(const Vector& in_v)
{
    m_v[0] = in_v.x();
    m_v[1] = in_v.y();
    m_v[2] = in_v.z();
    m_v[3] = 1.0f;

    return *this;
}

Vector::Vector(float in_fX, float in_fY, float in_fZ)
    : m_v(4)
{
    m_v[0] = in_fX;
    m_v[1] = in_fY;
    m_v[2] = in_fZ;
    m_v[3] = 1.0f;
}

Vector::Vector(float in_fX, float in_fY, float in_fZ, float in_fW)
    : m_v(4)
{
    m_v[0] = in_fX;
    m_v[1] = in_fY;
    m_v[2] = in_fZ;
    m_v[3] = in_fW;
}

Vector::Vector(const Vector& in_v, float in_fW)
    : m_v(4)
{
    m_v[0] = in_v.x();
    m_v[1] = in_v.y();
    m_v[2] = in_v.z();
    m_v[3] = in_fW;
}

Vector::Vector(const std::vector<float>& in_v)
    : m_v(4)
{
    for(std::vector<float>::size_type i = 0 ; i < 4 && i < in_v.size() ; ++i) {
        m_v[i] = in_v.at(i);
    }
}

#ifdef BOUGE_COMPILE_CPP0X
Vector::Vector(Vector&& in_v)
    : m_v(std::move(in_v.m_v))
{ }

const Vector& Vector::operator=(Vector&& in_v)
{
    m_v = std::move(in_v.m_v);
    return *this;
}
#endif // BOUGE_COMPILE_CPP0X

Vector::~Vector()
{ }

///////////////////////////////////////
// Conversion methods and operators. //
///////////////////////////////////////

std::string Vector::to_s(unsigned int in_iDecimalPlaces) const
{
    std::stringstream ss;
    ss.precision(in_iDecimalPlaces);
    ss.fill(' ');
    ss <<  "(" << this->x() << ", " << this->y() << ", " << this->z();

    // Only show the 4th component if it is not 1.
    if(!nearZero(m_v[3] - 1.0f))
        ss << ", " << m_v[3];

    ss << ")";
    return ss.str();
}

Vector::operator std::string() const
{
    return this->to_s();
}

/////////////////////////////////////
// Accessors, getters and setters. //
/////////////////////////////////////

float& Vector::operator[](unsigned int idx)
{
    return m_v[idx];
}

float Vector::operator[](unsigned int idx) const
{
    return m_v[idx];
}

////////////////////////////////
// Basic Vector calculations. //
////////////////////////////////

Vector Vector::operator -() const
{
    return Vector(-this->x(),
                  -this->y(),
                  -this->z());
}

Vector Vector::operator +(const Vector& in_v) const
{
    return Vector(this->x() + in_v.x(),
                  this->y() + in_v.y(),
                  this->z() + in_v.z());
}

Vector Vector::operator -(const Vector& in_v) const
{
    return (*this) + (-in_v);
}

Vector Vector::operator *(float in_f) const
{
    return Vector(this->x() * in_f,
                  this->y() * in_f,
                  this->z() * in_f);
}

Vector Vector::operator *(const Vector& in_v) const
{
    return Vector(this->x() * in_v.x(),
                  this->y() * in_v.y(),
                  this->z() * in_v.z());
}

void Vector::operator +=(const Vector& in_v)
{
    this->operator=((*this) + in_v);
}

void Vector::operator -=(const Vector& in_v)
{
    this->operator+=(-in_v);
}

void Vector::operator *=(float in_f)
{
    this->operator=((*this) * in_f);
}

Vector Vector::cross(const Vector& in_v) const
{
    return Vector(this->y()*in_v.z() - this->z()*in_v.y(),
                  this->z()*in_v.x() - this->x()*in_v.z(),
                  this->x()*in_v.y() - this->y()*in_v.x());
}

float Vector::dot(const Vector& in_v) const
{
    return this->x()*in_v.x()
         + this->y()*in_v.y()
         + this->z()*in_v.z();
}

///////////////////////////////////////
// Vector length related operations. //
///////////////////////////////////////

float Vector::len() const
{
    return sqrt(this->x()*this->x()
              + this->y()*this->y()
              + this->z()*this->z());
}

Vector& Vector::normalize()
{
    // The zero-vector stays the zero-vector.
    if(nearZero(this->x()) &&
       nearZero(this->y()) &&
       nearZero(this->z()) ) {
        return this->x(0.0f).y(0.0f).z(0.0f);
    }

    float l = this->len();

    // Very little vectors will be stretched to a unit vector in one direction.
    if(nearZero(l)) {
        if((this->x() >= this->y())
        && (this->x() >= this->z())
        && (this->x() >= 0.0f)) {
            return this->x(1.0f).y(0.0f).z(0.0f);
        } else if((this->x() <= this->y())
               && (this->x() <= this->z())
               && (this->x() <= 0.0f)) {
            return this->x(-1.0f).y(0.0f).z(0.0f);
        } else {
            if(this->y() >= this->z()
            && this->y() >= 0.0f) {
                return this->x(0.0f).y(1.0f).z(0.0f);
            } else if(this->y() <= this->z()
                   && this->y() <= 0.0f) {
                return this->x(0.0f).y(-1.0f).z(0.0f);
            } else {
                return this->x(0.0f).y(0.0f).z(this->z() >= 0.0f ? 1.0f : -1.0f);
            }
        }
    } else {
        // Follows the usual normalization rule.
        float m = 1.0f / l;
        return this->x(this->x()*m).y(this->y()*m).z(this->z()*m);
    }
}

Vector Vector::normalized() const
{
    Vector copy(*this);
    return copy.normalize();
}

Vector Vector::abs() const
{
    return Vector(std::abs(this->x()), std::abs(this->y()), std::abs(this->z()), std::abs((*this)[3]));
}

Vector& Vector::cleanup()
{
    if(nearZero(fract(this->x())))
        this->x(std::floor(this->x()));
    else if(nearZero(fract(this->x()) - 1.0f))
        this->x(std::ceil(this->x()));

    if(nearZero(fract(this->y())))
        this->y(std::floor(this->y()));
    else if(nearZero(fract(this->y()) - 1.0f))
        this->y(std::ceil(this->y()));

    if(nearZero(fract(this->z())))
        this->z(std::floor(this->z()));
    else if(nearZero(fract(this->z()) - 1.0f))
        this->z(std::ceil(this->z()));

    return *this;
}

Vector Vector::cleanedup() const
{
    Vector copy(*this);
    return copy.cleanup();
}

//////////////////////////////////////
// Vector interpolation operations. //
//////////////////////////////////////

Vector Vector::lerp(const Vector& v2, float between) const
{
    return *this + (v2 - *this)*between;
}

///////////////////////////////////
// Vector comparison operations. //
///////////////////////////////////
bool Vector::operator ==(const Vector& in_v) const
{
    Vector diff = *this - in_v;
    return nearZero(diff.x()) && nearZero(diff.y()) && nearZero(diff.z());
}

////////////////////////////
// Vector transformation. //
////////////////////////////

Vector operator*(const Base4x4Matrix& m, const Vector& v)
{
    return Vector(m[0]*v[0] + m[4]*v[1] + m[8] *v[2] + m[12]*v[3],
                  m[1]*v[0] + m[5]*v[1] + m[9] *v[2] + m[13]*v[3],
                  m[2]*v[0] + m[6]*v[1] + m[10]*v[2] + m[14]*v[3],
                  m[3]*v[0] + m[7]*v[1] + m[11]*v[2] + m[15]*v[3]);
}

} // namespace bouge