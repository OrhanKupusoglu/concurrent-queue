#ifndef TAXICAB_NUMBER_H
#define TAXICAB_NUMBER_H

#include <cstdint>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <utility>

#include "../../include/concurrent_queue.h"
#include "utility.h"

/**
 * Taxicab number
 * see:
 *      https://en.wikipedia.org/wiki/Taxicab_number
 *
 * GNU
 * 6.59 Other Built-in Functions Provided by GCC
 * see:
 *      https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html
 */

struct TaxiCab {
    TaxiCab(uint32_t tc) {
        taxicab_no = tc;
    }
    uint32_t taxicab_no;
    std::vector<std::pair<uint16_t, uint16_t>> cube;
};

// the interface
class Base {
    public:
        Base(uint32_t N,
             uint32_t R,
             uint32_t T,
             std::chrono::milliseconds& timeout,
             std::chrono::milliseconds& check,
             std::string prefix)
           : _N{N},
             _R{R},
             _T{T},
             _timeout{timeout},
             _check{check},
             _prefix{prefix}
        {}

        virtual ~Base() {}

        virtual void find_taxicab_number(const uint32_t n_start, const uint32_t n_end, const uint32_t n_range) {
            for (uint32_t i = n_start; i < n_end; ++i) {
                for (uint32_t j = 1; j < n_range; ++j) {
                    for (uint32_t k = 1; k < n_range; ++k) {
                        if (i == (j * j * j) + (k * k * k)) {
                            _queue.push({i, j, k});
                        }
                    }
                }
            }
        }

        virtual void save_taxicab_number(bool& loop) = 0;

        virtual void report_taxicab_number(int rank) = 0;

        virtual void run() {
            std::thread consumer_thread;
            std::thread worker_thread;

            _t_start = std::chrono::steady_clock::now();

            // single consumer thread
            consumer_thread = std::thread{&Base::save_taxicab_number, this, std::ref(_loop)};
            consumer_thread.detach();

            // one or more producer threads
            for (int i = 0; i < _T; ++i) {
                uint32_t n_start = i * (_N / _T) + 1;
                uint32_t n_end = (i + 1) * (_N / _T) + 1;

                worker_thread = std::thread{&Base::find_taxicab_number, this, n_start, n_end, _R};
                worker_thread.join();
            }

            _t_end = std::chrono::steady_clock::now();

            // terminate the detached consumer thread
            _loop = false;
            // must sleep more than queue's wait_and_pop_while() duration for a guaranteed graceful exit
            std::this_thread::sleep_for(_timeout + _check);
            // report taxicab numbers with at least this rank
            report_taxicab_number(2);
        }

        virtual void dump_taxicab_number(int rank, std::size_t size_all, std::size_t size_tc) {
            if (_json) {
                _util.dump_json_taxicab_number(rank, size_all, size_tc);
            } else {
                _util.dump_txt_taxicab_number(rank, size_all, size_tc);
            }
        }

        virtual void display_filename(bool on_off) {
            _display = on_off;
        }

        virtual void write_json(bool on_off) {
            _json = on_off;
        }

        virtual std::vector<TaxiCab>& found() {
            return _taxicab;
        }

        virtual void clear() {
            _queue.clear();
            _taxicab.clear();
            _loop = true;
        }

        friend class Utility;

    protected:
        const uint32_t _N;
        const uint32_t _R;
        const uint32_t _T;
        std::chrono::milliseconds _timeout;
        const std::chrono::milliseconds _check;
        std::string _prefix;
        bool _loop = true;
        ConcurrentQueue<std::tuple<uint32_t, uint32_t, uint32_t>> _queue{};
        std::chrono::time_point<std::chrono::steady_clock> _t_start;
        std::chrono::time_point<std::chrono::steady_clock> _t_end;
        std::string _output_dir = "output";
        bool _display = true;
        bool _json = true;
        std::vector<TaxiCab> _taxicab;
        Utility _util{this};
};

// save taxicab number's cubes in std::string
class TaxiCabNumberStr : public Base {
    public:
        TaxiCabNumberStr(uint32_t N,
                         uint32_t R,
                         uint32_t T,
                         std::chrono::milliseconds& timeout,
                         std::chrono::milliseconds& check) : Base(N, R, T, timeout, check, "str_") {}
        void save_taxicab_number(bool& loop) override;
        void report_taxicab_number(const int rank) override;
        void clear() override;

    private:
        std::vector<std::string> split(const std::string& s, const char delimiter);
        std::map<uint32_t, std::string> _cube{};
};

// save taxicab number's cubes in the bits of an uint64_t
class TaxiCabNumberInt : public Base {
    public:
        TaxiCabNumberInt(uint32_t N,
                         uint32_t R,
                         uint32_t T,
                         std::chrono::milliseconds& timeout,
                         std::chrono::milliseconds& check) : Base(N, R, T, timeout, check, "int_") {}
        void save_taxicab_number(bool& loop) override;
        void report_taxicab_number(const int rank) override;
        void clear() override;

    private:
        std::vector<uint16_t> split(const uint64_t x);
        std::map<uint32_t, uint64_t> _cube{};
};


#endif
