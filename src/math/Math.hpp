#ifndef ENGINE_MATH_HPP
#define ENGINE_MATH_HPP

#include <cmath>

namespace Math
{
    const float Pi = 3.1415926535f;
    const float Pi2 = Pi * 2.0f;
    const float Piover2 = Pi / 2.0f;

    template  <typename T>
    inline float Abs(T value)
    {
        return value < 0 ? - value : value;
    }

    template  <typename T>
    inline float Sqrt(T value)
    {
        return std::sqrt(value);
    }

    template  <typename T>
    inline float Sin(T t)
    {
        return std::sin(t);
    }

    template  <typename T>
    inline float Cos(T t)
    {
        return std::cos(t);
    }

    template  <typename T>
    inline float Tan(T t)
    {
        return std::tan(t);
    }

    template  <typename T>
    inline float DegToRad(T degrees)
    {
        return degrees * Pi / 180.0f;
    }

    template  <typename T>
    inline float RadToDeg(T radians)
    {
        return radians * 180.0f / Pi;
    }

    template  <typename T>
    inline bool NearZero(T value, float epsilon = 0.001f)
    {
        return Math::Abs(value) <= epsilon;
    }

    template <typename T>
    inline T Max(const T& a, const T& b)
    {
        return (a < b ? b : a);
    }

    template <typename T>
    inline T Min(const T& a, const T& b)
    {
        return (a < b ? a : b);
    }

    template  <typename T>
    inline float Lerp(T a, T b, float f)
    {
        return a + f * (b - a);
    }
}

#endif //ENGINE_MATH_HPP
