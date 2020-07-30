#pragma once

#ifndef UTIL_H
#define UTIL_H

#include <utility>
#include <ctime>
#include <cmath>
#include <type_traits>

#include "Constants.h"
#include "Vector2d.h"

class GameObject;

namespace util
{
    template<typename T>
    constexpr void check()
    {
        static_assert(std::is_arithmetic<T>::value,
                      "Works only with arithmetic types.");
    }

    template<typename T>
    inline bool withinRange(T arg, T min, T max)
    {
        return arg >= min && arg < max;
    }
    // Works only for positive min and max
    inline float clampLooping(float value, float min, float max) {
        int howManyTimesMax = value / max;
        return value >= 0 ? (value - (howManyTimesMax * max))
                          : max + (value - (howManyTimesMax * max));

    }

    template <typename T>
    inline T clamp(T value, T min, T max) {
        if(value > max) {
                return max;
        }
        if(value < min) {
                return min;
        }
        return value;
    }


    template <typename T>
    inline T uintPow(T value, unsigned power)
    {
        check<T>();

        if(power == 0) return T{1};

        T temp = value;
        while(--power) {
            temp *= value;
        }
        return temp;
    }

    inline float radToDeg(float rad)
    {
        return rad * (180.0f / Constants::pi);
    }

    inline float degToRad(float deg)
    {
        return deg * (Constants::pi / 180);
    }

    template <typename T>
    inline float vectorAngle(T x, T y)
    {
        float val = std::atan2(x, -y);
        return val < 0 ? Constants::pi * 2 + val : val;
    }

    inline float angleBetweenPoints(const Vector2D<float>& a,
                                    const Vector2D<float>& b)
    {
        Vector2D<float> delta = a;
        delta -= b;
        return vectorAngle(delta.x, delta.y);
    }

template <typename T>
    inline T interpolateValue(time_t frametime, T value)
    {
        check<T>();
        using namespace Constants;
        return value * ((T)frametime / targetSpeed);
    }

    inline Vector2D<float> lengthWithAngleToVector(float angle, float length)
    {

        return {std::sin(angle) * length,
                std::cos(angle) * length};
    }

    inline size_t twoIndeciesIntoOne(size_t i, size_t j, size_t width)
    {
        return i * width + j;
    }

    inline void oneIndexIntoTwo(size_t index, size_t width, size_t& i, size_t& j)
    {
        i = index / width;
        j = index - (index / width) * width;
    }

    template <typename T>
    inline bool withinArea(Vector2D<T> src, Vector2D<T> dest, float radius)
    {
        check<T>();

        T dx = src.x - dest.x;
        T dy = src.x - dest.x;

        float distance = std::hypot(dx, dy);

        if(distance < radius) {
            return true;
        }

        return false;
    }

    inline void mapToScreenSpace(Vector2D<float>& screen,
                                 const Vector2D<float>& screenSize,
                                 const Vector2D<float>& map,
                                 const Vector2D<float>& mapSize)
    {
        Vector2D<float> zeroOnScreen;

        zeroOnScreen.x = (mapSize.x - screenSize.x) / 2;
        zeroOnScreen.y = (mapSize.y - screenSize.y) / 2;


        screen.x = map.x - zeroOnScreen.x;
        screen.y = map.y - zeroOnScreen.y;
    }

    inline void mapToScreenSpaceAffectedByCamera(Vector2D<float>& screen,
                                                 const Vector2D<float>& screenSize,
                                                 const Vector2D<float>& map,
                                                 const Vector2D<float>& camera)
    {
        Vector2D<float> zeroOnScreen;

        zeroOnScreen.x = camera.x - screenSize.x / 2;
        zeroOnScreen.y = camera.y - screenSize.y / 2;


        screen.x = map.x - zeroOnScreen.x;
        screen.y = map.y - zeroOnScreen.y;
    }

    template<typename T, typename V>
    inline bool offMap(const Vector2D<T>& point, const Vector2D<V>& mapSize)
    {
        check<T>();
        check<V>();
        if(point.x <  (T)0         || point.y <  (T)0        ||
           point.x >= (T)mapSize.x || point.y >= (T)mapSize.y) return true;
        return false;
    }

    Vector2D<float> getCamera(GameObject& obj,
                              Vector2D<int> screenSize,
                              Vector2D<int> mapBoundaries);
}

#endif // UTIL_H
