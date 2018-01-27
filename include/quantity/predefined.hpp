#ifndef SPACEPHYS_PREDEFINED_HPP
#define SPACEPHYS_PREDEFINED_HPP

#include "quantity.hpp"
#include "vec.hpp"

namespace quantity
{
    namespace predefined
    {
        namespace pd_ = dimensions::predefined;

        using base_t  = double;

        template<class D>
        using qty_ = Quantity<base_t, D>;

        using scalar_t  = qty_<pd_::dimless_t>;
        using length_t  = qty_<pd_::length_t>;
        using mass_t    = qty_<pd_::mass_t>;
        using time_t    = qty_<pd_::time_t>;
        using speed_t   = qty_<pd_::velocity_t>;
        using impulse_t = qty_<pd_::impulse_t>;
        using accel_t   = qty_<pd_::acceleration_t>;
        using force_t   = qty_<pd_::force_t>;
        using area_t    = qty_<pd_::area_t>;
        using energy_t  = qty_<pd_::energy_t>;
        using power_t   = qty_<pd_::power_t>;

        template<class T>
        using rate_t    = qty_<dimensions::ops::rate_dim_t<typename T::dimension_t>>;

        template<class T>
        using inverse_t = qty_<dimensions::ops::inverse_dim_t<typename T::dimension_t>>;

        using length_vec   = Vec3<length_t>;
        using velocity_vec = Vec3<speed_t>;
        using accel_vec    = Vec3<accel_t>;
        using force_vec    = Vec3<force_t>;
        using impulse_vec  = Vec3<impulse_t>;

        constexpr base_t KILO = 1'000;
        constexpr base_t MEGA = 1'000'000;

        // creation functions
        inline constexpr length_t meters(base_t v) { return length_t(v); }
        inline constexpr auto kilometers(base_t v) { return meters(KILO * v); }

        constexpr length_vec meters(Vec3<base_t> v) { return length_vec(v); }
        constexpr auto kilometers(Vec3<base_t> v) { return meters(KILO*v); }
        constexpr auto meters(base_t x, base_t y, base_t z) { return meters(make_vector(x, y, z)); }
        constexpr auto kilometers(base_t x, base_t y, base_t z) { return kilometers(make_vector(x, y, z)); }

        constexpr mass_t grams(base_t v) { return mass_t(v / 1000.0); }
        constexpr mass_t kilogram(base_t v) { return mass_t(v); }
        constexpr auto tonnes(base_t v) { return kilogram(v * KILO); }

        constexpr force_t newtons(base_t v) { return force_t(v); }
        constexpr auto kilonewtons(base_t v) { return newtons(v * KILO); }

        constexpr energy_t joules(base_t v) { return energy_t(v); }
        constexpr auto kilojoules(base_t v) { return joules(v * KILO); }
        constexpr auto megajoules(base_t v) { return joules(v * MEGA); }

        constexpr power_t watts(base_t v) { return power_t(v); }
        constexpr auto kilowatts(base_t v) { return watts(v * KILO); }
        constexpr auto megawatts(base_t v) { return watts(v * MEGA); }

        inline constexpr length_t operator ""_m(long double f) { return meters(f); }
        inline constexpr length_t operator ""_km(long double f) { return kilometers(f); }

        inline constexpr mass_t operator ""_g(long double f) { return grams(f); }
        inline constexpr mass_t operator ""_kg(long double f) { return kilogram(f); }
        inline constexpr mass_t operator ""_t(long double f) { return tonnes(f); }

        inline constexpr force_t operator ""_N(long double f) { return newtons(f); }
        inline constexpr force_t operator ""_kN(long double f) { return kilonewtons(f); }

        inline constexpr energy_t operator ""_J(long double f) { return joules(f); }
        inline constexpr energy_t operator ""_kJ(long double f) { return kilojoules(f); }
        inline constexpr energy_t operator ""_MJ(long double f) { return megajoules(f); }

        inline constexpr power_t operator ""_W(long double f) { return watts(f); }
        inline constexpr power_t operator ""_kW(long double f) { return kilowatts(f); }
        inline constexpr power_t operator ""_MW(long double f) { return megawatts(f); }

        inline constexpr time_t operator ""_s(long double f) { return time_t(f); }
        inline constexpr speed_t operator ""_kps(long double f) { return kilometers(f) / 1.0_s; }
    }
}

#endif //SPACEPHYS_PREDEFINED_HPP
