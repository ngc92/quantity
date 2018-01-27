//
// Created by erik on 7/31/17.
//

#include "runtime_utils.hpp"
#include <ostream>
#include "runtime_ratio.hpp"

namespace quantity
{
    namespace runtime
    {
        Dimension::Dimension(Ratio l, Ratio m, Ratio t, Ratio f) :
                length(l),
                mass(m),
                time(t),
                factor(f)
        {
        }

        Dimension& operator+=(Dimension& a, const Dimension& b)
        {
            a.length = a.length + b.length;
            a.mass = a.mass + b.mass;
            a.time = a.time + b.time;
            a.factor = a.factor + b.factor;
            return a;
        }

        Dimension& operator-=(Dimension& a, const Dimension& b)
        {
            a.length = a.length - b.length;
            a.mass = a.mass - b.mass;
            a.time = a.time - b.time;
            a.factor = a.factor - b.factor;
            return a;
        }

        Dimension& operator*=(Dimension& a, const Ratio& b)
        {
            a.length = a.length * b;
            a.mass = a.mass * b;
            a.time = a.time * b;
            a.factor = a.factor * b;
            return a;
        }

        bool operator==(const Dimension& a, const Dimension& b)
        {
            return  a.length == b.length &&
                    a.mass   == b.mass &&
                    a.time   == b.time &&
                    a.factor == b.factor;
        }

        bool contains(const Dimension& haystack, const Dimension& needle)
        {
            return abs(haystack.length) >= abs(needle.length) &&
                    abs(haystack.mass) >= abs(needle.mass) &&
                    abs(haystack.time) >= abs(needle.time) &&
                    haystack.length * needle.length >= Ratio{0, 1} &&
                    haystack.mass * needle.mass >= Ratio{0, 1} &&
                    haystack.time * needle.time >= Ratio{0, 1};
        }

        Dimension NEWTON_DIM = {{1, 1}, {1, 1}, {-2, 1}};
        Dimension JOULE_DIM = {{2, 1}, {1, 1}, {-2, 1}};
        Dimension WATT_DIM = {{2, 1}, {1, 1}, {-3, 1}};
    }
}
