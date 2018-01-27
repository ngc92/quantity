//
// Created by erik on 1/20/18.
//

#ifndef SPACEPHYS_IO_HPP
#define SPACEPHYS_IO_HPP

#include <iostream>
#include <cmath>
#include <boost/throw_exception.hpp>
#include "quantity.hpp"
#include "runtime.hpp"
#include "vec.hpp"

namespace quantity
{
    namespace runtime
    {
        std::ostream& operator<<(std::ostream& stream, Dimension dim);
        Dimension parse_dim(std::string unit_s);
        Dimension dynamic_rescale(long double value, Dimension dimension);
    }

    template<class B, class T>
    std::ostream& operator<<(std::ostream& stream, Quantity<B, T> value)
    {
        auto dyn_dim = runtime::dynamic_rescale(value.value, runtime::to_dynamic(T{}));
        return stream << value.value / std::pow(10.0, dyn_dim.factor.num) << " " << dyn_dim;
    }


    template<class B, class T>
    std::istream& operator>>(std::istream& stream, Quantity<B, T>& value)
    {
        std::string unit_str;
        stream >> value.value;
        stream >> unit_str;
        auto parsed_unit = runtime::parse_dim(unit_str);
        value.value *= std::pow(B(10), B(parsed_unit.factor.num) / parsed_unit.factor.den);
        parsed_unit.factor = runtime::Ratio{0, 1};
        if(!(parsed_unit == runtime::to_dynamic(T{})))
        {
            /// \todo add more information to that error!
            BOOST_THROW_EXCEPTION(std::runtime_error("Unit mismatch"));
        }
        return stream;
    }

    template<class T>
    std::ostream& operator<<(std::ostream& os, const Vec3<T>& vec)
    {
        return os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    }

    template<class T>
    std::istream& operator>>(std::istream& is, Vec3<T>& vec)
    {
        return is >> vec.x >> vec.y >> vec.z;
    }
}

#endif //SPACEPHYS_IO_HPP
