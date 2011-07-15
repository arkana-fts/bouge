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
#ifndef BOUGE_CORETRACK_HPP
#define BOUGE_CORETRACK_HPP

#include <bouge/bougefwd.hpp>

#include <bouge/Math/Matrix.hpp>
#include <bouge/Math/Quaternion.hpp>
#include <bouge/Math/Vector.hpp>

#include <map>

namespace bouge {

    class BOUGE_API CoreTrack
    {
        typedef std::map<float, CoreKeyframePtr> KeyframeMap;
    public:
        CoreTrack();
        virtual ~CoreTrack();

        BOUGE_USER_DATA;

        bool hasRotation() const;
        Quaternion rotation(float time) const;
        bool hasTranslation() const;
        Vector translation(float time) const;
        bool hasScale() const;
        Vector scale(float time) const;

        std::size_t keyframeCount() const;
        float duration() const;

        class BOUGE_API iterator {
        public:
            iterator();
            ~iterator();

            bool operator==(iterator other) const;
            bool operator!=(iterator other) const;
            iterator& operator++();
            iterator operator++(int);
            iterator& operator--();
            iterator operator--(int);
            float time() const;
            CoreKeyframePtr keyframe();
            CoreKeyframe* operator->();
        private:
            friend class CoreTrack;
            iterator(KeyframeMap::iterator me);
            KeyframeMap::iterator myIter;
        };

        /// \return An iterator pointing at the first keyframe.
        iterator begin();

        /// Get the keyframe that is the closest to a given \a time.
        /// \note If the given \a time is behind the last keyframe or in front of
        ///       the first keyframe, the last or the first keyframe is returned
        ///       respectively.
        /// \param time The time to which you want to get the closest keyframe.
        /// \return An iterator pointing at the closes keyframe to \a time.
        iterator closest(float time);

        /// Get the first keyframe that is strictly greater than \a time.
        /// \param time The time after which the keyframe should be.
        /// \return The first keyframe encountered right after \a time.
        iterator after(float time);

        /// \return An iterator pointing behind the last keyframe.
        iterator end();

        class BOUGE_API const_iterator {
        public:
            const_iterator();
            ~const_iterator();

            bool operator==(const_iterator other) const;
            bool operator!=(const_iterator other) const;
            const_iterator& operator++();
            const_iterator operator++(int);
            const_iterator& operator--();
            const_iterator operator--(int);
            float time() const;
            CoreKeyframePtrC keyframe() const;
            const CoreKeyframe* operator->() const;
        private:
            friend class CoreTrack;
            const_iterator(KeyframeMap::const_iterator me);
            KeyframeMap::const_iterator myIter;
        };

        /// \return An iterator pointing at the first keyframe.
        const_iterator begin() const;

        /// Get the keyframe that is the closest to a given \a time.
        /// \note If the given \a time is behind the last keyframe or in front of
        ///       the first keyframe, the last or the first keyframe is returned
        ///       respectively.
        /// \param time The time to which you want to get the closest keyframe.
        /// \return An iterator pointing at the closes keyframe to \a time.
        const_iterator closest(float time) const;

        /// Get the first keyframe that is strictly greater than \a time.
        /// \param time The time after which the keyframe should be.
        /// \return The first keyframe encountered right after \a time.
        const_iterator after(float time) const;

        /// \return An iterator pointing behind the last keyframe.
        const_iterator end() const;

        CoreTrack& add(float time, CoreKeyframePtr keyframe);
        CoreTrack& remove(iterator who);
        CoreTrack& removeAllIn(float start, float end);

        // We could add crazy stuff like compression, collapsing close keyframes, ...
        // but we'll only do this when really needed by someone...

    protected:
        CoreTrack& updateDuration();

    private:
        KeyframeMap m_keyframes;
        float m_duration;

        const_iterator closestInImpl(float time, float tolerance) const;
        const_iterator closestImpl(float time) const;

        bool m_bHasTranslation;
        bool m_bHasRotation;
        bool m_bHasScale;
    };

} // namespace bouge

#endif // BOUGE_CORETRACK_HPP
