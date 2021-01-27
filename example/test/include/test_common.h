#ifndef TEST_COMMON_H
#define TEST_COMMON_H

#include "gtest/gtest.h"
#include <cmath>
#include "../../include/taxicab_number.h"
#include "../../include/utility.h"
#include "test_util.h"

template<typename U>
class CommonTests {
    public:
        CommonTests(std::string name) : _name{name} {};
        CommonTests() = delete;

        // too small, no taxicab numbers
        void test_size_small(uint32_t n, uint32_t r, uint32_t t) {
            uint32_t N = std::pow(10, n);
            uint32_t R = std::pow(10, r);

            std::cout << "test " << _name << " with n = 10^" << n << ", r = 10^" << r << ", t = " << t << '\n';

            U taxicab{N, R, t, _timeout, _check};

            taxicab.run();
            std::this_thread::sleep_for(_delay);

            auto& found = taxicab.found();

            std::cout << "found - size = " << found.size() << '\n';
            ASSERT_EQ(found.size(), taxicab_count(N));
        }

        void test_size_big(uint32_t n, uint32_t r, uint32_t t) {
            uint32_t N = std::pow(10, n);
            uint32_t R = std::pow(10, r);

            std::cout << "test " << _name << " with n = 10^" << n << ", r = 10^" << r << ", t = " << t << '\n';

            U taxicab{N, R, t, _timeout, _check};

            taxicab.run();
            std::this_thread::sleep_for(_delay);

            auto& found = taxicab.found();

            std::cout << "found - size = " << found.size() << '\n';
            ASSERT_EQ(found.size(), taxicab_count(N));

            ASSERT_TRUE(check_taxicab(found));
        }

    private:
        std::string _name;
        std::chrono::seconds _delay{1};
        std::chrono::milliseconds _timeout{10};
        std::chrono::milliseconds _check{1};
};

#endif
