//
// Created by erik on 1/20/18.
//

#ifndef SPACEPHYS_RUNTIME_HPP
#define SPACEPHYS_RUNTIME_HPP

#include "dimension.hpp"

namespace quantity
{
    namespace runtime
    {
        /*!
         * \brief A ratio type that works at runtime.
         * \details This basically reimplements a subset of std::ratio
         *          that can now be used at runtime. This is needed for
         *          parsing units and quantities that are received
         *          as dynamic input e.g. from a file.
         *
         *          Since we only need it for a very specific purpose
         *          the interface is deliberately kept minimal.
         */
        struct Ratio
        {
            Ratio(std::intmax_t n = 0, std::intmax_t d = 1);
            std::intmax_t num = 0;
            std::intmax_t den = 1;
        };

        /// Turn a std::ratio into a dynamic ratio.
        template<std::intmax_t D, std::intmax_t N>
        constexpr Ratio to_dynamic(std::ratio<D, N>)
        {
            return Ratio{D, N};
        }

        /*!
         * \brief Specifies a dimension at runtime.
         * \details Some more documentation.
         */
        struct Dimension {
            Dimension() = default;
            Dimension(Ratio l, Ratio m, Ratio t, Ratio f = Ratio{0, 1});
            Ratio length;
            Ratio mass;
            Ratio time;

            Ratio factor;  // factor, as an exponent of 10.
        };

        template<class T>
        constexpr Dimension to_dynamic(dimensions::DimBase<T>) {
            return Dimension{to_dynamic(typename T::length{}),
                             to_dynamic(typename T::mass{}),
                             to_dynamic(typename T::time{}),
                             Ratio{0, 1}
            };
        }

        bool operator==(const Dimension& a, const Dimension& b);
    }
}

#endif //SPACEPHYS_RUNTIME_HPP
