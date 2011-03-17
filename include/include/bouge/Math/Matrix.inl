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

///////////////////////////
// Matrix serialization. //
///////////////////////////

/// Serializes the affine matrix along with its inverse to a file.
/// Use it like this: f << m1 << m2 << etc.
/// \param f The file to write the matrix to.
/// \param m The matrix to write to the file.
/// \return a reference to the file to allow chaining.
template<class T>
T& operator<<(T& f, const bouge::AffineMatrix& m) {
    for(unsigned int i = 0 ; i < 16 ; ++i)
        f << m.array16f()[i] << " ";
    for(unsigned int j = 0 ; j < 4 ; ++j)
        for(unsigned int i = 0 ; i < 3 ; ++i)
            f << m.array16fInverse()[(j*4)+i] << " ";

    return f;
}

/// Extracts the affine matrix along with its inverse from a file.
/// Use it like this: f >> m1 >> m2 >> etc.
/// \param f The file to read the matrix from.
/// \param m The matrix to write the read values to.
/// \return a reference to the file to allow chaining.
template<class T>
T& operator>>(T& f, bouge::AffineMatrix& m) {
    for(unsigned int i = 0 ; i < 16 ; ++i)
        f >> m.m[i];
    for(unsigned int j = 0 ; j < 4 ; ++j)
        for(unsigned int i = 0 ; i < 3 ; ++i)
            f >> m.im[(j*4)+i];

    m.im[3] = m.im[7] = m.im[11] = 0.0f;
    m.im[15] = 1.0f;

    // 3x3 parts are just copied over.

    m.m3[0] = m.m[0]; m.m3[3] = m.m[4]; m.m3[6] = m.m[8];
    m.m3[1] = m.m[1]; m.m3[4] = m.m[5]; m.m3[7] = m.m[9];
    m.m3[2] = m.m[2]; m.m3[5] = m.m[6]; m.m3[8] = m.m[10];

    m.im3[0] = m.im[0]; m.im3[3] = m.im[4]; m.im3[6] = m.im[8];
    m.im3[1] = m.im[1]; m.im3[4] = m.im[5]; m.im3[7] = m.im[9];
    m.im3[2] = m.im[2]; m.im3[5] = m.im[6]; m.im3[8] = m.im[10];

    return f;
}

/// Serializes the general 4x4 matrix along with its inverse to a file.
/// Use it like this: f << m1 << m2 << etc.
/// \param f The file to write the matrix to.
/// \param m The matrix to write to the file.
/// \return a reference to the file to allow chaining.
template<class T>
T& operator<<(T& f, const bouge::General4x4Matrix& m) {
    for(unsigned int i = 0 ; i < 16 ; ++i)
        f << m.array16f()[i] << " ";
    for(unsigned int i = 0 ; i < 16 ; ++i)
        f << m.array16fInverse()[i] << " ";

    return f;
}

/// Extracts the general 4x4 matrix along with its inverse from a file.
/// Use it like this: f >> m1 >> m2 >> etc.
/// \param f The file to read the matrix from.
/// \param m The matrix to write the read values to.
/// \return a reference to the file to allow chaining.
template<class T>
T& operator>>(T& f, bouge::General4x4Matrix& m) {
    for(unsigned int i = 0 ; i < 16 ; ++i)
        f >> m.m[i];
    for(unsigned int i = 0 ; i < 16 ; ++i)
        f >> m.im[i];

    return f;
}
