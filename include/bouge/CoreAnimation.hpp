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
#ifndef BOUGE_COREANIMATION_HPP
#define BOUGE_COREANIMATION_HPP

#include <bouge/bougefwd.hpp>

#include <string>
#include <map>

namespace bouge {

    class BOUGE_API CoreAnimation
    {
        typedef std::map<std::string, CoreTrackPtr> TrackMap;
    public:
        CoreAnimation(std::string name, TimeFunction* preferredControl = 0);
        virtual ~CoreAnimation();

        BOUGE_USER_DATA;

        std::string name() const;
        CoreAnimation& name(std::string name);

        float duration() const;
        const TimeFunction* preferredControl() const;

        bool hasTrack(const std::string& bone);
        CoreTrackPtr track(const std::string& bone);
        const CoreTrackPtr track(const std::string& bone) const;
        CoreAnimation& track(const std::string& bone, CoreTrackPtr track);
        std::size_t trackCount() const;

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
            std::string bone() const;
            CoreTrackPtr track();
            CoreTrack* operator->();
        private:
            friend class CoreAnimation;
            iterator(TrackMap::iterator me);
            TrackMap::iterator myIter;
        };

        /// \return An iterator pointing at the first track.
        iterator begin();
        /// \return An iterator pointing behind the last track.
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
            std::string bone() const;
            CoreTrackPtrC track() const;
            const CoreTrack* operator->() const;
        private:
            friend class CoreAnimation;
            const_iterator(TrackMap::const_iterator me);
            TrackMap::const_iterator myIter;
        };

        /// \return An iterator pointing at the first track.
        const_iterator begin() const;
        /// \return An iterator pointing behind the last track.
        const_iterator end() const;

    private:
        std::string m_sName;
        TimeFunction* m_preferredControl;
        TrackMap m_tracks;
    };

} // namespace bouge

#endif // BOUGE_COREANIMATION_HPP
