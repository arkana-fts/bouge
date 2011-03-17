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
#ifndef BOUGE_MATRIX_H
#define BOUGE_MATRIX_H

#include <bouge/Config.hpp>

#include <string>
#include <vector>

namespace bouge {
    class AffineMatrix;
    class General4x4Matrix;
    class Vector;
    class Quaternion;

/// This class is just a container for some of the common code of both other
/// 4x4 matrix classes (AffineMatrix and General4x4Matrix).
class BOUGE_API Base4x4Matrix {
protected:
    /// As long as my mind is not clear anough on how this would allow fake
    /// tricky affine<->general conversions, this is just not allowed.
    Base4x4Matrix(const Base4x4Matrix&) {};

public:
    ////////////////////////////////////////////
    // Constructors and assignment operators. //
    ////////////////////////////////////////////

    /// Creates an identity matrix (all components to 0 but the diagonal to 1)
    Base4x4Matrix();

    /// If you don't get this (it's implemented),
    /// read point 2.1 at http://www.gotw.ca/gotw/031.htm
    virtual ~Base4x4Matrix() = 0;

    ///////////////////////////////////////
    // Conversion methods and operators. //
    ///////////////////////////////////////

    /// \return A read-only array of 16 floats holding the values of the
    ///         matrix in column-wise representation.
    inline const float *array16f() const {return &m[0];};
    /// \return A read-only array of 16 floats holding the values of the
    ///         inverse of the matrix in column-wise representation.
    inline const float *array16fInverse() const {return &im[0];};

    /// \return A string-representation of the matrix and its inverse.
    /// \param in_iDecimalPlaces The amount of numbers to print behind the dot.
    /// \param in_bOneLiner Whether to print the matrix as one single line or
    ///                     using multiple lines (more readable).
    /// \note The one-liner representation is given row-wise for readability.
    std::string to_s(unsigned int in_iDecimalPlaces = 2, bool in_bOneLiner = false) const;
    /// \return A string-representation of the matrix and its inverse.
    operator std::string() const;

    /////////////////////////////////////
    // Accessors, getters and setters. //
    /////////////////////////////////////

    /// Get the value of an element of this matrix.
    /// \param idx The index of the element of this matrix (column-wise).
    ///            This may only be a value between 0 and 15.
    /// \throws std::out_of_range if \a idx is >15.
    float operator[](unsigned int idx) const;
    /// Get the value of an element of this matrix. Acces it in the mathematical syntax.
    /// \param i The index of the row of the matrix. May only be a value between 1 and 4.
    /// \param j The index of the column of the matrix. May only be a value between 1 and 4.
    /// \throws std::out_of_range if \a i or \a j is >4 or 0.
    /// \note For example, the element [4, 1] is the element at the bottom left.
    float operator()(unsigned int i, unsigned int j) const;

protected:
    /// The matrix-data, in row-wise order.
    std::vector<float> m;
    /// The inverse matrix-data, in row-wise order.
    std::vector<float> im;
};

/// This matrix class defines a four-by-four matrix that is intended to be used
/// for affine transformations. Affine transformations are rotation, translation,
/// scaling and sheering. I add 2D orthogonal projection to the set too because
/// it is just scaling and translation.\n
/// This matrix class is useful for OpenGL, as with every operation it computes
/// the inverse at the same time and keeps it stored, giving you the ability
/// to get the inverse at anytime at absolutely no cost.\n
/// Of course, nothing comes for free, so you pay the performance with memory:
/// as it stores the the matrix itself, its 4x4 inverse \e and \e its \e upper
/// \e left \e 3x3 part \e and \e its \e inverse's \e upper \e left \e 3x3 part.
/// This 3x3 inverse is especially useful as a matris for transforming the normals.\n
/// To be as useful as possible, this matrix is stored row-wise, just as OpenGL
/// expects it to be.
class BOUGE_API AffineMatrix : public Base4x4Matrix {
    template<class T>
    friend T& operator>>(T& f, AffineMatrix& m);
    friend class General4x4Matrix;
public:
    virtual ~AffineMatrix();

    ////////////////////////////////////////////
    // Constructors and assignment operators. //
    ////////////////////////////////////////////

    /// Creates an identity matrix (all components to 0 but the diagonal to 1)
    AffineMatrix();
    /// Copies a matrix.
    /// \param in_m The matrix to be copied.
    AffineMatrix(const AffineMatrix& in_m);
    /// Copies a matrix.
    /// \param in_m The matrix to be copied.
    /// \return a const reference to myself that might be used as a rvalue.
    const AffineMatrix& operator=(const AffineMatrix& in_m);
#ifdef BOUGE_COMPILE_CPP0X
    /// Moves a matrix.
    /// \param in_m The matrix to be moved.
    AffineMatrix(AffineMatrix&& in_m);
    /// Moves a matrix.
    /// \param in_m The matrix to be moved.
    /// \return a const reference to myself that might be used as a rvalue.
    const AffineMatrix& operator=(AffineMatrix&& in_m);
#endif // BOUGE_COMPILE_CPP0X

    //////////////////////////////////
    // Special matrix constructors. //
    //////////////////////////////////

    /// \param in_fX The amount of translation in X direction.
    /// \param in_fY The amount of translation in Y direction.
    /// \param in_fZ The amount of translation in Z direction.
    /// \return A matrix that represents a translation.
    static AffineMatrix translation(float in_fX, float in_fY, float in_fZ);
    /// \param in_v The amount of translation.
    /// \return A matrix that represents a translation.
    static AffineMatrix translation(const Vector& in_v);
    /// \param in_m The matrix holding the translation we want to get.
    /// \return A matrix that represents a translation taken from another
    ///         affine transformation matrix.
    static AffineMatrix translation(const AffineMatrix& in_m);

    /// \param in_fTheta The rotation angle in radians.
    /// \return A matrix representing a rotation of \a in_fTheta radians around
    ///         the positive global X-axis.
    static AffineMatrix rotationX(float in_fTheta);
    /// \param in_fTheta The rotation angle in radians.
    /// \return A matrix representing a rotation of \a in_fTheta radians around
    ///         the positive global Y-axis.
    static AffineMatrix rotationY(float in_fTheta);
    /// \param in_fTheta The rotation angle in radians.
    /// \return A matrix representing a rotation of \a in_fTheta radians around
    ///         the positive global Z-axis.
    static AffineMatrix rotationZ(float in_fTheta);
    /// \param in_quat A quaternion representing the wanted rotation.
    /// \return A matrix representing a rotation about an arbitrary axis. The
    ///         rotation has to be given in form of a quaternion.
    static AffineMatrix rotation(const Quaternion& in_quat);

    /// \param in_fFactor The uniform scaling factor.
    /// \return A matrix representing a uniform scaling transformation.
    /// \note If in_fFactor is too close to zero, a unit matrix will be created.
    static AffineMatrix scale(float in_fFactor);
    /// \param in_fX The scaling factor in X-direction.
    /// \param in_fY The scaling factor in Y-direction.
    /// \param in_fZ The scaling factor in Z-direction.
    /// \return A matrix representing a non-uniform scaling transformation.
    /// \note If one of the three components is too close to zero, it will be
    ///       replaced by one.
    static AffineMatrix scale(float in_fX, float in_fY, float in_fZ);
    /// \param in_v A vector describing the scaling factors in all three directions.
    /// \return A matrix representing a non-uniform scaling transformation.
    /// \note If one of the three components is too close to zero, it will be
    ///       replaced by one.
    static AffineMatrix scale(const Vector& in_v);

    /// This creates a transformation matrix as they are commonly used. That is
    /// a matrix that first rotates and then translates a vector.
    /// Thus the created matrix is M = Trans*Rotate.
    /// \param in_trans The translational part of the matrix.
    /// \param in_rot The rotational part of the matrix.
    /// \return A matrix concatenating M = in_trans*in_rot.
    /// \note Obviously, this is more optimal than doing the concatenation by hand.
    static AffineMatrix transformation(const Vector& in_trans, const Quaternion& in_rot);

    /// This creates a transformation matrix as they are commonly used. That is
    /// a matrix that first rotates, then scales and then translates a vector.
    /// Thus the created matrix is M = Trans*Scale*Rotate.
    /// \param in_trans The translational part of the matrix.
    /// \param in_scale The scaling part of the matrix.
    /// \param in_rot The rotational part of the matrix.
    /// \return A matrix concatenating M = in_trans*in_scale*in_rot.
    /// \note Obviously, this is more optimal than doing the concatenation by hand.
    static AffineMatrix transformation(const Vector& in_trans, const Vector& in_scale, const Quaternion& in_rot);

    // TEST
    void setRotation(const Quaternion& quat);
    void setTransformation(const Vector& in_trans, const Quaternion& in_rot);
    void setTransformation(const Vector& in_trans, const Vector& in_scale, const Quaternion& in_rot);
//     void rightMultInv(const AffineMatrix& o);
    // TEST

    /// Creates an 2D orthographic projection. This places the origin at the
    /// top left of the screen, positive X going to the right, positive Y going
    /// down. That's what we'd expect for normal 2D drawing.
    /// \param in_fW The width of the screen, in pixels.
    /// \param in_fH The height of the screen, in pixels.
    /// \return A matrix representing a 2D orthographic projection as specified above.
    /// \warning The 3x3 inverse of this matrix is missing some important parts.
    ///          \e Don't \e use \e it! The 4x4 inverse is fine though.
    /// \note If either \a in_fW or \a in_fH is zero, this returns a unit matrix.
    static AffineMatrix ortho2DProjection(float in_fW, float in_fH);

    ///////////////////////////////////////
    // Conversion methods and operators. //
    ///////////////////////////////////////

    /// \return A read-only array of 9 floats holding the values of the
    ///         upper left 3x3 part of the matrix in column-wise representation.
    inline const float *array9f() const {return &m3[0];};
    /// \return A read-only array of 9 floats holding the values of the
    ///         upper left 3x3 part of the inverse of the matrix in
    ///         column-wise representation.
    inline const float *array9fInverse() const {return &im3[0];};

    /// \return An AffineMatrix representing the inverse of myself. (Having
    ///         myself as its inverse again.)
    AffineMatrix inverse() const;

    /// \return The "right" (or X) vector defined by this matrix's local coordinate system.
    ///         That is actually the same as this * (1, 0, 0).
    /// \note It is not normalized, but if this matrix does no scaling it should be normal.
    Vector right() const;

    /// \return The "up" (or Y) vector defined by this matrix's local coordinate system.
    ///         That is actually the same as this * (0, 1, 0).
    /// \note It is not normalized, but if this matrix does no scaling it should be normal.
    Vector up() const;

    /// \return The "front" (or Z) vector defined by this matrix's local coordinate system.
    ///         That is actually the same as this * (0, 0, -1).
    /// \note It is not normalized, but if this matrix does no scaling it should be normal.
    Vector front() const;

    ////////////////////////////
    // Matrix-Matrix product. //
    ////////////////////////////

    /// Returns the product of this matrix with another one. \a o gets
    /// multiplied on the right of this.
    /// \param o The other matrix that has to be multiplied from the right.
    /// \return The matrix resulting from *this * \a o.
    /// \note Of course, for the inverse the multiplication is done from the left.
    AffineMatrix operator *(const AffineMatrix& o) const;
    /// Multiplies this matrix with another one. \a o gets multiplied on the
    /// right of this.
    /// \param o The other matrix that has to be multiplied from the right.
    /// \note Of course, for the inverse the multiplication is done from the left.
    void operator *=(const AffineMatrix& o);

    /// Returns the product of this matrix with another general one. \a o gets
    /// multiplied on the right of this.
    /// \param o The other matrix that has to be multiplied from the right.
    /// \return The matrix resulting from *this * \a o.
    /// \note Of course, for the inverse the multiplication is done from the left.
    /// \note The result is a general matrix, not an affine one anymore.
    General4x4Matrix operator *(const General4x4Matrix& o) const;

private:
    /// The upper-left 3x3 part of the matrix-data, used to pass it to
    /// OpenGl as a pointer.
    std::vector<float> m3;
    /// The upper-left 3x3 part of the inverse matrix-data, used to pass it to
    /// OpenGl as a pointer.
    std::vector<float> im3;
};

/// This matrix class defines a more general four-by-four matrix.
/// Especially, it may represent not only everything that an \a AffineMatrix can
/// represent but also perspective projections.\n
/// It is (initially) designed especially to hold the perspective projection
/// matrix and its inverse the unprojection matrix.\n
/// Its main disadvantage in comparison to the affine matrix is that it looses
/// its ability to store a 3x3 inverse part. This is due to the nature of the
/// projection that can not be "partly undone, ignoring translations". It can,
/// but not that easily. Let's leave this as a bachelor thesis for somebody :D \n
/// One more advantage is that it needs 18 floats less memory than AffineMatrix.\n
/// Again, this matrix is stored row-wise, just as OpenGL expects it to be.
class BOUGE_API General4x4Matrix : public Base4x4Matrix {
    template<class T>
    friend T& operator>>(T& f, General4x4Matrix& m);
    friend class AffineMatrix;
public:
    virtual ~General4x4Matrix();

    ////////////////////////////////////////////
    // Constructors and assignment operators. //
    ////////////////////////////////////////////

    /// Creates an identity matrix (all components to 0 but the diagonal to 1)
    General4x4Matrix();
    /// Copies a matrix.
    /// \param in_m The matrix to be copied.
    General4x4Matrix(const General4x4Matrix& in_m);
    /// Copies a matrix.
    /// \param in_m The matrix to be copied.
    /// \return a const reference to myself that might be used as a rvalue.
    const General4x4Matrix& operator=(const General4x4Matrix& in_m);
#ifdef BOUGE_COMPILE_CPP0X
    /// Moves a matrix.
    /// \param in_m The matrix to be moved.
    General4x4Matrix(General4x4Matrix&& in_m);
    /// Moves a matrix.
    /// \param in_m The matrix to be moved.
    /// \return a const reference to myself that might be used as a rvalue.
    const General4x4Matrix& operator=(General4x4Matrix&& in_m);
#endif // BOUGE_COMPILE_CPP0X

    //////////////////////////////////
    // Special matrix constructors. //
    //////////////////////////////////

    /// Turns an affine matrix into a general matrix.
    /// \param in_m The affine matrix to be copied.
    General4x4Matrix(const AffineMatrix& in_m);
    /// Turns an affine matrix into a general matrix.
    /// \param in_m The affine matrix to be copied.
    /// \return a const reference to myself that might be used as a rvalue.
    const General4x4Matrix& operator=(const AffineMatrix& in_m);
#ifdef BOUGE_COMPILE_CPP0X
    /// Moves an affine matrix into a general matrix.
    /// \param in_m The affine matrix to be moved.
    General4x4Matrix(AffineMatrix&& in_m);
    /// Moves an affine matrix into a general matrix.
    /// \param in_m The affine matrix to be moved.
    /// \return a const reference to myself that might be used as a rvalue.
    const General4x4Matrix& operator=(AffineMatrix&& in_m);
#endif // BOUGE_COMPILE_CPP0X

    /// Creates a perspective projection matrix and its inverse the unprojection
    /// matrix.\n
    /// \param in_fFoV The Field of View, in degrees.
    /// \param in_fAspectRatio The aspect ratio of the screen (w/h).
    /// \param in_fNear The straight distance from the camera to the near plane.
    /// \param in_fFar The straight distance from the camera to the far plane.
    /// \note Learn to love your Z-Buffer: http://wiki.arkana-fts.org/doku.php?id=misc:zbuf
    /// \note If \a in_fFoV is too close to 0, 90 or 180 it will be set to 45.
    static General4x4Matrix perspectiveProjection(float in_fFoV, float in_fAspectRatio, float in_fNearPlane = 2.5f, float in_fFarPlane = 1000.0f);

    ///////////////////////////////////////
    // Conversion methods and operators. //
    ///////////////////////////////////////

    /// \return An General4x4Matrix representing the inverse of myself. (Having
    ///         myself as its inverse again.)
    General4x4Matrix inverse() const;

    ////////////////////////////
    // Matrix-Matrix product. //
    ////////////////////////////

    /// Returns the product of this matrix with another one. \a o gets
    /// multiplied on the right of this.
    /// \param o The other matrix that has to be multiplied from the right.
    /// \return The matrix resulting from *this * \a o.
    /// \note Of course, for the inverse the multiplication is done from the left.
    General4x4Matrix operator *(const General4x4Matrix& o) const;
    /// Multiplies this matrix with another one. \a o gets multiplied on the
    /// right of this.
    /// \param o The other matrix that has to be multiplied from the right.
    /// \note Of course, for the inverse the multiplication is done from the left.
    void operator *=(const General4x4Matrix& o);
};

#include "Matrix.inl"

} // namespace bouge

#endif // BOUGE_QUATERNION_H
