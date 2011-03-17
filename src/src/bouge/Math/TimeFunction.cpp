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
#include <bouge/Math/TimeFunction.hpp>

#include <algorithm>

namespace bouge {

    ConstantTF::ConstantTF(float val)
        : c(val)
    { }

    float ConstantTF::operator()(float t)
    {
        return c;
    }

    ConstantTF* ConstantTF::clone() const
    {
        return new ConstantTF(this->val());
    }

    float ConstantTF::val() const
    {
        return c;
    }

    LinearTF::LinearTF(float slope)
        : m_slope(slope)
    { }

    float LinearTF::operator()(float t)
    {
        return m_slope*t;
    }

    LinearTF* LinearTF::clone() const
    {
        return new LinearTF(this->slope());
    }

    float LinearTF::slope() const
    {
        return m_slope;
    }

    RepeatTF::RepeatTF(TimeFunction* tf)
        : m_tf(tf)
    { }

    RepeatTF::~RepeatTF()
    {
        delete m_tf;
    }

    float RepeatTF::operator()(float t)
    {
        float fractional = t - (float)((int)t);
        return (*m_tf)(fractional);
    }

    RepeatTF* RepeatTF::clone() const
    {
        return new RepeatTF(this->tf()->clone());
    }

    const TimeFunction* RepeatTF::tf() const
    {
        return m_tf;
    }

    CycleTF::CycleTF(TimeFunction* tf)
        : m_tf(tf)
    { }

    CycleTF::~CycleTF()
    {
        delete m_tf;
    }

    float CycleTF::operator()(float t)
    {
        bool fwd = (int)t % 2 == 0;
        float fractional = t - (float)((int)t);
        return fwd ? (*m_tf)(fractional) : (*m_tf)(1.0f-fractional);
    }

    CycleTF* CycleTF::clone() const
    {
        return new CycleTF(this->tf()->clone());
    }

    const TimeFunction* CycleTF::tf() const
    {
        return m_tf;
    }

    HoldTF::HoldTF(TimeFunction* tf, float valueFrom)
        : m_tf(tf)
        , m_valueFrom(valueFrom)
    { }

    HoldTF::~HoldTF()
    {
        delete m_tf;
    }

    float HoldTF::operator()(float t)
    {
        if(t <= 1.0f)
            return (*m_tf)(t);
        else
            return (*m_tf)(m_valueFrom);
    }

    HoldTF* HoldTF::clone() const
    {
        return new HoldTF(this->tf()->clone(), this->valueFrom());
    }

    const TimeFunction* HoldTF::tf() const
    {
        return m_tf;
    }

    float HoldTF::valueFrom() const
    {
        return m_valueFrom;
    }

    FadeInTF::FadeInTF(float fadeDuration, TimeFunction* other)
        : m_fadeDuration(fadeDuration)
        , m_other(other)
    { }

    FadeInTF::~FadeInTF()
    {
        delete m_other;
    }

    float FadeInTF::operator()(float t)
    {
        bool inFade = t < m_fadeDuration;
        float f = (*m_other)(t);

        return inFade ? (f*t/m_fadeDuration) : (f);
    }

    FadeInTF* FadeInTF::clone() const
    {
        return new FadeInTF(this->fadeDuration(), this->tf()->clone());
    }

    const TimeFunction* FadeInTF::tf() const
    {
        return m_other;
    }

    float FadeInTF::fadeDuration() const
    {
        return m_fadeDuration;
    }

    FadeOutTF::FadeOutTF(float fadeDuration, TimeFunction* other)
        : m_fadeStart(1.0f-fadeDuration)
        , m_reachZeroPoint(1.0f)
        , m_other(other)
    { }

    FadeOutTF::FadeOutTF(float fadeStartpoint, float reachZeroPoint, TimeFunction* other)
        : m_fadeStart(fadeStartpoint)
        , m_reachZeroPoint(reachZeroPoint)
        , m_other(other)
    { }

    FadeOutTF::~FadeOutTF()
    {
        delete m_other;
    }

    float FadeOutTF::operator()(float t)
    {
        bool inFade = t > m_fadeStart;
        bool out = t >= m_reachZeroPoint;
        float f = (*m_other)(t);

        return out ? 0.0f : (inFade ? (f*(m_reachZeroPoint-t)/(m_reachZeroPoint-m_fadeStart)) : (f));
    }

    FadeOutTF* FadeOutTF::clone() const
    {
        return new FadeOutTF(this->fadeStartpoint(), this->reachZeroPoint(), this->tf()->clone());
    }

    const TimeFunction* FadeOutTF::tf() const
    {
        return m_other;
    }

    float FadeOutTF::fadeDuration() const
    {
        return 1.0f-m_fadeStart;
    }

    float FadeOutTF::fadeStartpoint() const
    {
        return m_fadeStart;
    }

    float FadeOutTF::reachZeroPoint() const
    {
        return m_reachZeroPoint;
    }

} // namespace bouge
