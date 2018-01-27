#include <regex>
#include <iostream>

#include <boost/throw_exception.hpp>
#include "runtime_utils.hpp"
#include "runtime_ratio.hpp"
#include "quantity/io.hpp"
#include "quantity/predefined.hpp"

namespace {
    using namespace quantity;

    // SI prefixes
    std::map<char, runtime::Ratio> SIPrefixes_helper()
    {
        using runtime::Ratio;
        std::map<char, Ratio> prefixes;
        prefixes['p'] = Ratio{-12, 1};
        prefixes['n'] = Ratio{ -9, 1};
        prefixes['u'] = Ratio{ -6, 1};
        prefixes['m'] = Ratio{ -3, 1};
        prefixes['c'] = Ratio{ -2, 1};
        prefixes['d'] = Ratio{  1, 1};
        prefixes['h'] = Ratio{  2, 1};
        prefixes['k'] = Ratio{  3, 1};
        prefixes['M'] = Ratio{  6, 1};
        prefixes['G'] = Ratio{  9, 1};
        prefixes['T'] = Ratio{ 12, 1};
        return prefixes;
    }

    auto& SI_prefixes()
    {
        static auto mp = SIPrefixes_helper();
        return mp;
    }

    runtime::Dimension parse_single_factor(std::string& unit)
    {
        std::smatch m;
        std::regex e(R"(([pnumcdhkMGT]?)([gmstJWN])(\^(-?[[:d:]]+)(\/([[:d:]]+))?)?)");

        if (std::regex_search(unit, m, e, std::regex_constants::match_continuous)) {
            runtime::Dimension dims{};
            runtime::Ratio exponent{1, 1};

            enum State
            {
                BEGIN,
                EXP,
                DEN
            } state = BEGIN;

            int match_counter = 0;
            for (const auto& x: m) {
                // ignore first match, this is the whole sequence. IF that is just one character,
                // we could misinterpret it.
                match_counter += 1;
                if (match_counter == 1) {
                    continue;
                }
                if(x.length() == 0) {
                    continue;
                }

                if(x.length() == 1) {

                    char first_char = x.str()[0];

                    // check for SI prefix
                    if(match_counter == 2) {
                        try {
                            dims.factor = dims.factor + SI_prefixes().at(first_char);
                        } catch (std::out_of_range& oor)
                        {
                            BOOST_THROW_EXCEPTION(std::out_of_range(
                                    std::string("Could not find SI prefix for '") + first_char + "'"));
                        }
                        continue;
                    }

                    if (x == "g") {
                        dims.mass.num = 1;
                        dims.factor = dims.factor + runtime::Ratio{-3, 1};
                    }
                    if (x == "m") { dims.length.num = 1; }
                    if (x == "s") { dims.time.num = 1; }
                    if (x == "t") {
                        dims.mass.num = 1;
                        dims.factor = dims.factor + runtime::Ratio{3, 1};
                    }
                    if (x == "J") {
                        dims += runtime::to_dynamic(dimensions::predefined::energy_t{});
                    }
                    if (x == "W") {
                        dims += runtime::to_dynamic(dimensions::predefined::power_t{});
                    }
                    if (x == "N") {
                        dims += runtime::to_dynamic(dimensions::predefined::force_t{});
                    }
                }


                if (state == EXP) { exponent.num = std::stoi(x); }
                if (state == DEN) { exponent.den = std::stoi(x); }

                if (x.str()[0] == '^') { state = EXP; }
                if (x.str()[0] == '/') { state = DEN; }
            }

            dims *= exponent;
            unit = m.suffix();
            return dims;
        }

        BOOST_THROW_EXCEPTION(std::runtime_error("Invalid unit dimension: " + unit));
    }
}

namespace quantity
{
    namespace runtime
    {
        Dimension parse_dim(std::string unit_s)
        {
            Dimension u{};
            int mode = +1;
            do {
                Dimension parsed = parse_single_factor(unit_s);
                if (mode == -1) {
                    parsed *= runtime::Ratio{-1, 1};
                }
                u += parsed;
                if (unit_s[0] == '/') {
                    mode = -1;
                    unit_s = unit_s.substr(1);
                }
            } while (!unit_s.empty());

            return u;
        }

        namespace
        {
            const char* si_prefix(std::intmax_t exp) {
                static std::map<std::intmax_t, const char*> m = {
                        {12, "T"},
                        {9, "G"},
                        {6, "M"},
                        {3, "k"},
                        {2, "h"},
                        {0, ""},
                        {-1, "d"},
                        {-2, "c"},
                        {-3, "m"},
                        {-6, "u"},
                        {-9, "n"},
                        {-12, "p"}
                };
                try {
                    return m.at(exp);
                } catch (std::out_of_range& oor)
                {
                    BOOST_THROW_EXCEPTION(std::out_of_range(
                                                  "Could not find SI prefix for exponent " + std::to_string(exp)));
                }
            }

            void print_single_dimension(std::ostream& target, char unit, const Ratio& exponent, Ratio& factor)
            {
                if (exponent.num != 0) {
                    Ratio prefix = factor / exponent;
                    //std::cout << factor << " " << exponent << " " << prefix << "\n";
                    if(unit == 'g' && prefix >= Ratio(6)) {
                        unit = 't';
                        prefix = prefix - Ratio(6);
                    }
                    //std::cout << "updated " << prefix << "\n";

                    if(prefix.num % prefix.den == 0 && (prefix.num / prefix.den) % 3 == 0)
                    {
                        target << si_prefix(prefix.num / prefix.den);
                        //std::cout << "prefix " << factor << " " << prefix << " " << exponent << "\n";
                        factor = factor - prefix * exponent;
                        //std::cout << "updated " << factor << "\n";
                        if(unit == 't')
                        {
                            factor = factor - Ratio(6);
                        }
                    }

                    target << unit;

                    if (!(exponent == Ratio(1))) {
                        target << "^" << exponent;
                    }
                }
            }

            void print(std::ostream& target, Dimension dim, Ratio factor)
            {
                print_single_dimension(target, 'g', dim.mass, factor);
                print_single_dimension(target, 's', dim.time, factor);  // seconds before meters to circumvent milli seconds vs meter seconds
                print_single_dimension(target, 'm', dim.length, factor);
            }

            void split_ratio(const Ratio& r, Ratio& num, Ratio& den)
            {
                if (r.num >= 0 || r.den != 1) {
                    num = r;
                } else {
                    den = -r;
                }
            }

            void split_dim(const Dimension& dim, Dimension& num, Dimension& den) {
                split_ratio(dim.time, num.time, den.time);
                split_ratio(dim.length, num.length, den.length);
                split_ratio(dim.mass, num.mass, den.mass);
            }
        }

        std::ostream& operator<<(std::ostream& stream, Dimension dim)
        {
            // OK, first figure out the total prefix factor
            auto factor = dim.factor + Ratio(3)*dim.mass;

            if(contains(dim, WATT_DIM))
            {
                factor = factor - Ratio(3);
                print_single_dimension(stream, 'W', {1, 1}, factor);
                dim -= WATT_DIM;
            }
            else if(contains(dim, JOULE_DIM))
            {
                factor = factor - Ratio(3);
                print_single_dimension(stream, 'J', {1, 1}, factor);
                dim -= JOULE_DIM;
            }
            else if(contains(dim, NEWTON_DIM))
            {
                factor = factor - Ratio(3);
                print_single_dimension(stream, 'N', {1, 1}, factor);
                dim -= NEWTON_DIM;
            }

            Dimension num_d;
            Dimension den_d;
            split_dim(dim, num_d, den_d);
            if(num_d == Dimension{{0,1}, {0, 1}, {0,1}}) {
                num_d = dim;
                den_d = Dimension{{0,1}, {0, 1}, {0,1}};
            }

            print(stream, num_d, factor);
            if(den_d.mass.num != 0 || den_d.time.num != 0 || den_d.length.num != 0) {
                stream << "/";
                print(stream, den_d, Ratio(0));
            }
            return stream;
        }

        Dimension dynamic_rescale(long double value, Dimension dimension)
        {
            value = std::abs(value);

            // no prefixes for 0, this would cause an endless loop below
            if(value == 0) return dimension;

            while(value < 1.0) {
                value *= 1e+3;
                dimension.factor = dimension.factor - Ratio{3, 1};
            }

            // no inceasing prefix for time-only dimension.
            if(dimension.mass.num == 0 && dimension.length.num == 0) return dimension;

            while(value > 1e3) {
                value *= 1e-3;
                dimension.factor = dimension.factor + Ratio{3, 1};
            }

            return dimension;
        }
    }
}



