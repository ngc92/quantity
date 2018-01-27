#include "runtime_ratio.hpp"
#include <ostream>

namespace quantity {
    namespace runtime {
        Ratio::Ratio(std::intmax_t n, std::intmax_t d) : num(n), den(d)
        {
            if(d < 0) {
                den = -den;
                num = -num;
            }
        }

        Ratio operator+(const Ratio& a, const Ratio& b) {
            auto den = a.den * b.den;
            auto num = a.den * b.num + b.den * a.num;
            return Ratio{num, den};
        }

        Ratio& operator+=(Ratio& a, const Ratio& b) {
            return a = (a+b);
        }

        Ratio operator-(const Ratio& a, const Ratio& b) {
            return a + (-b);
        }

        Ratio& operator-=(Ratio& a, const Ratio& b) {
            return a = (a-b);
        }

        Ratio operator*(const Ratio& a, const Ratio& b) {
            return Ratio{a.num * b.num, a.den * b.den};
        }

        Ratio& operator*=(Ratio& a, const Ratio& b) {
            return a = (a*b);
        }

        Ratio operator/(const Ratio& a, const Ratio& b) {
            return Ratio{a.num * b.den, a.den * b.num};
        }

        Ratio& operator/=(Ratio& a, const Ratio& b) {
            return a = (a/b);
        }

        Ratio operator-(const Ratio& a) {
            return Ratio{-a.num, a.den};
        }

        bool operator==(const Ratio& a, const Ratio& b) {
            return a.num * b.den == b.num * a.den;
        }

        bool operator!=(const Ratio& a, const Ratio& b)
        {
            return !(a == b);
        }

        bool operator>=(const Ratio& a, const Ratio& b) {
            return a.num * b.den >= b.num * a.den;
        }

        Ratio abs(Ratio a) {
            return Ratio{std::abs(a.num), std::abs(a.den)};
        }

        std::ostream& operator<<(std::ostream& s, const Ratio& r) {
            s << r.num;
            if (r.den != 1)
                s << "/" << r.den;
            return s;
        }
    }
}