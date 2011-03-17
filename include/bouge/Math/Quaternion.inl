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

///////////////////////////////
// Quaternion serialization. //
///////////////////////////////

/// Serializes a quaternion to a file. Use it like this: f << q1 << q2 << etc.
/// \param f The file to write the quaternion to.
/// \param q The quaternion to write to the file.
/// \return a reference to the file to allow chaining.
template<class T>
T& operator<<(T& f, const bouge::Quaternion& v) {
    f << v.x() << " " << v.y() << " " << v.z() << " " << v.w();
    return f;
}

/// Extracts a quaternion from a file. Use it like this: f >> q1 >> q2 >> etc.
/// \param f The file to read the quaternion from.
/// \param q The quaternion to write the read values to.
/// \return a reference to the file to allow chaining.
template<class T>
T& operator>>(T& f, bouge::Quaternion& v) {
    float x = 0.0f, y = 0.0f, z = 0.0f, w = 1.0f;
    f >> x >> y >> z >> w;
    v.x(x).y(y).z(z).w(w);
    return f;
}

template<class FloatIterator>
Quaternion::Quaternion(FloatIterator in_begin, const FloatIterator& in_end)
    : m_q(4)
{
    FloatIterator iter = in_begin;
    for(int i = 0 ; i < 4 && iter != in_end ; ++i, ++iter) {
        m_q[i] = *iter;
    }
}
