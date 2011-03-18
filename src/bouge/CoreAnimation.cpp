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
#include <bouge/CoreAnimation.hpp>
#include <bouge/CoreTrack.hpp>
#include <bouge/Math/TimeFunction.hpp>

#include <stdexcept>

namespace bouge {

    CoreAnimation::CoreAnimation(std::string name, TimeFunction* preferredControl)
        : m_sName(name)
        , m_preferredControl(preferredControl)
    {
        if(!m_preferredControl)
            m_preferredControl = new RepeatTF(new LinearTF(1.0f));
    }

    CoreAnimation::~CoreAnimation()
    {
        delete m_preferredControl;
    }

    std::string CoreAnimation::name() const
    {
        return m_sName;
    }

    CoreAnimation& CoreAnimation::name(std::string name)
    {
        m_sName = name;
        return *this;
    }

    float CoreAnimation::duration() const
    {
        float duration = 0.0f;

        /// \TODO: If this indeed is a bottleneck (MEASURE!), we might cache that value and add a "recalc" method.
        for(const_iterator iTrack = this->begin() ; iTrack != this->end() ; ++iTrack) {
            duration = std::max(duration, iTrack->duration());
        }

        return duration;
    }

    const TimeFunction* CoreAnimation::preferredControl() const
    {
        return m_preferredControl;
    }

    bool CoreAnimation::hasTrack(const std::string& bone)
    {
        return m_tracks.find(bone) != m_tracks.end();
    }

    CoreTrackPtr CoreAnimation::track(const std::string& bone)
    {
        TrackMap::iterator i = m_tracks.find(bone);
        if(i == m_tracks.end())
            throw std::invalid_argument("No track for the bone '" + bone + "' present in the animation '" + this->name() + "'");

        return i->second;
    }

    const CoreTrackPtr CoreAnimation::track(const std::string& bone) const
    {
        TrackMap::const_iterator i = m_tracks.find(bone);
        if(i == m_tracks.end())
            throw std::invalid_argument("No track for the bone '" + bone + "' present in the animation '" + this->name() + "'");

        return i->second;
    }

    CoreAnimation& CoreAnimation::track(const std::string& bone, CoreTrackPtr track)
    {
        m_tracks[bone] = track;
        return *this;
    }

    std::size_t CoreAnimation::trackCount() const
    {
        return m_tracks.size();
    }

    CoreAnimation::iterator::iterator(TrackMap::iterator me)
        : myIter(me)
    { }

    CoreAnimation::iterator::iterator()
    { }

    CoreAnimation::iterator::~iterator()
    { }

    bool CoreAnimation::iterator::operator==(iterator other) const
    {
        return myIter == other.myIter;
    }

    bool CoreAnimation::iterator::operator!=(iterator other) const
    {
        return myIter != other.myIter;
    }

    CoreAnimation::iterator& CoreAnimation::iterator::operator++()
    {
        return ++myIter, *this;
    }

    CoreAnimation::iterator CoreAnimation::iterator::operator++(int)
    {
        return iterator(myIter++);
    }

    CoreAnimation::iterator& CoreAnimation::iterator::operator--()
    {
        return --myIter, *this;
    }

    CoreAnimation::iterator CoreAnimation::iterator::operator--(int)
    {
        return iterator(myIter--);
    }

    std::string CoreAnimation::iterator::bone() const
    {
        return myIter->first;
    }

    CoreTrackPtr CoreAnimation::iterator::track()
    {
        return myIter->second;
    }

    CoreTrack* CoreAnimation::iterator::operator->()
    {
        return myIter->second.operator->();
    }

    CoreAnimation::iterator CoreAnimation::begin()
    {
        return iterator(m_tracks.begin());
    }

    CoreAnimation::iterator CoreAnimation::end()
    {
        return iterator(m_tracks.end());
    }

    CoreAnimation::const_iterator::const_iterator(TrackMap::const_iterator me)
        : myIter(me)
    { }

    CoreAnimation::const_iterator::const_iterator()
    { }

    CoreAnimation::const_iterator::~const_iterator()
    { }

    bool CoreAnimation::const_iterator::operator==(const_iterator other) const
    {
        return myIter == other.myIter;
    }

    bool CoreAnimation::const_iterator::operator!=(const_iterator other) const
    {
        return myIter != other.myIter;
    }

    CoreAnimation::const_iterator& CoreAnimation::const_iterator::operator++()
    {
        return ++myIter, *this;
    }

    CoreAnimation::const_iterator CoreAnimation::const_iterator::operator++(int)
    {
        return const_iterator(myIter++);
    }

    CoreAnimation::const_iterator& CoreAnimation::const_iterator::operator--()
    {
        return --myIter, *this;
    }

    CoreAnimation::const_iterator CoreAnimation::const_iterator::operator--(int)
    {
        return const_iterator(myIter--);
    }

    std::string CoreAnimation::const_iterator::bone() const
    {
        return myIter->first;
    }

    CoreTrackPtrC CoreAnimation::const_iterator::track() const
    {
        return myIter->second;
    }

    const CoreTrack* CoreAnimation::const_iterator::operator->() const
    {
        return myIter->second.operator->();
    }

    CoreAnimation::const_iterator CoreAnimation::begin() const
    {
        return const_iterator(m_tracks.begin());
    }

    CoreAnimation::const_iterator CoreAnimation::end() const
    {
        return const_iterator(m_tracks.end());
    }

}
