//
// Created by erik on 1/27/18.
//

#ifndef QUANTITY_RUNTIME_RATION_HPP
#define QUANTITY_RUNTIME_RATION_HPP

#include "quantity/runtime.hpp"
#include <iosfwd>

namespace quantity
{
    namespace runtime
    {
        // the operators
        Ratio operator-(const Ratio& a);

        Ratio& operator+=(Ratio& a, const Ratio& b);
        Ratio operator+(const Ratio& a, const Ratio& b);
        Ratio& operator-=(Ratio& a, const Ratio& b);
        Ratio operator-(const Ratio& a, const Ratio& b);
        Ratio operator*(const Ratio& a, const Ratio& b);
        Ratio& operator*=(Ratio& a, const Ratio& b);
        Ratio operator/(const Ratio& a, const Ratio& b);
        Ratio& operator/=(Ratio& a, const Ratio& b);


        bool operator==(const Ratio& a, const Ratio& b);
        bool operator!=(const Ratio& a, const Ratio& b);
        bool operator>=(const Ratio& a, const Ratio& b);

        Ratio abs(Ratio a);

        std::ostream& operator<<(std::ostream& s, const Ratio& r);
    }
}

#endif //QUANTITY_RUNTIME_RATION_HPP
