#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <utility>
#include <tuple>
#include <cmath>

template <typename T>
struct Vector2D
{
    T x{};
    T y{};
    Vector2D() = default;
    Vector2D(T xx, T yy) : x(xx), y(yy) {}
    Vector2D(const Vector2D<T>& vec) : x(vec.x), y(vec.y) {}

    template<typename V>
    Vector2D<T> operator+(const Vector2D<V> &another)
    {
        Vector2D<T> vec{x, y};

        vec += another;

        return vec;
    }

    template<typename V>
    Vector2D<T>& operator+=(const Vector2D<V> &another)
    {
        this->x += another.x;
        this->y += another.y;

        return *this;
    }

    template<typename V>
    Vector2D<T> operator-(const Vector2D<V> &another)
    {
        Vector2D<T> vec{x, y};

        vec -= another;

        return vec;
    }

    template<typename V>
    Vector2D<T>& operator-=(const Vector2D<V> &another)
    {
        this->x -= another.x;
        this->y -= another.y;

        return *this;
    }

    template<typename V>
    Vector2D<T> operator*(const Vector2D<V> &another)
    {
        Vector2D<T> vec{x, y};

        vec *= another;

        return vec;
    }

    template<typename V>
    Vector2D<T>& operator*=(const Vector2D<V> &another)
    {
        this->x *= another.x;
        this->y *= another.y;

        return *this;
    }

    template<typename V>
    Vector2D<T> operator/(const Vector2D<V> &another)
    {
        Vector2D<T> vec{x, y};

        vec /= another;

        return vec;
    }

    template<typename V>
    Vector2D<T>& operator/=(const Vector2D<V> &another)
    {
        this->x /= another.x;
        this->y /= another.y;

        return *this;
    }


    static Vector2D<T> full(T val)
    {
        return {val, val};
    }

    template <typename V>
    operator Vector2D<V>() const
    {
        return Vector2D<V>{static_cast<V>(x),
                           static_cast<V>(y)};
    }

    template<typename V>
    bool operator<(const Vector2D<V>& b) const
    {
        return std::tie(x, y) < std::tie(b.x, b.y);
    }

    template<typename V>
    bool operator<=(const Vector2D<V>& b) const
    {
        return std::tie(x, y) <= std::tie(b.x, b.y);
    }

    template<typename V>
    bool operator>(const Vector2D<V>& b) const
    {
        return std::tie(x, y) > std::tie(b.x, b.y);
    }

    template<typename V>
    bool operator>=(const Vector2D<V>& b) const
    {
        return std::tie(x, y) >= std::tie(b.x, b.y);
    }

    template<typename V>
    bool operator==(const Vector2D<V>& another)
    {
        return this->x == another.x && this->y == another.y;
    }

    template<typename V>
    bool operator!=(const Vector2D<V>& another)
    {
        return !(*this == another);
    }

    void Normalize()
    {
        float length = std::hypot(x, y);
        x /= length;
        y /= length;
    }

    template<typename U>
    static double crossProduct(const Vector2D<T>& vec1,
                               const Vector2D<U>& vec2)
    {
        return vec1.x * vec2.y - vec1.y * vec2.x;
    }
};

#endif // VECTOR2D_H
