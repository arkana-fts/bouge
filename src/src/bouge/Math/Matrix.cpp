/**
 * \file quaternion.cpp
 * \author Pompei2
 * \date 2 February 2010
 * \brief This file contains the implementation of the quaternion class. This class defines a 4-component vector.
 **/

#include <bouge/Math/Matrix.hpp>
#include <bouge/Math/Quaternion.hpp>
#include <bouge/Math/Vector.hpp>
#include <bouge/Math/Util.hpp>

#include <sstream>
#include <cmath>

namespace bouge {

////////////////////////////////
////////////////////////////////
//// The Base 4 Matrix part ////
////////////////////////////////
////////////////////////////////

// That's what I mean in the header.
Base4x4Matrix::~Base4x4Matrix()
{ }

////////////////////////////////////////////
// Constructors and assignment operators. //
////////////////////////////////////////////

Base4x4Matrix::Base4x4Matrix()
    : m(16)
    , im(16)
{
    m[0] = 1.0f; m[4] = 0.0f; m[8]  = 0.0f; m[12] = 0.0f;
    m[1] = 0.0f; m[5] = 1.0f; m[9]  = 0.0f; m[13] = 0.0f;
    m[2] = 0.0f; m[6] = 0.0f; m[10] = 1.0f; m[14] = 0.0f;
    m[3] = 0.0f; m[7] = 0.0f; m[11] = 0.0f; m[15] = 1.0f;
    im[0] = 1.0f; im[4] = 0.0f; im[8]  = 0.0f; im[12] = 0.0f;
    im[1] = 0.0f; im[5] = 1.0f; im[9]  = 0.0f; im[13] = 0.0f;
    im[2] = 0.0f; im[6] = 0.0f; im[10] = 1.0f; im[14] = 0.0f;
    im[3] = 0.0f; im[7] = 0.0f; im[11] = 0.0f; im[15] = 1.0f;
}

///////////////////////////////////////
// Conversion methods and operators. //
///////////////////////////////////////

std::string Base4x4Matrix::to_s(unsigned int in_iDecimalPlaces, bool in_bOneLiner) const
{
    std::ostringstream ss;
    ss.precision(in_iDecimalPlaces);
    ss.fill(' ');

    if(in_bOneLiner) {
        ss <<  "(" << m[0] << ", " << m[4] << ", " << m[8]  << ", " << m[12] << "; "
                   << m[1] << ", " << m[5] << ", " << m[9]  << ", " << m[13] << "; "
                   << m[2] << ", " << m[6] << ", " << m[10] << ", " << m[14] << "; "
                   << m[3] << ", " << m[7] << ", " << m[11] << ", " << m[15] << ") "
            << "(" << im[0] << ", " << im[4] << ", " << im[8]  << ", " << im[12] << "; "
                   << im[1] << ", " << im[5] << ", " << im[9]  << ", " << im[13] << "; "
                   << im[2] << ", " << im[6] << ", " << im[10] << ", " << im[14] << "; "
                   << im[3] << ", " << im[7] << ", " << im[11] << ", " << im[15] << ") ";
    } else {
        ss <<  "/" << m[0] << " " << m[4] << " " << m[8]  << " " << m[12] << "\\ " "/" << im[0] << " " << im[4] << " " << im[8]  << " " << im[12] << "\\\n"
               "|" << m[1] << " " << m[5] << " " << m[9]  << " " << m[13] << "| "  "|" << im[1] << " " << im[5] << " " << im[9]  << " " << im[13] << "|\n"
               "|" << m[2] << " " << m[6] << " " << m[10] << " " << m[14] << "| "  "|" << im[2] << " " << im[6] << " " << im[10] << " " << im[14] << "|\n"
              "\\" << m[3] << " " << m[7] << " " << m[11] << " " << m[15] << "/ " "\\" << im[3] << " " << im[7] << " " << im[11] << " " << im[15] << "/";
    }

    return ss.str();
}

Base4x4Matrix::operator std::string() const
{
    return this->to_s();
}

/////////////////////////////////////
// Accessors, getters and setters. //
/////////////////////////////////////

float Base4x4Matrix::operator[](unsigned int idx) const
{
    return m[idx];
}

float Base4x4Matrix::operator()(unsigned int i, unsigned int j) const
{
    return m[4*(j-1)+(i-1)];
}

////////////////////////////////
////////////////////////////////
//// The Affine Matrix part ////
////////////////////////////////
////////////////////////////////

AffineMatrix::~AffineMatrix()
{ }

////////////////////////////////////////////
// Constructors and assignment operators. //
////////////////////////////////////////////

AffineMatrix::AffineMatrix()
    : Base4x4Matrix()
    , m3(9)
    , im3(9)
{
    m3[0] = m[0]; m3[3] = m[4]; m3[6] = m[8];
    m3[1] = m[1]; m3[4] = m[5]; m3[7] = m[9];
    m3[2] = m[2]; m3[5] = m[6]; m3[8] = m[10];
    im3[0] = im[0]; im3[3] = im[4]; im3[6] = im[8];
    im3[1] = im[1]; im3[4] = im[5]; im3[7] = im[9];
    im3[2] = im[2]; im3[5] = im[6]; im3[8] = im[10];
}

AffineMatrix::AffineMatrix(const AffineMatrix& in_m)
    : Base4x4Matrix()
    , m3(9)
    , im3(9)
{
    m[0] = in_m.m[0]; m[4] = in_m.m[4]; m[8]  = in_m.m[8];  m[12] = in_m.m[12];
    m[1] = in_m.m[1]; m[5] = in_m.m[5]; m[9]  = in_m.m[9];  m[13] = in_m.m[13];
    m[2] = in_m.m[2]; m[6] = in_m.m[6]; m[10] = in_m.m[10]; m[14] = in_m.m[14];
    m[3] = in_m.m[3]; m[7] = in_m.m[7]; m[11] = in_m.m[11]; m[15] = in_m.m[15];
    im[0] = in_m.im[0]; im[4] = in_m.im[4]; im[8]  = in_m.im[8];  im[12] = in_m.im[12];
    im[1] = in_m.im[1]; im[5] = in_m.im[5]; im[9]  = in_m.im[9];  im[13] = in_m.im[13];
    im[2] = in_m.im[2]; im[6] = in_m.im[6]; im[10] = in_m.im[10]; im[14] = in_m.im[14];
    im[3] = in_m.im[3]; im[7] = in_m.im[7]; im[11] = in_m.im[11]; im[15] = in_m.im[15];
    m3[0] = in_m.m3[0]; m3[3] = in_m.m3[3]; m3[6] = in_m.m3[6];
    m3[1] = in_m.m3[1]; m3[4] = in_m.m3[4]; m3[7] = in_m.m3[7];
    m3[2] = in_m.m3[2]; m3[5] = in_m.m3[5]; m3[8] = in_m.m3[8];
    im3[0] = in_m.im3[0]; im3[3] = in_m.im3[3]; im3[6] = in_m.im3[6];
    im3[1] = in_m.im3[1]; im3[4] = in_m.im3[4]; im3[7] = in_m.im3[7];
    im3[2] = in_m.im3[2]; im3[5] = in_m.im3[5]; im3[8] = in_m.im3[8];
}

const AffineMatrix& AffineMatrix::operator=(const AffineMatrix& in_m)
{
    m[0] = in_m.m[0]; m[4] = in_m.m[4]; m[8]  = in_m.m[8];  m[12] = in_m.m[12];
    m[1] = in_m.m[1]; m[5] = in_m.m[5]; m[9]  = in_m.m[9];  m[13] = in_m.m[13];
    m[2] = in_m.m[2]; m[6] = in_m.m[6]; m[10] = in_m.m[10]; m[14] = in_m.m[14];
    m[3] = in_m.m[3]; m[7] = in_m.m[7]; m[11] = in_m.m[11]; m[15] = in_m.m[15];
    im[0] = in_m.im[0]; im[4] = in_m.im[4]; im[8]  = in_m.im[8];  im[12] = in_m.im[12];
    im[1] = in_m.im[1]; im[5] = in_m.im[5]; im[9]  = in_m.im[9];  im[13] = in_m.im[13];
    im[2] = in_m.im[2]; im[6] = in_m.im[6]; im[10] = in_m.im[10]; im[14] = in_m.im[14];
    im[3] = in_m.im[3]; im[7] = in_m.im[7]; im[11] = in_m.im[11]; im[15] = in_m.im[15];
    m3[0] = in_m.m3[0]; m3[3] = in_m.m3[3]; m3[6] = in_m.m3[6];
    m3[1] = in_m.m3[1]; m3[4] = in_m.m3[4]; m3[7] = in_m.m3[7];
    m3[2] = in_m.m3[2]; m3[5] = in_m.m3[5]; m3[8] = in_m.m3[8];
    im3[0] = in_m.im3[0]; im3[3] = in_m.im3[3]; im3[6] = in_m.im3[6];
    im3[1] = in_m.im3[1]; im3[4] = in_m.im3[4]; im3[7] = in_m.im3[7];
    im3[2] = in_m.im3[2]; im3[5] = in_m.im3[5]; im3[8] = in_m.im3[8];

    return *this;
}

#ifdef BOUGE_COMPILE_CPP0X
AffineMatrix::AffineMatrix(AffineMatrix&& in_m)
    : m(std::move(in_m.m))
    , im(std::move(in_m.im))
    , m3(std::move(in_m.m3))
    , im3(std::move(in_m.im3))
{
    this->operator=(std::move(in_m));
}

const AffineMatrix& AffineMatrix::operator=(AffineMatrix&& in_m)
{
    this->m = std::move(in_m.m);
    this->im = std::move(in_m.im);
    this->m3 = std::move(in_m.m3);
    this->im3 = std::move(in_m.im3);

    return *this;
}
#endif // BOUGE_COMPILE_CPP0X

//////////////////////////////////
// Special matrix constructors. //
//////////////////////////////////

AffineMatrix AffineMatrix::translation(float in_fX, float in_fY, float in_fZ)
{
    AffineMatrix m;
    m.m[12] = in_fX;
    m.m[13] = in_fY;
    m.m[14] = in_fZ;
    m.im[12] = -in_fX;
    m.im[13] = -in_fY;
    m.im[14] = -in_fZ;
    return m;
}

AffineMatrix AffineMatrix::translation(const Vector& in_v)
{
    return AffineMatrix::translation(in_v.x(), in_v.y(), in_v.z());
}

AffineMatrix AffineMatrix::translation(const AffineMatrix& in_m)
{
    return AffineMatrix::translation(in_m(1,4), in_m(2,4), in_m(3,4));
}

AffineMatrix AffineMatrix::rotationX(float in_fTheta)
{
    AffineMatrix m;
    float c = cos(in_fTheta);
    float s = sin(in_fTheta);
    m.m[0] = 1.0f; m.m[4] = 0.0f; m.m[8]  = 0.0f; m.m[12] = 0.0f;
    m.m[1] = 0.0f; m.m[5] =    c; m.m[9]  =   -s; m.m[13] = 0.0f;
    m.m[2] = 0.0f; m.m[6] =    s; m.m[10] =    c; m.m[14] = 0.0f;
    m.m[3] = 0.0f; m.m[7] = 0.0f; m.m[11] = 0.0f; m.m[15] = 1.0f;
    m.im[0] = 1.0f; m.im[4] = 0.0f; m.im[8]  = 0.0f; m.im[12] = 0.0f;
    m.im[1] = 0.0f; m.im[5] =    c; m.im[9]  =    s; m.im[13] = 0.0f;
    m.im[2] = 0.0f; m.im[6] =   -s; m.im[10] =    c; m.im[14] = 0.0f;
    m.im[3] = 0.0f; m.im[7] = 0.0f; m.im[11] = 0.0f; m.im[15] = 1.0f;
    m.m3[0] = m.m[0]; m.m3[3] = m.m[4]; m.m3[6] = m.m[8];
    m.m3[1] = m.m[1]; m.m3[4] = m.m[5]; m.m3[7] = m.m[9];
    m.m3[2] = m.m[2]; m.m3[5] = m.m[6]; m.m3[8] = m.m[10];
    m.im3[0] = m.im[0]; m.im3[3] = m.im[4]; m.im3[6] = m.im[8];
    m.im3[1] = m.im[1]; m.im3[4] = m.im[5]; m.im3[7] = m.im[9];
    m.im3[2] = m.im[2]; m.im3[5] = m.im[6]; m.im3[8] = m.im[10];
    return m;
}

AffineMatrix AffineMatrix::rotationY(float in_fTheta)
{
    AffineMatrix m;
    float c = cos(in_fTheta);
    float s = sin(in_fTheta);
    m.m[0] =    c; m.m[4] = 0.0f; m.m[8]  =    s; m.m[12] = 0.0f;
    m.m[1] = 0.0f; m.m[5] = 1.0f; m.m[9]  = 0.0f; m.m[13] = 0.0f;
    m.m[2] =   -s; m.m[6] = 0.0f; m.m[10] =    c; m.m[14] = 0.0f;
    m.m[3] = 0.0f; m.m[7] = 0.0f; m.m[11] = 0.0f; m.m[15] = 1.0f;
    m.im[0] =    c; m.im[4] = 0.0f; m.im[8]  =   -s; m.im[12] = 0.0f;
    m.im[1] = 0.0f; m.im[5] = 1.0f; m.im[9]  = 0.0f; m.im[13] = 0.0f;
    m.im[2] =    s; m.im[6] = 0.0f; m.im[10] =    c; m.im[14] = 0.0f;
    m.im[3] = 0.0f; m.im[7] = 0.0f; m.im[11] = 0.0f; m.im[15] = 1.0f;
    m.m3[0] = m.m[0]; m.m3[3] = m.m[4]; m.m3[6] = m.m[8];
    m.m3[1] = m.m[1]; m.m3[4] = m.m[5]; m.m3[7] = m.m[9];
    m.m3[2] = m.m[2]; m.m3[5] = m.m[6]; m.m3[8] = m.m[10];
    m.im3[0] = m.im[0]; m.im3[3] = m.im[4]; m.im3[6] = m.im[8];
    m.im3[1] = m.im[1]; m.im3[4] = m.im[5]; m.im3[7] = m.im[9];
    m.im3[2] = m.im[2]; m.im3[5] = m.im[6]; m.im3[8] = m.im[10];
    return m;
}

AffineMatrix AffineMatrix::rotationZ(float in_fTheta)
{
    AffineMatrix m;
    float c = cos(in_fTheta);
    float s = sin(in_fTheta);
    m.m[0] =    c; m.m[4] =   -s; m.m[8]  = 0.0f; m.m[12] = 0.0f;
    m.m[1] =    s; m.m[5] =    c; m.m[9]  = 0.0f; m.m[13] = 0.0f;
    m.m[2] = 0.0f; m.m[6] = 0.0f; m.m[10] = 1.0f; m.m[14] = 0.0f;
    m.m[3] = 0.0f; m.m[7] = 0.0f; m.m[11] = 0.0f; m.m[15] = 1.0f;
    m.im[0] =    c; m.im[4] =    s; m.im[8]  = 0.0f; m.im[12] = 0.0f;
    m.im[1] =   -s; m.im[5] =    c; m.im[9]  = 0.0f; m.im[13] = 0.0f;
    m.im[2] = 0.0f; m.im[6] = 0.0f; m.im[10] = 1.0f; m.im[14] = 0.0f;
    m.im[3] = 0.0f; m.im[7] = 0.0f; m.im[11] = 0.0f; m.im[15] = 1.0f;
    m.m3[0] = m.m[0]; m.m3[3] = m.m[4]; m.m3[6] = m.m[8];
    m.m3[1] = m.m[1]; m.m3[4] = m.m[5]; m.m3[7] = m.m[9];
    m.m3[2] = m.m[2]; m.m3[5] = m.m[6]; m.m3[8] = m.m[10];
    m.im3[0] = m.im[0]; m.im3[3] = m.im[4]; m.im3[6] = m.im[8];
    m.im3[1] = m.im[1]; m.im3[4] = m.im[5]; m.im3[7] = m.im[9];
    m.im3[2] = m.im[2]; m.im3[5] = m.im[6]; m.im3[8] = m.im[10];
    return m;
}

AffineMatrix AffineMatrix::rotation(const Quaternion& in_quat)
{
    float s = 0.0f;
    float l = in_quat.dot(in_quat);
    if(nearZero(l)) {
        s = 1.0f;
    } else {
        s = 2.0f / l;
    }

    float xs = in_quat.x() * s;
    float ys = in_quat.y() * s;
    float zs = in_quat.z() * s;
    float wx = in_quat.w() * xs;
    float wy = in_quat.w() * ys;
    float wz = in_quat.w() * zs;
    float xx = in_quat.x() * xs;
    float xy = in_quat.x() * ys;
    float xz = in_quat.x() * zs;
    float yy = in_quat.y() * ys;
    float yz = in_quat.y() * zs;
    float zz = in_quat.z() * zs;

    AffineMatrix m;
    m.m[0] = 1.0f - (yy + zz); m.m[1] = xy + wz;          m.m[2]  = xz - wy;
    m.m[4] = xy - wz;          m.m[5] = 1.0f - (xx + zz); m.m[6]  = yz + wx;
    m.m[8] = xz + wy;          m.m[9] = yz - wx;          m.m[10] = 1.0f - (xx + yy);
    m.im[0] = 1.0f - (yy + zz); m.im[1] = xy - wz;          m.im[2]  = xz + wy;
    m.im[4] = xy + wz;          m.im[5] = 1.0f - (xx + zz); m.im[6]  = yz - wx;
    m.im[8] = xz - wy;          m.im[9] = yz + wx;          m.im[10] = 1.0f - (xx + yy);

    m.m3[0] = m.m[0]; m.m3[3] = m.m[4]; m.m3[6] = m.m[8];
    m.m3[1] = m.m[1]; m.m3[4] = m.m[5]; m.m3[7] = m.m[9];
    m.m3[2] = m.m[2]; m.m3[5] = m.m[6]; m.m3[8] = m.m[10];
    m.im3[0] = m.im[0]; m.im3[3] = m.im[4]; m.im3[6] = m.im[8];
    m.im3[1] = m.im[1]; m.im3[4] = m.im[5]; m.im3[7] = m.im[9];
    m.im3[2] = m.im[2]; m.im3[5] = m.im[6]; m.im3[8] = m.im[10];
    return m;
}

AffineMatrix AffineMatrix::scale(float in_fFactor)
{
    return AffineMatrix::scale(in_fFactor, in_fFactor, in_fFactor);
}

AffineMatrix AffineMatrix::scale(float in_fX, float in_fY, float in_fZ)
{
    if(nearZero(in_fX)) in_fX = 1.0f;
    if(nearZero(in_fY)) in_fY = 1.0f;
    if(nearZero(in_fZ)) in_fZ = 1.0f;
    const float oneoverX = 1.0f/in_fX;
    const float oneoverY = 1.0f/in_fY;
    const float oneoverZ = 1.0f/in_fZ;
    AffineMatrix m;
    m.m[0] = in_fX;
    m.m[5] = in_fY;
    m.m[10] = in_fZ;
    m.im[0] = oneoverX;
    m.im[5] = oneoverY;
    m.im[10] = oneoverZ;
    m.m3[0] = in_fX;
    m.m3[4] = in_fY;
    m.m3[8] = in_fZ;
    m.im3[0] = oneoverX;
    m.im3[4] = oneoverY;
    m.im3[8] = oneoverZ;
    return m;
}

AffineMatrix AffineMatrix::scale(const Vector& in_v)
{
    return AffineMatrix::scale(in_v.x(), in_v.y(), in_v.z());
}

AffineMatrix AffineMatrix::transformation(const Vector& in_trans, const Quaternion& in_rot)
{
    AffineMatrix m = AffineMatrix::rotation(in_rot);

    // Applying the translation directly to the rotation is way more efficient.

    m.m[12] = in_trans.x();
    m.m[13] = in_trans.y();
    m.m[14] = in_trans.z();

    m.im[12] = -in_trans.x()*m.im[0] - in_trans.y()*m.im[4] - in_trans.z()*m.im[8];
    m.im[13] = -in_trans.x()*m.im[1] - in_trans.y()*m.im[5] - in_trans.z()*m.im[9];
    m.im[14] = -in_trans.x()*m.im[2] - in_trans.y()*m.im[6] - in_trans.z()*m.im[10];
    return m;
}

AffineMatrix AffineMatrix::transformation(const Vector& in_trans, const Vector& in_scale, const Quaternion& in_rot)
{
    AffineMatrix m = AffineMatrix::rotation(in_rot);

    // Applying the scale and translation directly to the rotation is way more efficient.

    m.m[0] *= in_scale.x(); m.m[4] *= in_scale.x(); m.m[8]  *= in_scale.x(); m.m[12] = in_trans.x();
    m.m[1] *= in_scale.y(); m.m[5] *= in_scale.y(); m.m[9]  *= in_scale.y(); m.m[13] = in_trans.y();
    m.m[2] *= in_scale.z(); m.m[6] *= in_scale.z(); m.m[10] *= in_scale.z(); m.m[14] = in_trans.z();
    m.m3[0] = m.m[0]; m.m3[3] = m.m[4]; m.m3[6] = m.m[8];
    m.m3[1] = m.m[1]; m.m3[4] = m.m[5]; m.m3[7] = m.m[9];
    m.m3[2] = m.m[2]; m.m3[5] = m.m[6]; m.m3[8] = m.m[10];

    float one_over_s[] = {1.0f/in_scale.x(), 1.0f/in_scale.y(), 1.0f/in_scale.z()};
    m.im[0] *= one_over_s[0]; m.im[4] *= one_over_s[1]; m.im[8]  *= one_over_s[2];
    m.im[1] *= one_over_s[0]; m.im[5] *= one_over_s[1]; m.im[9]  *= one_over_s[2];
    m.im[2] *= one_over_s[0]; m.im[6] *= one_over_s[1]; m.im[10] *= one_over_s[2];
    float minus_t_over_s[] = {-in_trans.x() * one_over_s[0],
                              -in_trans.y() * one_over_s[1],
                              -in_trans.z() * one_over_s[2]};
    m.im[12] = minus_t_over_s[0]*m.im[0] + minus_t_over_s[1]*m.im[4] + minus_t_over_s[2]*m.im[8];
    m.im[13] = minus_t_over_s[0]*m.im[1] + minus_t_over_s[1]*m.im[5] + minus_t_over_s[2]*m.im[9];
    m.im[14] = minus_t_over_s[0]*m.im[2] + minus_t_over_s[1]*m.im[6] + minus_t_over_s[2]*m.im[10];

    m.im3[0] = m.im[0]; m.im3[3] = m.im[4]; m.im3[6] = m.im[8];
    m.im3[1] = m.im[1]; m.im3[4] = m.im[5]; m.im3[7] = m.im[9];
    m.im3[2] = m.im[2]; m.im3[5] = m.im[6]; m.im3[8] = m.im[10];
    return m;
}

void AffineMatrix::setRotation(const Quaternion& in_quat)
{
    float s = 0.0f;
    float l = in_quat.dot(in_quat);
    if(nearZero(l)) {
        s = 1.0f;
    } else {
        s = 2.0f / l;
    }

    float xs = in_quat.x() * s;
    float ys = in_quat.y() * s;
    float zs = in_quat.z() * s;
    float wx = in_quat.w() * xs;
    float wy = in_quat.w() * ys;
    float wz = in_quat.w() * zs;
    float xx = in_quat.x() * xs;
    float xy = in_quat.x() * ys;
    float xz = in_quat.x() * zs;
    float yy = in_quat.y() * ys;
    float yz = in_quat.y() * zs;
    float zz = in_quat.z() * zs;

    m[0] = 1.0f - (yy + zz);  m[1] = xy + wz;           m[2]  = xz - wy;
    m[4] = xy - wz;           m[5] = 1.0f - (xx + zz);  m[6]  = yz + wx;
    m[8] = xz + wy;           m[9] = yz - wx;           m[10] = 1.0f - (xx + yy);
    im[0] = 1.0f - (yy + zz); im[1] = xy - wz;          im[2]  = xz + wy;
    im[4] = xy + wz;          im[5] = 1.0f - (xx + zz); im[6]  = yz - wx;
    im[8] = xz - wy;          im[9] = yz + wx;          im[10] = 1.0f - (xx + yy);

    m3[0] = m[0]; m3[3] = m[4]; m3[6] = m[8];
    m3[1] = m[1]; m3[4] = m[5]; m3[7] = m[9];
    m3[2] = m[2]; m3[5] = m[6]; m3[8] = m[10];
    im3[0] = im[0]; im3[3] = im[4]; im3[6] = im[8];
    im3[1] = im[1]; im3[4] = im[5]; im3[7] = im[9];
    im3[2] = im[2]; im3[5] = im[6]; im3[8] = im[10];
}

void AffineMatrix::setTransformation(const Vector& in_trans, const Quaternion& in_rot)
{
    this->setRotation(in_rot);

    // Applying the translation directly to the rotation is way more efficient.

    m[12] = in_trans.x();
    m[13] = in_trans.y();
    m[14] = in_trans.z();

    im[12] = -in_trans.x()*im[0] - in_trans.y()*im[4] - in_trans.z()*im[8];
    im[13] = -in_trans.x()*im[1] - in_trans.y()*im[5] - in_trans.z()*im[9];
    im[14] = -in_trans.x()*im[2] - in_trans.y()*im[6] - in_trans.z()*im[10];
}

void AffineMatrix::setTransformation(const Vector& in_trans, const Vector& in_scale, const Quaternion& in_rot)
{
    this->setRotation(in_rot);

    // Applying the scale and translation directly to the rotation is way more efficient.

    m[0] *= in_scale.x(); m[4] *= in_scale.x(); m[8]  *= in_scale.x(); m[12] = in_trans.x();
    m[1] *= in_scale.y(); m[5] *= in_scale.y(); m[9]  *= in_scale.y(); m[13] = in_trans.y();
    m[2] *= in_scale.z(); m[6] *= in_scale.z(); m[10] *= in_scale.z(); m[14] = in_trans.z();
    m3[0] = m[0]; m3[3] = m[4]; m3[6] = m[8];
    m3[1] = m[1]; m3[4] = m[5]; m3[7] = m[9];
    m3[2] = m[2]; m3[5] = m[6]; m3[8] = m[10];

    float one_over_s[] = {1.0f/in_scale.x(), 1.0f/in_scale.y(), 1.0f/in_scale.z()};
    im[0] *= one_over_s[0]; im[4] *= one_over_s[1]; im[8]  *= one_over_s[2];
    im[1] *= one_over_s[0]; im[5] *= one_over_s[1]; im[9]  *= one_over_s[2];
    im[2] *= one_over_s[0]; im[6] *= one_over_s[1]; im[10] *= one_over_s[2];
    float minus_t_over_s[] = {-in_trans.x() * one_over_s[0],
                              -in_trans.y() * one_over_s[1],
                              -in_trans.z() * one_over_s[2]};
    im[12] = minus_t_over_s[0]*im[0] + minus_t_over_s[1]*im[4] + minus_t_over_s[2]*im[8];
    im[13] = minus_t_over_s[0]*im[1] + minus_t_over_s[1]*im[5] + minus_t_over_s[2]*im[9];
    im[14] = minus_t_over_s[0]*im[2] + minus_t_over_s[1]*im[6] + minus_t_over_s[2]*im[10];

    im3[0] = im[0]; im3[3] = im[4]; im3[6] = im[8];
    im3[1] = im[1]; im3[4] = im[5]; im3[7] = im[9];
    im3[2] = im[2]; im3[5] = im[6]; im3[8] = im[10];
}

void AffineMatrix::operator *=(const AffineMatrix& o)
{
    // Operation optimized for affine matrices: the lower row is 0 0 0 1.

    float oldm[] = {m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], m[8], m[9], m[10], m[11], m[12], m[13], m[14], m[15]};

    m[0]  = oldm[0] * o.m[0]  + oldm[4] * o.m[1]  + oldm[8]  * o.m[2];
    m[1]  = oldm[1] * o.m[0]  + oldm[5] * o.m[1]  + oldm[9]  * o.m[2];
    m[2]  = oldm[2] * o.m[0]  + oldm[6] * o.m[1]  + oldm[10] * o.m[2];

    m[4]  = oldm[0] * o.m[4]  + oldm[4] * o.m[5]  + oldm[8]  * o.m[6];
    m[5]  = oldm[1] * o.m[4]  + oldm[5] * o.m[5]  + oldm[9]  * o.m[6];
    m[6]  = oldm[2] * o.m[4]  + oldm[6] * o.m[5]  + oldm[10] * o.m[6];

    m[8]  = oldm[0] * o.m[8]  + oldm[4] * o.m[9]  + oldm[8]  * o.m[10];
    m[9]  = oldm[1] * o.m[8]  + oldm[5] * o.m[9]  + oldm[9]  * o.m[10];
    m[10] = oldm[2] * o.m[8]  + oldm[6] * o.m[9]  + oldm[10] * o.m[10];

    m[12] = oldm[0] * o.m[12] + oldm[4] * o.m[13] + oldm[8]  * o.m[14] + oldm[12] * o.m[15];
    m[13] = oldm[1] * o.m[12] + oldm[5] * o.m[13] + oldm[9]  * o.m[14] + oldm[13] * o.m[15];
    m[14] = oldm[2] * o.m[12] + oldm[6] * o.m[13] + oldm[10] * o.m[14] + oldm[14] * o.m[15];

    // Inverses are multiplied from the left.

    float oldim[] = {im[0], im[1], im[2], im[3], im[4], im[5], im[6], im[7], im[8], im[9], im[10], im[11], im[12], im[13], im[14], im[15]};

    im[0]  = o.im[0] * oldim[0]  + o.im[4] * oldim[1]  + o.im[8]  * oldim[2];
    im[1]  = o.im[1] * oldim[0]  + o.im[5] * oldim[1]  + o.im[9]  * oldim[2];
    im[2]  = o.im[2] * oldim[0]  + o.im[6] * oldim[1]  + o.im[10] * oldim[2];

    im[4]  = o.im[0] * oldim[4]  + o.im[4] * oldim[5]  + o.im[8]  * oldim[6];
    im[5]  = o.im[1] * oldim[4]  + o.im[5] * oldim[5]  + o.im[9]  * oldim[6];
    im[6]  = o.im[2] * oldim[4]  + o.im[6] * oldim[5]  + o.im[10] * oldim[6];

    im[8]  = o.im[0] * oldim[8]  + o.im[4] * oldim[9]  + o.im[8]  * oldim[10];
    im[9]  = o.im[1] * oldim[8]  + o.im[5] * oldim[9]  + o.im[9]  * oldim[10];
    im[10] = o.im[2] * oldim[8]  + o.im[6] * oldim[9]  + o.im[10] * oldim[10];

    im[12] = o.im[0] * oldim[12] + o.im[4] * oldim[13] + o.im[8]  * oldim[14] + o.im[12] * oldim[15];
    im[13] = o.im[1] * oldim[12] + o.im[5] * oldim[13] + o.im[9]  * oldim[14] + o.im[13] * oldim[15];
    im[14] = o.im[2] * oldim[12] + o.im[6] * oldim[13] + o.im[10] * oldim[14] + o.im[14] * oldim[15];

    // 3x3 parts are just copied over.

    m3[0] = m[0]; m3[3] = m[4]; m3[6] = m[8];
    m3[1] = m[1]; m3[4] = m[5]; m3[7] = m[9];
    m3[2] = m[2]; m3[5] = m[6]; m3[8] = m[10];

    im3[0] = im[0]; im3[3] = im[4]; im3[6] = im[8];
    im3[1] = im[1]; im3[4] = im[5]; im3[7] = im[9];
    im3[2] = im[2]; im3[5] = im[6]; im3[8] = im[10];
}
/* Was not really a gain.
void AffineMatrix::rightMultInv(const AffineMatrix& o)
{
    // Operation optimized for affine matrices: the lower row is 0 0 0 1.

    float oldm[] = {m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], m[8], m[9], m[10], m[11], m[12], m[13], m[14], m[15]};

    m[0]  = oldm[0] * o.im[0]  + oldm[4] * o.im[1]  + oldm[8]  * o.im[2];
    m[1]  = oldm[1] * o.im[0]  + oldm[5] * o.im[1]  + oldm[9]  * o.im[2];
    m[2]  = oldm[2] * o.im[0]  + oldm[6] * o.im[1]  + oldm[10] * o.im[2];

    m[4]  = oldm[0] * o.im[4]  + oldm[4] * o.im[5]  + oldm[8]  * o.im[6];
    m[5]  = oldm[1] * o.im[4]  + oldm[5] * o.im[5]  + oldm[9]  * o.im[6];
    m[6]  = oldm[2] * o.im[4]  + oldm[6] * o.im[5]  + oldm[10] * o.im[6];

    m[8]  = oldm[0] * o.im[8]  + oldm[4] * o.im[9]  + oldm[8]  * o.im[10];
    m[9]  = oldm[1] * o.im[8]  + oldm[5] * o.im[9]  + oldm[9]  * o.im[10];
    m[10] = oldm[2] * o.im[8]  + oldm[6] * o.im[9]  + oldm[10] * o.im[10];

    m[12] = oldm[0] * o.im[12] + oldm[4] * o.im[13] + oldm[8]  * o.im[14] + oldm[12] * o.im[15];
    m[13] = oldm[1] * o.im[12] + oldm[5] * o.im[13] + oldm[9]  * o.im[14] + oldm[13] * o.im[15];
    m[14] = oldm[2] * o.im[12] + oldm[6] * o.im[13] + oldm[10] * o.im[14] + oldm[14] * o.im[15];

    // Inverses are multiplied from the left.

    float oldim[] = {im[0], im[1], im[2], im[3], im[4], im[5], im[6], im[7], im[8], im[9], im[10], im[11], im[12], im[13], im[14], im[15]};

    im[0]  = o.m[0] * oldim[0]  + o.m[4] * oldim[1]  + o.m[8]  * oldim[2];
    im[1]  = o.m[1] * oldim[0]  + o.m[5] * oldim[1]  + o.m[9]  * oldim[2];
    im[2]  = o.m[2] * oldim[0]  + o.m[6] * oldim[1]  + o.m[10] * oldim[2];

    im[4]  = o.m[0] * oldim[4]  + o.m[4] * oldim[5]  + o.m[8]  * oldim[6];
    im[5]  = o.m[1] * oldim[4]  + o.m[5] * oldim[5]  + o.m[9]  * oldim[6];
    im[6]  = o.m[2] * oldim[4]  + o.m[6] * oldim[5]  + o.m[10] * oldim[6];

    im[8]  = o.m[0] * oldim[8]  + o.m[4] * oldim[9]  + o.m[8]  * oldim[10];
    im[9]  = o.m[1] * oldim[8]  + o.m[5] * oldim[9]  + o.m[9]  * oldim[10];
    im[10] = o.m[2] * oldim[8]  + o.m[6] * oldim[9]  + o.m[10] * oldim[10];

    im[12] = o.m[0] * oldim[12] + o.m[4] * oldim[13] + o.m[8]  * oldim[14] + o.m[12] * oldim[15];
    im[13] = o.m[1] * oldim[12] + o.m[5] * oldim[13] + o.m[9]  * oldim[14] + o.m[13] * oldim[15];
    im[14] = o.m[2] * oldim[12] + o.m[6] * oldim[13] + o.m[10] * oldim[14] + o.m[14] * oldim[15];

    // 3x3 parts are just copied over.

    m3[0] = m[0]; m3[3] = m[4]; m3[6] = m[8];
    m3[1] = m[1]; m3[4] = m[5]; m3[7] = m[9];
    m3[2] = m[2]; m3[5] = m[6]; m3[8] = m[10];

    im3[0] = im[0]; im3[3] = im[4]; im3[6] = im[8];
    im3[1] = im[1]; im3[4] = im[5]; im3[7] = im[9];
    im3[2] = im[2]; im3[5] = im[6]; im3[8] = im[10];
}
*/
AffineMatrix AffineMatrix::ortho2DProjection(float in_fW, float in_fH)
{
    if(nearZero(in_fW) || nearZero(in_fH))
        return AffineMatrix();

    AffineMatrix result;
    result.m[0] = 2.0f/in_fW;                                   result.m[12] = -1.0f;
                        result.m[5] = -2.0f/in_fH;              result.m[13] = 1.0f;
                                            result.m[10] = -1.0f;
//                                                                 15 = 1.0f is implicitly alredy done in c'tor.
    result.im[0] = 0.5f*in_fW;                                  result.im[12] = 0.5f*in_fW;
                        result.im[5] = -0.5f*in_fH;             result.im[13] = 0.5f*in_fH;
                                            result.im[10] = -1.0f;
    result.m3[0] = 2.0f/in_fW;
                        result.m3[4] = -2.0f/in_fH;
                                            result.m3[8] = -1.0f;
    result.im3[0] = 0.5f*in_fW;
                        result.im3[4] = -0.5f*in_fH;
                                            result.im3[8] = -1.0f;
    return result;
}

///////////////////////////////////////
// Conversion methods and operators. //
///////////////////////////////////////

AffineMatrix AffineMatrix::inverse() const
{
    AffineMatrix result;
    result.m[0] = im[0]; result.m[4] = im[4]; result.m[8]  = im[8];  result.m[12] = im[12];
    result.m[1] = im[1]; result.m[5] = im[5]; result.m[9]  = im[9];  result.m[13] = im[13];
    result.m[2] = im[2]; result.m[6] = im[6]; result.m[10] = im[10]; result.m[14] = im[14];
    result.m[3] = im[3]; result.m[7] = im[7]; result.m[11] = im[11]; result.m[15] = im[15];
    result.im[0] = m[0]; result.im[4] = m[4]; result.im[8]  = m[8];  result.im[12] = m[12];
    result.im[1] = m[1]; result.im[5] = m[5]; result.im[9]  = m[9];  result.im[13] = m[13];
    result.im[2] = m[2]; result.im[6] = m[6]; result.im[10] = m[10]; result.im[14] = m[14];
    result.im[3] = m[3]; result.im[7] = m[7]; result.im[11] = m[11]; result.im[15] = m[15];
    result.m3[0] = im3[0]; result.m3[3] = im3[3]; result.m3[6] = im3[6];
    result.m3[1] = im3[1]; result.m3[4] = im3[4]; result.m3[7] = im3[7];
    result.m3[2] = im3[2]; result.m3[5] = im3[5]; result.m3[8] = im3[8];
    result.im3[0] = m3[0]; result.im3[3] = m3[3]; result.im3[6] = m3[6];
    result.im3[1] = m3[1]; result.im3[4] = m3[4]; result.im3[7] = m3[7];
    result.im3[2] = m3[2]; result.im3[5] = m3[5]; result.im3[8] = m3[8];
    return result;
}

Vector AffineMatrix::right() const
{
    return Vector(m[0], m[4], m[8]);
}

Vector AffineMatrix::up() const
{
    return Vector(m[1], m[5], m[9]);
}

Vector AffineMatrix::front() const
{
    return Vector(-m[2], -m[6], -m[10]);
}

////////////////////////////
// Matrix-Matrix product. //
////////////////////////////

AffineMatrix AffineMatrix::operator *(const AffineMatrix& o) const
{
    AffineMatrix result;

    // Operation optimized for affine matrices: the lower row is 0 0 0 1.

    result.m[0]  = m[0] * o.m[0]  + m[4] * o.m[1]  + m[8]  * o.m[2];
    result.m[1]  = m[1] * o.m[0]  + m[5] * o.m[1]  + m[9]  * o.m[2];
    result.m[2]  = m[2] * o.m[0]  + m[6] * o.m[1]  + m[10] * o.m[2];

    result.m[4]  = m[0] * o.m[4]  + m[4] * o.m[5]  + m[8]  * o.m[6];
    result.m[5]  = m[1] * o.m[4]  + m[5] * o.m[5]  + m[9]  * o.m[6];
    result.m[6]  = m[2] * o.m[4]  + m[6] * o.m[5]  + m[10] * o.m[6];

    result.m[8]  = m[0] * o.m[8]  + m[4] * o.m[9]  + m[8]  * o.m[10];
    result.m[9]  = m[1] * o.m[8]  + m[5] * o.m[9]  + m[9]  * o.m[10];
    result.m[10] = m[2] * o.m[8]  + m[6] * o.m[9]  + m[10] * o.m[10];

    result.m[12] = m[0] * o.m[12] + m[4] * o.m[13] + m[8]  * o.m[14] + m[12] * o.m[15];
    result.m[13] = m[1] * o.m[12] + m[5] * o.m[13] + m[9]  * o.m[14] + m[13] * o.m[15];
    result.m[14] = m[2] * o.m[12] + m[6] * o.m[13] + m[10] * o.m[14] + m[14] * o.m[15];

    // Inverses are multiplied from the left.

    result.im[0]  = o.im[0] * im[0]  + o.im[4] * im[1]  + o.im[8]  * im[2];
    result.im[1]  = o.im[1] * im[0]  + o.im[5] * im[1]  + o.im[9]  * im[2];
    result.im[2]  = o.im[2] * im[0]  + o.im[6] * im[1]  + o.im[10] * im[2];

    result.im[4]  = o.im[0] * im[4]  + o.im[4] * im[5]  + o.im[8]  * im[6];
    result.im[5]  = o.im[1] * im[4]  + o.im[5] * im[5]  + o.im[9]  * im[6];
    result.im[6]  = o.im[2] * im[4]  + o.im[6] * im[5]  + o.im[10] * im[6];

    result.im[8]  = o.im[0] * im[8]  + o.im[4] * im[9]  + o.im[8]  * im[10];
    result.im[9]  = o.im[1] * im[8]  + o.im[5] * im[9]  + o.im[9]  * im[10];
    result.im[10] = o.im[2] * im[8]  + o.im[6] * im[9]  + o.im[10] * im[10];

    result.im[12] = o.im[0] * im[12] + o.im[4] * im[13] + o.im[8]  * im[14] + o.im[12] * im[15];
    result.im[13] = o.im[1] * im[12] + o.im[5] * im[13] + o.im[9]  * im[14] + o.im[13] * im[15];
    result.im[14] = o.im[2] * im[12] + o.im[6] * im[13] + o.im[10] * im[14] + o.im[14] * im[15];

    // 3x3 parts are just copied over.

    result.m3[0] = result.m[0]; result.m3[3] = result.m[4]; result.m3[6] = result.m[8];
    result.m3[1] = result.m[1]; result.m3[4] = result.m[5]; result.m3[7] = result.m[9];
    result.m3[2] = result.m[2]; result.m3[5] = result.m[6]; result.m3[8] = result.m[10];

    result.im3[0] = result.im[0]; result.im3[3] = result.im[4]; result.im3[6] = result.im[8];
    result.im3[1] = result.im[1]; result.im3[4] = result.im[5]; result.im3[7] = result.im[9];
    result.im3[2] = result.im[2]; result.im3[5] = result.im[6]; result.im3[8] = result.im[10];

    return result;
}

// void AffineMatrix::operator *=(const AffineMatrix& o)
// {
//     this->operator=(*this * o);
// }

General4x4Matrix AffineMatrix::operator*(const General4x4Matrix& o) const
{
    return General4x4Matrix(*this) * o;
}

/////////////////////////////////
/////////////////////////////////
//// The General Matrix part ////
/////////////////////////////////
/////////////////////////////////

General4x4Matrix::~General4x4Matrix()
{
}

////////////////////////////////////////////
// Constructors and assignment operators. //
////////////////////////////////////////////

General4x4Matrix::General4x4Matrix()
    : Base4x4Matrix()
{
}

General4x4Matrix::General4x4Matrix(const General4x4Matrix& in_m)
    : Base4x4Matrix()
{
    this->operator=(in_m);
}

const General4x4Matrix& General4x4Matrix::operator=(const General4x4Matrix& in_m)
{
    m[0] = in_m.m[0]; m[4] = in_m.m[4]; m[8]  = in_m.m[8];  m[12] = in_m.m[12];
    m[1] = in_m.m[1]; m[5] = in_m.m[5]; m[9]  = in_m.m[9];  m[13] = in_m.m[13];
    m[2] = in_m.m[2]; m[6] = in_m.m[6]; m[10] = in_m.m[10]; m[14] = in_m.m[14];
    m[3] = in_m.m[3]; m[7] = in_m.m[7]; m[11] = in_m.m[11]; m[15] = in_m.m[15];
    im[0] = in_m.im[0]; im[4] = in_m.im[4]; im[8]  = in_m.im[8];  im[12] = in_m.im[12];
    im[1] = in_m.im[1]; im[5] = in_m.im[5]; im[9]  = in_m.im[9];  im[13] = in_m.im[13];
    im[2] = in_m.im[2]; im[6] = in_m.im[6]; im[10] = in_m.im[10]; im[14] = in_m.im[14];
    im[3] = in_m.im[3]; im[7] = in_m.im[7]; im[11] = in_m.im[11]; im[15] = in_m.im[15];

    return *this;
}

#ifdef BOUGE_COMPILE_CPP0X
General4x4Matrix::General4x4Matrix(General4x4Matrix&& in_m)
    : m(std::move(in_m.m))
    , im(std::move(in_m.im))
{ }

const General4x4Matrix& General4x4Matrix::operator=(General4x4Matrix&& in_m)
{
    m = std::move(in_m.m);
    im = std::move(in_m.im);

    return *this;
}
#endif // BOUGE_COMPILE_CPP0X

//////////////////////////////////
// Special matrix constructors. //
//////////////////////////////////

General4x4Matrix::General4x4Matrix(const AffineMatrix& in_m)
    : Base4x4Matrix()
{
    this->operator=(in_m);
}

const General4x4Matrix& General4x4Matrix::operator=(const AffineMatrix& in_m)
{
    m[0] = in_m.m[0]; m[4] = in_m.m[4]; m[8]  = in_m.m[8];  m[12] = in_m.m[12];
    m[1] = in_m.m[1]; m[5] = in_m.m[5]; m[9]  = in_m.m[9];  m[13] = in_m.m[13];
    m[2] = in_m.m[2]; m[6] = in_m.m[6]; m[10] = in_m.m[10]; m[14] = in_m.m[14];
    m[3] = in_m.m[3]; m[7] = in_m.m[7]; m[11] = in_m.m[11]; m[15] = in_m.m[15];
    im[0] = in_m.im[0]; im[4] = in_m.im[4]; im[8]  = in_m.im[8];  im[12] = in_m.im[12];
    im[1] = in_m.im[1]; im[5] = in_m.im[5]; im[9]  = in_m.im[9];  im[13] = in_m.im[13];
    im[2] = in_m.im[2]; im[6] = in_m.im[6]; im[10] = in_m.im[10]; im[14] = in_m.im[14];
    im[3] = in_m.im[3]; im[7] = in_m.im[7]; im[11] = in_m.im[11]; im[15] = in_m.im[15];

    return *this;
}

#ifdef BOUGE_COMPILE_CPP0X
General4x4Matrix::General4x4Matrix(AffineMatrix&& in_m)
    : m(std::move(in_m.m))
    , im(std::move(in_m.im))
{ }

const General4x4Matrix& General4x4Matrix::operator=(AffineMatrix&& in_m)
{
    m = std::move(in_m.m);
    im = std::move(in_m.im);

    return *this;
}
#endif // BOUGE_COMPILE_CPP0X

General4x4Matrix General4x4Matrix::perspectiveProjection(float in_fFoV, float in_fAspectRatio, float in_fNearPlane, float in_fFarPlane)
{
    // Check for ill-formated input.

    // Make fov reside between 0 and 180.
    if(in_fFoV < 0.0f) in_fFoV = -in_fFoV;
    int ifov = (int)in_fFoV/180;
    float fov = in_fFoV - (float)(ifov*180);

    // 90 would crash the tangent, 180 and 0 would give t the value 0, crashing
    // the division below.
    if(nearZero(fov-90.0f) || nearZero(fov-180.0f) || nearZero(fov))
        fov = 45.0f;

    // If f and n were the same, it would let f-n become 0 and crash the division below.
    float f = in_fFarPlane;
    float n = in_fNearPlane;
    if(nearZero(f - n)) {
        f = 1000.0f;
        n = 2.5f;
    }

    float t = tan(deg2rad*fov/2.0f);

    General4x4Matrix result;
    result.m[0] = 1.0f/(t*in_fAspectRatio);
                        result.m[5] = 1.0f/t;
                                            result.m[10] = -(f+n)/(f-n); result.m[14] = -2.0f*f*n/(f-n);
                                            result.m[11] = -1.0f;        result.m[15] = 0.0f;
    result.im[0] = t*in_fAspectRatio;
                        result.im[5] = t;
                                                                               result.im[14] = -1.0f;
                                            result.im[11] = -0.5f*(f-n)/(f*n); result.im[15] = 0.5f*(f+n)/(f-n);
    return result;
}

///////////////////////////////////////
// Conversion methods and operators. //
///////////////////////////////////////

General4x4Matrix General4x4Matrix::inverse() const
{
    General4x4Matrix result;
    result.m[0] = im[0]; result.m[4] = im[4]; result.m[8]  = im[8];  result.m[12] = im[12];
    result.m[1] = im[1]; result.m[5] = im[5]; result.m[9]  = im[9];  result.m[13] = im[13];
    result.m[2] = im[2]; result.m[6] = im[6]; result.m[10] = im[10]; result.m[14] = im[14];
    result.m[3] = im[3]; result.m[7] = im[7]; result.m[11] = im[11]; result.m[15] = im[15];
    result.im[0] = m[0]; result.im[4] = m[4]; result.im[8]  = m[8];  result.im[12] = m[12];
    result.im[1] = m[1]; result.im[5] = m[5]; result.im[9]  = m[9];  result.im[13] = m[13];
    result.im[2] = m[2]; result.im[6] = m[6]; result.im[10] = m[10]; result.im[14] = m[14];
    result.im[3] = m[3]; result.im[7] = m[7]; result.im[11] = m[11]; result.im[15] = m[15];
    return result;
}

////////////////////////////
// Matrix-Matrix product. //
////////////////////////////

General4x4Matrix General4x4Matrix::operator *(const General4x4Matrix& o) const
{
    General4x4Matrix result;

    result.m[0]  = m[0] * o.m[0]  + m[4] * o.m[1]  + m[8]  * o.m[2]  + m[12] * o.m[3];
    result.m[1]  = m[1] * o.m[0]  + m[5] * o.m[1]  + m[9]  * o.m[2]  + m[13] * o.m[3];
    result.m[2]  = m[2] * o.m[0]  + m[6] * o.m[1]  + m[10] * o.m[2]  + m[14] * o.m[3];
    result.m[3]  = m[3] * o.m[0]  + m[7] * o.m[1]  + m[11] * o.m[2]  + m[15] * o.m[3];

    result.m[4]  = m[0] * o.m[4]  + m[4] * o.m[5]  + m[8]  * o.m[6]  + m[12] * o.m[7];
    result.m[5]  = m[1] * o.m[4]  + m[5] * o.m[5]  + m[9]  * o.m[6]  + m[13] * o.m[7];
    result.m[6]  = m[2] * o.m[4]  + m[6] * o.m[5]  + m[10] * o.m[6]  + m[14] * o.m[7];
    result.m[7]  = m[3] * o.m[4]  + m[7] * o.m[5]  + m[11] * o.m[6]  + m[15] * o.m[7];

    result.m[8]  = m[0] * o.m[8]  + m[4] * o.m[9]  + m[8]  * o.m[10] + m[12] * o.m[11];
    result.m[9]  = m[1] * o.m[8]  + m[5] * o.m[9]  + m[9]  * o.m[10] + m[13] * o.m[11];
    result.m[10] = m[2] * o.m[8]  + m[6] * o.m[9]  + m[10] * o.m[10] + m[14] * o.m[11];
    result.m[11] = m[3] * o.m[8]  + m[7] * o.m[9]  + m[11] * o.m[10] + m[15] * o.m[11];

    result.m[12] = m[0] * o.m[12] + m[4] * o.m[13] + m[8]  * o.m[14] + m[12] * o.m[15];
    result.m[13] = m[1] * o.m[12] + m[5] * o.m[13] + m[9]  * o.m[14] + m[13] * o.m[15];
    result.m[14] = m[2] * o.m[12] + m[6] * o.m[13] + m[10] * o.m[14] + m[14] * o.m[15];
    result.m[15] = m[3] * o.m[12] + m[7] * o.m[13] + m[11] * o.m[14] + m[15] * o.m[15];

    // Inverses are multiplied from the left.

    result.im[0]  = o.im[0] * im[0]  + o.im[4] * im[1]  + o.im[8]  * im[2]  + o.im[12] * im[3];
    result.im[1]  = o.im[1] * im[0]  + o.im[5] * im[1]  + o.im[9]  * im[2]  + o.im[13] * im[3];
    result.im[2]  = o.im[2] * im[0]  + o.im[6] * im[1]  + o.im[10] * im[2]  + o.im[14] * im[3];
    result.im[3]  = o.im[3] * im[0]  + o.im[7] * im[1]  + o.im[11] * im[2]  + o.im[15] * im[3];

    result.im[4]  = o.im[0] * im[4]  + o.im[4] * im[5]  + o.im[8]  * im[6]  + o.im[12] * im[7];
    result.im[5]  = o.im[1] * im[4]  + o.im[5] * im[5]  + o.im[9]  * im[6]  + o.im[13] * im[7];
    result.im[6]  = o.im[2] * im[4]  + o.im[6] * im[5]  + o.im[10] * im[6]  + o.im[14] * im[7];
    result.im[7]  = o.im[3] * im[4]  + o.im[7] * im[5]  + o.im[11] * im[6]  + o.im[15] * im[7];

    result.im[8]  = o.im[0] * im[8]  + o.im[4] * im[9]  + o.im[8]  * im[10] + o.im[12] * im[11];
    result.im[9]  = o.im[1] * im[8]  + o.im[5] * im[9]  + o.im[9]  * im[10] + o.im[13] * im[11];
    result.im[10] = o.im[2] * im[8]  + o.im[6] * im[9]  + o.im[10] * im[10] + o.im[14] * im[11];
    result.im[11] = o.im[3] * im[8]  + o.im[7] * im[9]  + o.im[11] * im[10] + o.im[15] * im[11];

    result.im[12] = o.im[0] * im[12] + o.im[4] * im[13] + o.im[8]  * im[14] + o.im[12] * im[15];
    result.im[13] = o.im[1] * im[12] + o.im[5] * im[13] + o.im[9]  * im[14] + o.im[13] * im[15];
    result.im[14] = o.im[2] * im[12] + o.im[6] * im[13] + o.im[10] * im[14] + o.im[14] * im[15];
    result.im[15] = o.im[3] * im[12] + o.im[7] * im[13] + o.im[11] * im[14] + o.im[15] * im[15];

    return result;
}

void General4x4Matrix::operator *=(const General4x4Matrix& o)
{
    this->operator=(*this * o);
}

} // namespace bouge
