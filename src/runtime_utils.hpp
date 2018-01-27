//
// Created by erik on 7/31/17.
//

#ifndef SPACEPHYS_RUNTIME_RATIO_HPP
#define SPACEPHYS_RUNTIME_RATIO_HPP

#include "quantity/runtime.hpp"
#include <iosfwd>

namespace quantity
{
    namespace runtime
    {
        // helpers
        Dimension& operator+=(Dimension& a, const Dimension& b);
        Dimension& operator-=(Dimension& a, const Dimension& b);
        Dimension& operator*=(Dimension& a, const Ratio& b);

        bool contains(const Dimension& haystack, const Dimension& needle);

        // pre-defined runtime dimension
        extern Dimension NEWTON_DIM;
        extern Dimension JOULE_DIM;
        extern Dimension WATT_DIM;
    }
}
#endif //SPACEPHYS_RUNTIME_RATIO_HPP
