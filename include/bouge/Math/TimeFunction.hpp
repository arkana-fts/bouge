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
#ifndef BOUGE_TIMEFUNCTION_HPP
#define BOUGE_TIMEFUNCTION_HPP

#include <bouge/Config.hpp>

namespace bouge {

    /// This represents a one-dimensional time-dependent function.
    /// Its input is the time (0 = start, 1 = end) and it outputs a value at
    /// each time. It does \e not have to be continuous.
    class BOUGE_API TimeFunction
    {
    public:
        virtual float operator()(float t) = 0;
        virtual TimeFunction* clone() const = 0;
        virtual ~TimeFunction() {};
    };

    /// This is just a constant over time. Looks like a straight line at height c.
    class BOUGE_API ConstantTF : public TimeFunction
    {
    public:
        ConstantTF(float val);
        virtual float operator()(float t);
        virtual ConstantTF* clone() const;

        float val() const;
    private:
        float c;
    };

    /// This is a linear function, that is a line with a constant slope. Looks like this:\n
    ///  ^     /   \n
    ///  |    /    \n
    ///  |   /     \n
    ///  |  /      \n
    ///  | /       \n
    ///  |/        \n
    ///  |-----> t \n
    ///     1
    class BOUGE_API LinearTF : public TimeFunction
    {
    public:
        LinearTF(float slope);
        virtual float operator()(float t);
        virtual LinearTF* clone() const;

        float slope() const;
    private:
        float m_slope;
    };

    /// This is a repeated time function, that means it takes a time function,
    /// and when it gets to t > 1, it uses t's fractional part, i.e. it restarts.
    /// For a linear time function, it gets like this:\n
    ///  ^          \n
    ///  |  /  /    \n
    ///  | /  /     \n
    ///  |/  /  /   \n
    ///  |------> t \n
    ///     1
    class BOUGE_API RepeatTF : public TimeFunction
    {
    public:
        RepeatTF(TimeFunction* tf);
        virtual ~RepeatTF();
        virtual float operator()(float t);
        virtual RepeatTF* clone() const;

        const TimeFunction* tf() const;
    private:
        TimeFunction* m_tf;
    };

    /// This is a cycled time function, that is like the \a RepeatTF,
    /// but every odd piece is mirrored. Looks like this for a linear one:\n
    ///  ^          \n
    ///  |  /\      \n
    ///  | /  \     \n
    ///  |/    \/   \n
    ///  |------> t \n
    ///     1
    class BOUGE_API CycleTF : public TimeFunction
    {
    public:
        CycleTF(TimeFunction* tf);
        virtual ~CycleTF();
        virtual float operator()(float t);
        virtual CycleTF* clone() const;

        const TimeFunction* tf() const;
    private:
        TimeFunction* m_tf;
    };

    /// This is a time function, that holds the value it had at t=1 for all t>1.
    /// Looks like this in the linear example:\n
    ///  ^   ____   \n
    ///  |  /       \n
    ///  | /        \n
    ///  |/         \n
    ///  |------> t \n
    ///     1
    class BOUGE_API HoldTF : public TimeFunction
    {
    public:
        HoldTF(TimeFunction* tf, float valueFrom=1.0f);
        virtual ~HoldTF();
        virtual float operator()(float t);
        virtual HoldTF* clone() const;

        const TimeFunction* tf() const;
        float valueFrom() const;
    private:
        TimeFunction* m_tf;
        float m_valueFrom;
    };

    /// This makes the given function fade-in for a certain amount of time.
    /// It fades in linearly, that means a constant function becomes a ramp,
    /// a linear function gets a quadratic ramp, etc.\n
    /// For example, when applied to a constant function, it becomes a trapezoid,
    /// which (for a fade duration f) looks like this:\n
    ///  ^   ____   \n
    ///  |  /       \n
    ///  | /        \n
    ///  |/         \n
    ///  |------> t \n
    ///     f   1
    class BOUGE_API FadeInTF : public TimeFunction
    {
    public:
        FadeInTF(float fadeDuration, TimeFunction* other);
        virtual ~FadeInTF();
        virtual float operator()(float t);
        virtual FadeInTF* clone() const;

        const TimeFunction* tf() const;
        float fadeDuration() const;
    private:
        float m_fadeDuration;
        TimeFunction* m_other;
    };

    /// This makes the given function fade-out for a certain amount of time.
    /// It fades out linearly, that means a constant function becomes a ramp,
    /// a linear function gets a quadratic ramp, etc.\n
    /// For example, when applied to a constant function, it becomes a trapezoid,
    /// which (for a fade duration f) looks like this:\n
    ///  ^___       \n
    ///  |   \      \n
    ///  |    \     \n
    ///  |     \    \n
    ///  |------> t \n
    ///      f  1
    class BOUGE_API FadeOutTF : public TimeFunction
    {
    public:
        FadeOutTF(float fadeDuration, TimeFunction* other);
        FadeOutTF(float fadeStartpoint, float reachZeroPoint, TimeFunction* other);
        virtual ~FadeOutTF();
        virtual float operator()(float t);
        virtual FadeOutTF* clone() const;

        const TimeFunction* tf() const;
        float fadeDuration() const;
        float fadeStartpoint() const;
        float reachZeroPoint() const;
    private:
        float m_fadeStart;
        float m_reachZeroPoint;
        TimeFunction* m_other;
    };

} // namespace bouge

#endif // BOUGE_TIMEFUNCTION_HPP
