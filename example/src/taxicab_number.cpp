#include <utility>
#include <tuple>
#include <algorithm>
#include <thread>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <bitset>

#include "../include/taxicab_number.h"

void TaxiCabNumberStr::save_taxicab_number(bool& loop) {
    std::tuple<uint32_t, uint32_t, uint32_t> ta{};

    while (loop) {
        bool go = _queue.wait_and_pop_while(ta, _timeout, _check);

        if (!loop) {
            break;
        }

        if (go) {
            uint32_t i = std::get<0>(ta);
            uint32_t j = std::get<1>(ta);
            uint32_t k = std::get<2>(ta);

            auto ab = std::minmax(j, k);
            uint64_t a = ab.first;
            uint64_t b = ab.second;

            auto ta = std::to_string(a) + '.' + std::to_string(b);

            auto it = _cube.find(i);
            if (it == _cube.end()) { // first time
                _cube.insert({i, ta});
            } else {
                if (it->second.find(ta) == std::string::npos) {  // not found before
                    it->second += '.' + ta;
                }
            }
        }
    }
}

// by Jonathan Boccara - Fluent C++
// https://www.fluentcpp.com/2017/04/21/how-to-split-a-string-in-c/
std::vector<std::string> TaxiCabNumberStr::split(const std::string& s, const char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);

    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

void TaxiCabNumberStr::report_taxicab_number(const int rank) {
    _taxicab.clear();
    for (auto it = _cube.cbegin(); it != _cube.cend(); ++it) {
        std::pair<uint32_t, std::string> ra = *it;
        auto data = split(ra.second, '.');

        if (data.size() >= (2 * rank)) {
            TaxiCab found(ra.first);

            for (int j = 0; j < data.size(); j += 2) {
                uint32_t a = std::stoul(data[j]);
                uint32_t b = std::stoul( data[j + 1]);
                found.cube.push_back({a, b});
            }

            _taxicab.push_back(found);
        }
    }

    dump_taxicab_number(rank, _cube.size(), _taxicab.size());
}

void TaxiCabNumberStr::clear() {
    Base::clear();
    _cube.clear();
}

// -----------------------------------------------------------------------------

void TaxiCabNumberInt::save_taxicab_number(bool& loop) {
     std::tuple<uint32_t, uint32_t, uint32_t> ta{};

    while (loop) {
        bool go = _queue.wait_and_pop_while(ta, _timeout, _check);

        if (!loop) {
            break;
        }

        if (go) {
            uint32_t i = std::get<0>(ta);
            uint32_t j = std::get<1>(ta);
            uint32_t k = std::get<2>(ta);

            auto ab = std::minmax(j, k);
            uint64_t a = ab.first;
            uint64_t b = ab.second;

            auto it = _cube.find(i);
            if (it == _cube.end()) { // first time
                uint64_t xa = a;
                uint64_t xb = b << 10;
                uint64_t xc = xa | xb;

                _cube.insert({i, xc});
            } else {
#if __cplusplus > 201703L  // C++20
                int pos = 64 - std::countl_zero(it->second);
#else
                int pos = 64 - __builtin_clzll(it->second);
#endif
                if (pos >= 0 && pos < 20) {
                    uint64_t xnum1 = it->second & 1048575;              // ignore other entries: 0000000000000000000000000000000000000000000011111111111111111111
                    uint64_t xa1 = a;
                    uint64_t xb1 = b << 10;
                    uint64_t xc1 = xa1 | xb1;

                    if (xc1 != xnum1) {
                        uint64_t xa = a << 20;
                        uint64_t xb = b << 30;
                        it->second |= xa | xb;
                    }
                } else if (pos >= 20 && pos < 40) {
                    uint64_t xnum1 = it->second & 1048575;              // ignore other entries: 0000000000000000000000000000000000000000000011111111111111111111
                    uint64_t xa1 = a;
                    uint64_t xb1 = b << 10;
                    uint64_t xc1 = xa1 | xb1;

                    uint64_t xnum2 = it->second & 1099510579200;        // ignore other entries: 0000000000000000000000001111111111111111111100000000000000000000
                    uint64_t xa2 = a << 20;
                    uint64_t xb2 = b << 30;
                    uint64_t xc2 = xa2 | xb2;

                    if ((xc1 != xnum1) && (xc2 != xnum2)) {
                        uint64_t xa = a << 40;
                        uint64_t xb = b << 50;
                        it->second |= xa | xb;
                    }
                }
            }
        }
    }
}

std::vector<uint16_t> TaxiCabNumberInt::split(const uint64_t x) {
    int size = 0;
#if __cplusplus > 201703L  // C++20
    int pos = 64 - std::countl_zero(x);
#else
    int pos = 64 - __builtin_clzll(x);
#endif

    if (pos >= 0 && pos < 20) {
        size = 2;
    } else if (pos >= 21 && pos < 40) {
        size = 4;
    } else {
        size = 6;
    }

    std::vector<uint16_t> nums{};
    nums.reserve(size);
    uint64_t mask = 1023;   // 2^10 - 1;

    for (int i = 0; i < size; ++i) {
        uint64_t num = (x & (mask << (i * 10))) >> (i * 10);
        nums.push_back(static_cast<uint16_t>(num));
    }

    return nums;
}

void TaxiCabNumberInt::report_taxicab_number(const int rank) {
    _taxicab.clear();
    for (auto it = _cube.cbegin(); it != _cube.cend(); ++it) {
        std::pair<uint32_t, uint64_t> ra = *it;
        auto data = split(ra.second);

        if (data.size() >= (2 * rank)) {
            TaxiCab found(ra.first);

            for (int j = 0; j < data.size(); j += 2) {
                uint16_t a = data[j];
                uint16_t b = data[j + 1];
                found.cube.push_back({a, b});
            }

            _taxicab.push_back(found);
        }
    }

    dump_taxicab_number(rank, _cube.size(), _taxicab.size());
}

void TaxiCabNumberInt::clear() {
    Base::clear();
    _cube.clear();
}
