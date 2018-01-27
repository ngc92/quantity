//
// Created by erik on 1/26/18.
//

#include <boost/test/unit_test.hpp>

#include "quantity/io.hpp"
#include "runtime_utils.hpp"
#include "quantity/predefined.hpp"

BOOST_AUTO_TEST_SUITE(runtime_utils)
    using namespace quantity::runtime;
    BOOST_AUTO_TEST_CASE(predefined_units)
    {
        BOOST_CHECK(JOULE_DIM == to_dynamic(quantity::dimensions::predefined::energy_t{}));
        BOOST_CHECK(NEWTON_DIM == to_dynamic(quantity::dimensions::predefined::force_t{}));
        BOOST_CHECK(WATT_DIM == to_dynamic(quantity::dimensions::predefined::power_t{}));
    }

    BOOST_AUTO_TEST_CASE(contains_test)
    {
        BOOST_CHECK(contains(JOULE_DIM, JOULE_DIM));
        BOOST_CHECK(contains(NEWTON_DIM, NEWTON_DIM));
        BOOST_CHECK(contains(JOULE_DIM, NEWTON_DIM));
        BOOST_CHECK(!contains(NEWTON_DIM, JOULE_DIM));
    }
}