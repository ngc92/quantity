/*
 * This header defines the base class for a quantity and corresponding low level operations.
 */
#ifndef SPACEPHYS_DIMENSION_BASE_HPP
#define SPACEPHYS_DIMENSION_BASE_HPP

#include <ratio>

namespace quantity
{
    namespace dimensions
    {
        template<class T>
        struct DimBase { };

        template<class LengthDim, class TimeDim, class MassDim>
        struct Dimension_t : public DimBase<Dimension_t<LengthDim, TimeDim, MassDim>>
        {
            using length = LengthDim;
            using time   = TimeDim;
            using mass   = MassDim;
        };

        namespace ops
        {
            // calculation with dimensions
            template<class D1, class D2, template<class, class> class OP>
            struct OperationDim
            {
                using new_len  = OP<typename D1::length, typename D2::length>;
                using new_time = OP<typename D1::time,   typename D2::time>;
                using new_mass = OP<typename D1::mass,   typename D2::mass>;
                using type  = Dimension_t<new_len, new_time, new_mass>;
            };

            /// A dimension in which every component has value P/Q.
            template<std::intmax_t P, std::intmax_t Q>
            using fill_t = Dimension_t<std::ratio<P, Q>, std::ratio<P, Q>, std::ratio<P, Q>>;

            /// Product dimension of `A` and `B`.
            template<class A, class B>
            using mul_t = typename OperationDim<A, B, std::ratio_add>::type;

            /// Ratio dimension of `A` and `B`.
            template<class A, class B>
            using div_t = typename OperationDim<A, B, std::ratio_subtract>::type;

            /// power dimension `A**(P/Q)`.
            template<class A, std::intmax_t P, std::intmax_t Q>
            using pow_t = typename OperationDim<A, fill_t<P, Q>, std::ratio_multiply>::type;

            /// check whether two dimensions are equal.
            template<class T, class U>
            constexpr bool dimensions_equal()
            {
                return std::ratio_equal<typename T::length, typename U::length>::value &&
                       std::ratio_equal<typename T::time, typename U::time>::value &&
                       std::ratio_equal<typename T::mass, typename U::mass>::value;
            }
        }


        // pre defined useful dimensions
        namespace predefined
        {
            // useful constants as ratios
            using Zero = std::ratio<0, 1>;
            using One  = std::ratio<1, 1>;

            using dimless_t = Dimension_t<Zero, Zero, Zero>;
            using length_t  = Dimension_t<One, Zero, Zero>;
            using time_t    = Dimension_t<Zero, One, Zero>;
            using mass_t    = Dimension_t<Zero, Zero, One>;
        }

        using predefined::dimless_t;

        namespace ops
        {
            template<class T>
            using rate_dim_t = ops::div_t<T, predefined::time_t>;

            template<class T>
            using inverse_dim_t = ops::div_t<predefined::dimless_t, T>;
        }

        namespace predefined
        {
            using velocity_t     = ops::rate_dim_t<length_t>;
            using impulse_t      = ops::mul_t<velocity_t, mass_t>;
            using acceleration_t = ops::rate_dim_t<velocity_t>;
            using force_t        = ops::mul_t<acceleration_t, mass_t>;
            using area_t         = ops::mul_t<length_t, length_t>;
            using energy_t       = ops::mul_t<force_t, length_t>;
            using power_t        = ops::rate_dim_t<energy_t>;
        }
    }

}

#endif //SPACEPHYS_DIMENSION_BASE_HPP
