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
#include <bouge/CoreTrack.hpp>
#include <bouge/CoreKeyframe.hpp>
#include <bouge/Math/Util.hpp>

namespace bouge {

    CoreTrack::CoreTrack()
        : m_duration(0.0f)
        , m_bHasTranslation(false)
        , m_bHasRotation(false)
        , m_bHasScale(false)
    { }

    CoreTrack::~CoreTrack()
    { }

    bool CoreTrack::hasRotation() const
    {
        return m_bHasRotation;
    }

    Quaternion CoreTrack::rotation(float time) const
    {
        if(!this->hasRotation())
            return Quaternion();

        const_iterator to = this->after(time);
        const_iterator from = to;
        if(from != this->begin())
            --from;

        // Border cases... For example only 1 keyframe
        if(from == to) {
            return from->rotation();
        }

        // Note: We really DO need slerp here, instead of nlerp, in the case
        // between goes beyond 1.0 (that is, we extrapolate).
        float between = (time - from.time())/(to.time() - from.time());
        return from->rotation().slerp(to->rotation(), between);
    }

    bool CoreTrack::hasTranslation() const
    {
        return m_bHasTranslation;
    }

    Vector CoreTrack::translation(float time) const
    {
        if(!this->hasTranslation())
            return Vector();

        const_iterator to = this->after(time);
        const_iterator from = to;
        if(from != this->begin())
            --from;

        // Border cases... For example only 1 keyframe
        if(from == to) {
            return from->translation();
        }

        float between = (time - from.time())/(to.time() - from.time());
        return from->translation().lerp(to->translation(), between);
    }

    bool CoreTrack::hasScale() const
    {
        return m_bHasScale;
    }

    Vector CoreTrack::scale(float time) const
    {
        if(!this->hasScale())
            return Vector(1.0f, 1.0f, 1.0f);

        const_iterator to = this->after(time);
        const_iterator from = to;
        if(from != this->begin())
            --from;

        // Border cases... For example only 1 keyframe
        if(from == to) {
            return from->scale();
        }

        float between = (time - from.time())/(to.time() - from.time());
        return from->scale().lerp(to->scale(), between);
    }

    /// \TODO: probably, there is no more need for this (maybe bugged?) method by now.
    AffineMatrix CoreTrack::transform(float time) const
    {
        AffineMatrix ret; // NRVO

        const_iterator to = this->after(time);
        const_iterator from = to;

        // Second condition is, for now, to "hold" the last frame if time is past the end.
        if(from != this->begin() && time <= to.time())
            --from;

        // Border cases... For example only 1 keyframe
        if(from == to) {
            // NRVO
            ret = AffineMatrix::translation(from->translation()) * AffineMatrix::scale(from->scale()) * AffineMatrix::rotation(from->rotation());
            return ret;
        }

        float interval = to.time() - from.time();
        float between = (time - from.time())/interval;

        /// \TODO: Verify this

        if(this->hasTranslation()) {
            ret = AffineMatrix::translation(from->translation().lerp(to->translation(), between));
        }

        if(this->hasScale()) {
            ret *= AffineMatrix::scale(from->scale().lerp(to->scale(), between));
        }

        if(this->hasRotation()) {
            ret *= AffineMatrix::rotation(from->rotation().nlerp(to->rotation(), between));
        }

        return ret;
    }

    CoreTrack& CoreTrack::add(float time, CoreKeyframePtr keyframe)
    {
        m_keyframes.insert(std::make_pair(time, keyframe));

        if(keyframe->hasTranslation())
            m_bHasTranslation = true;

        if(keyframe->hasRotation())
            m_bHasRotation = true;

        if(keyframe->hasScale())
            m_bHasScale = true;

        return this->updateDuration();
    }

    CoreTrack& CoreTrack::remove(iterator who)
    {
        m_keyframes.erase(who.myIter);
        return this->updateDuration();
    }

    // If there are several inside [time-tolerance ; time+tolerance], all are removed.
    CoreTrack& CoreTrack::removeAllIn(float start, float end)
    {
        m_keyframes.erase(m_keyframes.lower_bound(start), m_keyframes.upper_bound(end));
        return this->updateDuration();
    }

    std::size_t CoreTrack::keyframeCount() const
    {
        return m_keyframes.size();
    }

    float CoreTrack::duration() const
    {
        return m_duration;
    }

    CoreTrack& CoreTrack::updateDuration()
    {
        std::map<float, CoreKeyframePtr>::reverse_iterator i = m_keyframes.rbegin();
        m_duration = i->first;
        return *this;
    }

    CoreTrack::iterator::iterator(KeyframeMap::iterator me)
        : myIter(me)
    { }

    CoreTrack::iterator::iterator()
    { }

    CoreTrack::iterator::~iterator()
    { }

    bool CoreTrack::iterator::operator==(iterator other) const
    {
        return myIter == other.myIter;
    }

    bool CoreTrack::iterator::operator!=(iterator other) const
    {
        return myIter != other.myIter;
    }

    CoreTrack::iterator& CoreTrack::iterator::operator++()
    {
        return ++myIter, *this;
    }

    CoreTrack::iterator CoreTrack::iterator::operator++(int)
    {
        return iterator(myIter++);
    }

    CoreTrack::iterator& CoreTrack::iterator::operator--()
    {
        return --myIter, *this;
    }

    CoreTrack::iterator CoreTrack::iterator::operator--(int)
    {
        return iterator(myIter--);
    }

    float CoreTrack::iterator::time() const
    {
        return myIter->first;
    }

    CoreKeyframePtr CoreTrack::iterator::keyframe()
    {
        return myIter->second;
    }

    CoreKeyframe* CoreTrack::iterator::operator->()
    {
        return myIter->second.operator->();
    }

    CoreTrack::iterator CoreTrack::begin()
    {
        return iterator(m_keyframes.begin());
    }

    CoreTrack::iterator CoreTrack::closest(float time)
    {
        iterator i(m_keyframes.lower_bound(time));

        // i is now either the exact element at time (unlikely), or the next one.
        // Just need to check if i or the previous one i-1 is closer to time.

        // border case 1
        if(i == this->begin()) {
            return i;
        }

        // border case 2
        if(i == this->end()) {
            return --i;
        }

        iterator prev = i;
        --prev;

        float distI = i.time() - time;
        float distPrev = time - i.time();

        if(distI <= distPrev) {
            return i;
        } else {
            return prev;
        }
    }

    CoreTrack::iterator CoreTrack::after(float time)
    {
        iterator i(m_keyframes.upper_bound(time));
        return (i == this->end() ? --i : i);
    }

    CoreTrack::iterator CoreTrack::end()
    {
        return iterator(m_keyframes.end());
    }

    CoreTrack::const_iterator::const_iterator(KeyframeMap::const_iterator me)
        : myIter(me)
    { }

    CoreTrack::const_iterator::const_iterator()
    { }

    CoreTrack::const_iterator::~const_iterator()
    { }

    bool CoreTrack::const_iterator::operator==(const_iterator other) const
    {
        return myIter == other.myIter;
    }

    bool CoreTrack::const_iterator::operator!=(const_iterator other) const
    {
        return myIter != other.myIter;
    }

    CoreTrack::const_iterator& CoreTrack::const_iterator::operator++()
    {
        return ++myIter, *this;
    }

    CoreTrack::const_iterator CoreTrack::const_iterator::operator++(int)
    {
        return const_iterator(myIter++);
    }

    CoreTrack::const_iterator& CoreTrack::const_iterator::operator--()
    {
        return --myIter, *this;
    }

    CoreTrack::const_iterator CoreTrack::const_iterator::operator--(int)
    {
        return const_iterator(myIter--);
    }

    float CoreTrack::const_iterator::time() const
    {
        return myIter->first;
    }

    CoreKeyframePtrC CoreTrack::const_iterator::keyframe() const
    {
        return myIter->second;
    }

    const CoreKeyframe* CoreTrack::const_iterator::operator->() const
    {
        return myIter->second.operator->();
    }

    CoreTrack::const_iterator CoreTrack::begin() const
    {
        return const_iterator(m_keyframes.begin());
    }

    CoreTrack::const_iterator CoreTrack::closest(float time) const
    {
        const_iterator i(m_keyframes.lower_bound(time));

        // i is now either the exact element at time (unlikely), or the next one.
        // Just need to check if i or the previous one i-1 is closer to time.

        // border case 1
        if(i == this->begin()) {
            return i;
        }

        // border case 2
        if(i == this->end()) {
            return --i;
        }

        const_iterator prev = i;
        --prev;

        float distI = i.time() - time;
        float distPrev = time - i.time();

        if(distI <= distPrev) {
            return i;
        } else {
            return prev;
        }
    }

    CoreTrack::const_iterator CoreTrack::after(float time) const
    {
        const_iterator i(m_keyframes.upper_bound(time));
        return (i == this->end() ? --i : i);
    }

    CoreTrack::const_iterator CoreTrack::end() const
    {
        return const_iterator(m_keyframes.end());
    }

}
