#define BOOST_TEST_MODULE quantity
#include <boost/test/unit_test.hpp>

#include "quantity/io.hpp"
#include "runtime_utils.hpp"
#include "quantity/predefined.hpp"
#include "runtime_ratio.hpp"

using namespace quantity;
using runtime::Ratio;

template<class T>
void check_stream_out(T&& object, const std::string& desired)
{
    std::stringstream target;
    target << object;
    BOOST_CHECK_EQUAL(target.str(), desired);
}

template<class T>
void check_stream_in(const std::string& input, T&& desired)
{
    std::decay_t<T> object;
    std::stringstream target;
    target << input;
    target >> object;
    BOOST_CHECK_EQUAL(object, desired);
}

void check_round_trip(runtime::Dimension dim) {
    std::stringstream target;
    try {
        target << dim;
        try {
            runtime::Dimension loaded = runtime::parse_dim(target.str());
            // dim and loaded might differ in their prefactors.
            BOOST_CHECK_EQUAL(dim.length, loaded.length);
            BOOST_CHECK_EQUAL(dim.mass, loaded.mass);
            BOOST_CHECK_EQUAL(dim.time, loaded.time);
        } catch(std::out_of_range& oor) {
            std::string error_message = "out_of_range exception in parse_dim when processing " + target.str();
            std::cout << dim.length << " " << dim.mass << " " << dim.time << " " << dim.factor << "\n";
            BOOST_ERROR(error_message + "\n" + oor.what());
        }
    } catch(std::out_of_range& oor) {
        std::string error_message = "out_of_range exception in <<. Partially processed " + target.str();
        std::cout << dim.length << " " << dim.mass << " " << dim.time << " " << dim.factor << "\n";
        BOOST_ERROR(error_message + "\n" + oor.what());
    }
}

BOOST_AUTO_TEST_SUITE(io_tests)

    using namespace quantity::predefined;

    // some dimensions to use in the following tests
    auto r0 = Ratio(0);
    auto r1 = Ratio(1);
    auto G = runtime::Dimension{r0, r1, r0, Ratio(-3)};
    auto M = runtime::Dimension{r1, r0, r0, r0};
    auto S = runtime::Dimension{r0, r0, r1, r0};

    auto KG  = runtime::Dimension{r0, r1, r0, r0};
    auto KM  = runtime::Dimension{r1, r0, r0, Ratio(3)};
    auto MPS = runtime::Dimension{r1, r0, Ratio(-1), r0};

    auto M2 = runtime::Dimension{Ratio(2), r0, r0, r0};
    auto KM2 = runtime::Dimension{Ratio(2), r0, r0, Ratio(6)};
    auto TON = runtime::Dimension{r0, r1, r0, Ratio(3)};
    auto KT = runtime::Dimension{r0, r1, r0, Ratio(6)};
    auto KT2 = runtime::Dimension{r0, Ratio(2), r0, Ratio(12)};
    auto KGMS = runtime::Dimension{r1, r1, Ratio(-1), r0};
    auto TMS = runtime::Dimension{r1, r1, Ratio(-1), Ratio(3)};

    BOOST_AUTO_TEST_CASE(runtime_dim_out)
    {
        check_stream_out(G, "g");
        check_stream_out(M, "m");
        check_stream_out(S, "s");
        check_stream_out(KG, "kg");
        check_stream_out(KM, "km");
        check_stream_out(MPS, "m/s");

        check_stream_out(M2, "m^2");
        check_stream_out(KM2, "km^2");
        check_stream_out(TON, "t");
        check_stream_out(KT, "kt");
        check_stream_out(KT2, "kt^2");
        check_stream_out(KGMS, "kgm/s");
        check_stream_out(TMS, "tm/s");  // this one is weird
    }

    BOOST_AUTO_TEST_CASE(dim_roundtrip)
    {
        // try all dimensions systematically.
        for(int mexp = -4; mexp <= 4; ++mexp)
        {
            for(int kexp = -4; kexp <= 4; ++kexp)
            {
                for(int sexp = -4; sexp <= 4; ++sexp)
                {
                    // for (0,0,0) we get no unit, which would be the empty string, that
                    // does not round trip.
                    if(mexp != 0 || kexp != 0 || sexp != 0) {
                        check_round_trip(runtime::Dimension{{mexp, 1},
                                                            {kexp, 1},
                                                            {sexp, 1}});
                    }
                }
            }
        }
    }

    BOOST_AUTO_TEST_CASE(quantity_input)
    {
        check_stream_in("5m", 5.0_m);
        check_stream_in("5kg", 5.0_kg);
        check_stream_in("12.4s", 12.4_s);
        check_stream_in("0.5t", 0.5_t);
        check_stream_in("7.5 km", 7.5_km);
        check_stream_in("12.0N", 12.0_N);
        check_stream_in("12.0 N", 12.0_N);
        check_stream_in("8.0 J", 8.0_J);
        check_stream_in("1.2 kW", 1.2_kW);
        check_stream_in("1.2 kt", 1200.0_t);
        check_stream_in("100 km^2", 10.0_km * 10.0_km);
        check_stream_in("1000 mg", 0.001_kg);

        // compound
        check_stream_in("12.0 kgm/s^2", 12.0_N);
        check_stream_in("8.0 kgm^2/s^2", 8.0_J);
        check_stream_in("-4900 kgm^2/s^3", -4.9_kW);
    }

    BOOST_AUTO_TEST_CASE(quantity_output_simple)
    {
        // simple outputs. No rescaling of the value required.
        check_stream_out(5.0_m, "5 m");
        check_stream_out(12.4_s, "12.4 s");
        check_stream_out(-9.2_kg, "-9.2 kg");
        check_stream_out(0.0_km, "0 m");
    }

    BOOST_AUTO_TEST_CASE(quantity_output_with_prefix)
    {
        // here we need to figure out the prefactor
        check_stream_out(7.5_km, "7.5 km");
        check_stream_out(12.6_g, "12.6 g");
        check_stream_out(12.6_kg, "12.6 kg");

        // and handle quadratic stuff correctly.
        check_stream_out(2.0_km * 2.0_km, "4 km^2");
        check_stream_out(2.0_km * 2.0_m, "4000 m^2");
    }

    BOOST_AUTO_TEST_CASE(quantity_output_compound)
    {
        // recognize that a certain combination of dimensions can be written in a shorter combination.
        check_stream_out(-12.2_N, "-12.2 N");
        check_stream_out(8.0_J, "8 J");
        check_stream_out(1.2_kW, "1.2 kW");
    }

    BOOST_AUTO_TEST_CASE(quantity_output_time)
    {
        // don't do kiloseconds
        check_stream_out(52.0_s, "52 s");
        check_stream_out(1025.0_s, "1025 s");
        check_stream_out(0.1_s, "100 ms");
    }

    BOOST_AUTO_TEST_CASE(quantity_input_checking)
    {
        BOOST_CHECK_THROW(check_stream_in("5kg", 5.0_m), std::runtime_error);
    }

    BOOST_AUTO_TEST_CASE(vector_output)
    {
        check_stream_out(quantity::Vec3<int>{1, 2, 3}, "(1, 2, 3)");
        check_stream_out(quantity::make_vector(2.1_m, -2.5_m, 1.8_m), "(2.1 m, -2.5 m, 1.8 m)");
    }

    BOOST_AUTO_TEST_CASE(vector_input)
    {
        check_stream_in("1 2 3", quantity::Vec3<int>{1, 2, 3});
        check_stream_in("2.1 m -2.5 m 1.8 m", quantity::make_vector(2.1_m, -2.5_m, 1.8_m));

        //! \todo allow comma separated values; allow parenthesis
    }

BOOST_AUTO_TEST_SUITE_END()