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
#ifndef BOUGE_COREKEYFRAME_HPP
#define BOUGE_COREKEYFRAME_HPP

#include <bouge/bougefwd.hpp>

#include <bouge/Math/Vector.hpp>
#include <bouge/Math/Quaternion.hpp>
#include <bouge/Math/Matrix.hpp>

namespace bouge {

    class BOUGE_API CoreKeyframe
    {
    public:
        CoreKeyframe();
        CoreKeyframe(Quaternion rotation, Vector translation, Vector scale);
        virtual ~CoreKeyframe();

        BOUGE_USER_DATA;

        bool hasRotation() const;
        Quaternion rotation() const;
        CoreKeyframe& rotation(Quaternion q);

        bool hasTranslation() const;
        Vector translation() const;
        CoreKeyframe& translation(Vector v);

        bool hasScale() const;
        Vector scale() const;
        CoreKeyframe& scale(Vector v);

    private:
        Vector m_trans;
        Vector m_scale;
        Quaternion m_rot;

        bool m_bHasTrans;
        bool m_bHasScale;
        bool m_bHasRot;
    };

} // namespace bouge

#endif // BOUGE_COREKEYFRAME_HPP
