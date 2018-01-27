//
// Created by erik on 1/27/18.
//

//
// Created by erik on 1/26/18.
//

#include <boost/test/unit_test.hpp>

#include "quantity/io.hpp"
#include "runtime_ratio.hpp"
#include "quantity/predefined.hpp"

BOOST_AUTO_TEST_SUITE(runtime_ratio)
    using namespace quantity::runtime;

    BOOST_AUTO_TEST_CASE(ratio_uminus)
    {
        BOOST_CHECK_EQUAL((-Ratio{7, 8}), (Ratio{-7, 8}));
        BOOST_CHECK_EQUAL((-Ratio{7, -5}), (Ratio{-7, -5}));
    }

    BOOST_AUTO_TEST_CASE(ratio_minus)
    {
        BOOST_CHECK_EQUAL((Ratio(12) - Ratio{12, 4}), Ratio(9));
        BOOST_CHECK_EQUAL((Ratio(-12) - Ratio{-12, -4}), Ratio(-15));
        BOOST_CHECK_EQUAL((Ratio{4, 3} - Ratio{3, 4}), (Ratio{7, 12}));

        Ratio a{12};
        a -= Ratio{12, 4};
        BOOST_CHECK_EQUAL(a, Ratio(9));

        Ratio b{4, 3};
        b -= Ratio{3, 4};
        BOOST_CHECK_EQUAL(b, (Ratio{7, 12}));
    }

    BOOST_AUTO_TEST_CASE(ratio_plus)
    {
        BOOST_CHECK_EQUAL(Ratio(7) + Ratio(-5), Ratio(2));
        BOOST_CHECK_EQUAL((Ratio{4, 3} + Ratio{3, 4}), (Ratio{25, 12}));

        Ratio a{12};
        a += Ratio{12, 4};
        BOOST_CHECK_EQUAL(a, Ratio(15));

        Ratio b{4, 3};
        b += Ratio{3, 2};
        BOOST_CHECK_EQUAL(b, (Ratio{17, 6}));
    }

    BOOST_AUTO_TEST_CASE(ratio_times)
    {
        BOOST_CHECK_EQUAL(Ratio{3} * Ratio{-2}, Ratio{-6});
        BOOST_CHECK_EQUAL((Ratio{4, 3} * Ratio{3, 4}), Ratio{1});

        Ratio a{6};
        a *= Ratio{5, 4};
        BOOST_CHECK_EQUAL(a, (Ratio{15, 2}));

        Ratio b{4, 3};
        b *= Ratio{3, 2};
        BOOST_CHECK_EQUAL(b, (Ratio{12, 6}));
    }

    BOOST_AUTO_TEST_CASE(ratio_divide)
    {
        BOOST_CHECK_EQUAL((Ratio{-12, 1} / Ratio{-4, 1}), (Ratio{-12, -4}));
        Ratio b{4, 3};
        b /= Ratio{3, 2};
        BOOST_CHECK_EQUAL(b, (Ratio{8, 9}));
    }

    BOOST_AUTO_TEST_CASE(ratio_equals)
    {
        BOOST_CHECK((Ratio{5*3, 2*3} == Ratio{5, 2}));
        BOOST_CHECK((Ratio{5*3, 2*3} != Ratio{6, 2}));
    }

    BOOST_AUTO_TEST_CASE(ratio_gt)
    {
        BOOST_CHECK(!(Ratio{-12, -4} >= Ratio(6)));
    }
}