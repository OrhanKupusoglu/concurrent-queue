#include <iostream>
#include <cmath>
#include <chrono>
#include <benchmark/benchmark.h>

#include "../../include/taxicab_number.h"

// passed as defines, for example:
// -DBMARK_POW_N=5 -DBMARK_POW_R=2 -DBMARK_SIZE_T=1
const uint32_t bmark_N = std::pow(10, BMARK_POW_N);
const uint32_t bmark_R = std::pow(10, BMARK_POW_R);
const uint32_t bmark_T = BMARK_SIZE_T;

std::chrono::milliseconds timeout{10};
std::chrono::milliseconds check{1};

template <class T>
class BenchmarkTaxiCab : public ::benchmark::Fixture {
    public:
        void SetUp(const ::benchmark::State& st) {
            taxicab.display_filename(true);
            taxicab.write_json(true);
        }

        void TearDown(const ::benchmark::State&) {
            taxicab.clear();
        }

        T taxicab{bmark_N, bmark_R, bmark_T, timeout, check};
};

using BenchmarkTaxiCabStr = BenchmarkTaxiCab<TaxiCabNumberStr>;
BENCHMARK_DEFINE_F(BenchmarkTaxiCabStr, inst)(benchmark::State& state) {
    while (state.KeepRunning()) {
        taxicab.run();
    }
}

BENCHMARK_REGISTER_F(BenchmarkTaxiCabStr, inst);

using BenchmarkTaxiCabInt = BenchmarkTaxiCab<TaxiCabNumberInt>;
BENCHMARK_DEFINE_F(BenchmarkTaxiCabInt, inst)(benchmark::State& state) {
    while (state.KeepRunning()) {
        taxicab.run();
    }
}

BENCHMARK_REGISTER_F(BenchmarkTaxiCabInt, inst);

// run the benchmark
BENCHMARK_MAIN();
