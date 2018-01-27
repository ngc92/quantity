//
// Created by erik on 1/20/18.
//

#ifndef SPACEPHYS_QUANTITY_HPP
#define SPACEPHYS_QUANTITY_HPP

#include "dimension.hpp"
#include <cmath>

namespace quantity
{
    /*!
     * \brief A small wrapper around a numerical type T that amends it with dimension information.
     * \tparam T Numerical type to be wrapped.
     * \tparam U Dimension of the resulting quantity.
     */
    template <class T, class U>
    class Quantity
    {
    public:
        /// The dimension that is used for this quantity.
        using dimension_t = U;

        constexpr Quantity() : value(T{}) { }
        explicit constexpr Quantity( const T& val ) : value(val) { };
        explicit constexpr operator T() const { return value; }
        T value;
    };

    /// A creation function for a quantity of given unit.
    template<class T, class U>
    constexpr auto quantity(T value, U unit) {
        return Quantity<T, U>( value );
    }

    /// special case dimensionless quantities.
    /// These implicitly convert from/to real numbers.
    template <class T>
    class Quantity<T, dimensions::dimless_t >
    {
    public:
        using dimension_t = dimensions::dimless_t;

        constexpr Quantity() : value(T{}) { }
        constexpr Quantity( const T& val ) : value(val) { };
        constexpr operator T() const { return value; }
        T value;
    };

    // Addition of same-type quantities
    template<class T, class U>
    constexpr Quantity<T, U>& operator+=(Quantity<T, U>& a, const Quantity<T, U>& b)
    {
        a.value += b.value;
        return a;
    }

    template<class T, class U>
    constexpr Quantity<T, U>& operator-=(Quantity<T, U>& a, const Quantity<T, U>& b)
    {
        a.value -= b.value;
        return a;
    }

    template<class T, class U>
    constexpr Quantity<T, U> operator+(const Quantity<T, U>& a, const Quantity<T, U>& b)
    {
        auto c = a;
        c += b;
        return c;
    }

    template<class T, class U>
    constexpr Quantity<T, U> operator-(const Quantity<T, U>& a, const Quantity<T, U>& b)
    {
        auto c = a;
        c -= b;
        return c;
    }

    template<class T, class U>
    constexpr Quantity<T, U> operator-(const Quantity<T, U>& a)
    {
        return Quantity<T, U>{-a.value};
    }

    // --------------------------------------------------------------------------------
    //   scalar multiplication
    template<class T, class U>
    constexpr Quantity<T, U>& operator*=(Quantity<T, U>& a, const T& factor)
    {
        a.value *= factor;
        return a;
    }

    template<class T, class U>
    constexpr Quantity<T, U> operator*(const Quantity<T, U>& a, const T& f)
    {
        auto c = a;
        c *= f;
        return c;
    }

    template<class T, class U>
    constexpr Quantity<T, U> operator/(const Quantity<T, U>& a, const T& f)
    {
        return a * (T(1)/f);
    }

    template<class T, class U>
    constexpr Quantity<T, U> operator*(const T& f, const Quantity<T, U>& a)
    {
        auto c = a;
        c *= f;
        return c;
    }


    // unit multiplication
    template<class T, class U, class V>
    Quantity<T, dimensions::ops::mul_t<U, V>>
    operator*( const Quantity<T, U>& a, const Quantity<T, V>& b)
    {
        return Quantity<T, dimensions::ops::mul_t<U, V>> (a.value * b.value);
    }

    template<class T, class U, class V>
    constexpr Quantity<T, U>& operator*=(Quantity<T, U>& a, const Quantity<T, V>& b)
    {
        a = a*b;
        return a;
    }

    template<class T, class U, class V>
    constexpr Quantity<T, dimensions::ops::div_t<U, V>>
    operator/( const Quantity<T, U>& a, const Quantity<T, V>& b)
    {
        return Quantity<T, dimensions::ops::div_t<U, V>>(a.value / b.value);
    }

    // comparisons
    template<class T, class U>
    constexpr bool operator<(const Quantity<T, U>& a, const Quantity<T, U>& b)
    {
        return a.value < b.value;
    }

    template<class T, class U>
    constexpr bool operator<=(const Quantity<T, U>& a, const Quantity<T, U>& b)
    {
        return a.value <= b.value;
    }

    template<class T, class U>
    constexpr bool operator>=(const Quantity<T, U>& a, const Quantity<T, U>& b)
    {
        return a.value >= b.value;
    }

    template<class T, class U>
    constexpr bool operator>(const Quantity<T, U>& a, const Quantity<T, U>& b)
    {
        return a.value > b.value;
    }

    template<class T, class U>
    constexpr bool operator==(const Quantity<T, U>& a, const Quantity<T, U>& b)
    {
        return a.value == b.value;
    }

    template<class T, class U>
    constexpr bool operator!=(const Quantity<T, U>& a, const Quantity<T, U>& b)
    {
        return !(a==b);
    }

    // some math functions
    template<class U, class V>
    auto sqrt( const Quantity<U, V>& s )
    {
        return Quantity<U, dimensions::ops::pow_t<V, 1, 2>>( std::sqrt(s.value) );
    }

    template<class U, class V>
    auto abs( const Quantity<U, V>& s )
    {
        return Quantity<U, V>( std::abs(s.value) );
    }
}

#endif //SPACEPHYS_QUANTITY_HPP
