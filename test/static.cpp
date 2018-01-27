#ifndef SPACEPHYS_TEST_STATIC_HPP
#define SPACEPHYS_TEST_STATIC_HPP

#include "quantity/dimension.hpp"

using namespace quantity::dimensions;

template<std::intmax_t n, std::intmax_t d>
using r_ = std::ratio<n, d>;

static_assert(std::is_same<ops::fill_t<1, 1>, Dimension_t<predefined::One , predefined::One, predefined::One>>::value,
              "fill does not work");

using dim_a = Dimension_t<r_<2, 1>, r_<-1, 2>, r_<2, 3>>;
using dim_b = Dimension_t<r_<1, 1>, r_<2, 1>, r_<1, 2>>;

using dim_prod = Dimension_t<r_<3, 1>, r_<3, 2>, r_<7, 6>>;
using dim_div = Dimension_t<r_<1, 1>, r_<-5, 2>, r_<1, 6>>;
using dim_a_2_3 = Dimension_t<r_<4, 3>, r_<-1, 3>, r_<4, 9>>;
using dim_a_inv = Dimension_t<r_<-2, 1>, r_<1, 2>, r_<-2, 3>>;
using dim_a_rate = Dimension_t<r_<2, 1>, r_<-3, 2>, r_<2, 3>>;


static_assert(std::is_same<ops::mul_t<dim_a, dim_b>, dim_prod>::value, "error in dimension multiplication");
static_assert(std::is_same<ops::div_t<dim_a, dim_b>, dim_div>::value, "error in dimension division");
static_assert(std::is_same<ops::pow_t<dim_a, 2, 3>, dim_a_2_3>::value, "error in dimension power");
static_assert(std::is_same<ops::inverse_dim_t<dim_a>, dim_a_inv>::value, "error in dimension inverse");
static_assert(std::is_same<ops::rate_dim_t <dim_a>, dim_a_rate>::value, "error in dimension rate");
static_assert(ops::dimensions_equal<dim_a, dim_a>(), "error in dimension equal");
static_assert(!ops::dimensions_equal<dim_a, dim_b>(), "error in dimension equal");

static_assert(ops::dimensions_equal< predefined::power_t, Dimension_t<r_<2, 1>, r_<-3, 1>, r_<1, 1>> >(), "error in "
        "dimension equal");

#endif //SPACEPHYS_TEST_STATIC_HPP
