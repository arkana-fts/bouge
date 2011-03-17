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
// Vector serialization. //
///////////////////////////

/// Serializes a vector to a file. Use it like this: f << v1 << v2 << etc.
/// \param f The file to write the vector to.
/// \param v The vector to write to the file.
/// \return a reference to the file to allow chaining.
template<class T>
T& operator<<(T& f, const bouge::Vector& v) {
    f << v.x() << " " << v.y() << " " << v.z();
    return f;
}

/// Extracts a vector from a file. Use it like this: f >> v1 >> v2 >> etc.
/// \param f The file to read the vector from.
/// \param v The vector to write the read values to.
/// \return a reference to the file to allow chaining.
template<class T>
T& operator>>(T& f, bouge::Vector& v) {
    f >> v[0] >> v[1] >> v[2];
    return f;
}

template<class FloatIterator>
Vector::Vector(FloatIterator in_begin, const FloatIterator& in_end)
    : m_v(4)
{
    FloatIterator iter = in_begin;
    for(int i = 0 ; i < 4 && iter != in_end ; ++i, ++iter) {
        m_v[i] = *iter;
    }
}
