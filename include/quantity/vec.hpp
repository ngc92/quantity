#ifndef SPACE_GAME_PHYSICS_VEC_HPP
#define SPACE_GAME_PHYSICS_VEC_HPP

#include <iosfwd>
#include <cmath>
#include <tuple>  // for std::tie

namespace quantity {

    /*! \brief Minimal class for a three dimensional vector.
     *  \details This class is templated (so we can add units to
     *  the quantities inside the vector) and works header-only.
     */
    template<class T>
    struct Vec3
    {
        /// default constructor initializes to all zero.
        constexpr Vec3() = default;

        /// \brief heterogeneous constructor.
        /// \details This allows us to write eg. vec<double>(int, float, double).
        template<class A, class B, class C>
        constexpr Vec3(A&& x_, B&& y_, C&& z_) : x(x_), y(y_), z(z_)
        {}

        /// conversion constructor from a vector of convertable types.
        template<class S>
        constexpr explicit Vec3(const Vec3<S>& o) : x(T(o.x)), y(T(o.y)), z(T(o.z))
        {}

        // the coordinates
        T x{0};
        T y{0};
        T z{0};

        /// apply a function to all components and construct new vector.
        template<class F>
        auto map(F&& f) const;
    };

    template<class T>
    constexpr Vec3<T> make_vector(const T& x, const T& y, const T& z) {
        return Vec3<T>(x, y, z);
    }

    // operators
    template<class T, class S>
    constexpr auto operator+(const Vec3<T>& a, const Vec3<S>& b)
    {
        return make_vector(a.x + b.x, a.y + b.y, a.z + b.z);
    }

    template<class T, class S>
    constexpr auto operator-(const Vec3<T>& a, const Vec3<S>& b)
    {
        return make_vector(a.x - b.x, a.y - b.y, a.z - b.z);
    }

    template<class T, class S>
    constexpr auto operator*(const Vec3<T>& a, S&& s) -> decltype(make_vector(a.x * s, a.y * s, a.z * s))
    {
        return make_vector(a.x * s, a.y * s, a.z * s);
    }

    template<class T, class S>
    constexpr auto operator/(const Vec3<T>& a, S&& s)
    {
        return make_vector(a.x / s, a.y / s, a.z / s);
    }

    template<class S, class T>
    constexpr auto operator*(S&& s, const Vec3<T>& a) -> decltype(make_vector(s * a.x, s * a.y, s * a.z))
    {
        return make_vector(s * a.x, s * a.y, s * a.z);
    }

    template<class T, class S>
    Vec3<T>& operator+=(Vec3<T>& a, const Vec3<S>& b)
    {
        a.x += b.x;
        a.y += b.y;
        a.z += b.z;
        return a;
    }

    template<class T, class S>
    Vec3<T>& operator-=(Vec3<T>& a, const Vec3<S>& b)
    {
        a.x -= b.x;
        a.y -= b.y;
        a.z -= b.z;
        return a;
    }

    template<class T, class S>
    Vec3<T>& operator*=(Vec3<T>& a, const S& s)
    {
        a.x *= s;
        a.y *= s;
        a.z *= s;
        return a;
    }

    template<class T, class S>
    constexpr bool operator==(const Vec3<T>& a, const Vec3<S>& b)
    {
        return std::tie(a.x, a.y, a.z) == std::tie(b.x, b.y, b.z);
    }

    template<class T, class S>
    constexpr bool operator!=(const Vec3<T>& a, const Vec3<S>& b)
    {
        return !(a==b);
    }

    // utility functions
    template<class T, class S>
    auto dot(const Vec3<T>& a, const Vec3<S>& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    template<class T, class S>
    auto cross(const Vec3<T>& a, const Vec3<S>& b)
    {
        return make_vector(a.y * b.z - a.z * b.y,
                           a.z * b.x - a.x * b.z,
                           a.x * b.y - b.x * a.y);
    }

    template<class T>
    T length(const Vec3<T>& vec)
    {
        using std::sqrt;
        return sqrt(dot(vec, vec));
    }

    template<class T, class S>
    auto parallel(const Vec3<T>& source, const Vec3<S>& reference)
    {
        auto s = dot(reference, reference);
        auto f = dot(source, reference);
        if(s == S(0) * S(0))
        {
            // everything is parallel to the null vector
            return source;
        }

        return (f / s) * reference;
    }

    template<class T, class S>
    auto perpendicular(const Vec3<T>& source, const Vec3<S>& reference)
    {
        if(reference == Vec3<S>{0,0,0})
            return source;

        return source - parallel(source, reference);
    }

    template<class T>
    template<class F>
    auto Vec3<T>::map(F&& f) const
    {
        return make_vector(f(x), f(y), f(z));
    }
}

#endif //SPACE_GAME_PHYSICS_VEC_HPP
