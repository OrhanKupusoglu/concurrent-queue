#include <iostream>
#include <iomanip>
#include <cmath>

#include "../include/taxicab_number.h"

int main(int argc, char **argv) {
    int p = 0;
    int p_min = 4;
    int p_max = 10;
    int p_def = 5;
    uint32_t N = 0;
    int r = 0;
    int r_min = 1;
    int r_max = 3;
    int r_def = 2;
    uint32_t R = 0;
    uint32_t T = 0;
    uint32_t t_min = 1;
    uint32_t t_max = 1024;
    uint32_t t_def = t_min;
    const int width = 36;

    if (argc > 1 && (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help" || std::string(argv[1]) == "help")) {
        std::string name{argv[0]};
        std::cout << "usage:\n\t" << name << " " << p_def << " " << r_def << " " << t_def << '\n'
                  << "\t" << name << " p r t\n"
                  << "\tp: gives the range for taxicab numbers [1, 10^p]\n"
                  << "\tr: gives the range for cubes [1, 10^r]\n"
                  << "\tt: gives the number of producer threads\n"
                  << "version: " << APP_VERSION << '\n'
                  << "compiler: " << CXX_COMPILER << '\n'
                  << "standard: " << CXX_STANDARD << '\n'
                  << "hardware threads: " << std::thread::hardware_concurrency() << '\n';
        return 0;
    }

    if (argc > 1) {
        try {
            p = std::stoi(argv[1]);
        } catch (std::invalid_argument const &e) {
            std::cerr << "ERROR: invalid argument for limit number - argv[1] = " << argv[1] << '\n';
            return 1;
        } catch (std::out_of_range const &e) {
            std::cerr << "ERROR: out of limit number- argv[1] = " << argv[1] << '\n';
            return 2;
        }
        if (p < p_min) {
            std::cerr << "ERROR: power of limit number is too small: " << argv[1] << ", cannot be less than 10^" << p_min << '\n';
            return 3;
        }
        if (p > p_max) {
            std::cerr << "ERROR: power of limit number is too big: " << argv[1] << ", cannot be greater than 10^" << p_max << '\n';
            return 4;
        }
    } else {
        p = p_def;
    }

    N = std::pow(10, p);

    if (argc > 2) {
        try {
            r = std::stoi(argv[2]);
        } catch (std::invalid_argument const &e) {
            std::cerr << "ERROR: invalid argument for integer cube range - argv[2] = " << argv[2] << '\n';
            return 5;
        } catch (std::out_of_range const &e) {
            std::cerr << "ERROR: out of range for integer cube range - argv[2] = " << argv[2] << '\n';
            return 6;
        }
        if (r < r_min) {
            std::cerr << "ERROR:  power of integer cube range is too small: " << argv[2] << ", cannot be less than 10^" << r_min << '\n';
            return 7;
        }
        if (r > r_max) {
            std::cerr << "ERROR:  power of integer cube range is too big: " << argv[2] << ", cannot be greater than 10^" << r_max << '\n';
            return 8;
        }
    } else {
        r = r_def;
    }

    R = std::pow(10, r);

    // worker threads are producers
    if (argc > 3) {
        try {
            T = std::stol(argv[3]);
        } catch (std::invalid_argument const &e) {
            std::cerr << "ERROR: invalid argument for worker thread number - argv[3] = " << argv[3] << '\n';
            return 9;
        } catch (std::out_of_range const &e) {
            std::cerr << "ERROR: out of range worker thread number - argv[3] = " << argv[3] << '\n';
            return 10;
        }
        if (T < t_min) {
            std::cerr << "ERROR:  worker thread number is too small: " << argv[3] << ", cannot be less than " << t_min << '\n';
            return 11;
        }
        if (T > t_max) {
            std::cerr << "ERROR:  worker thread number is too big: " << argv[3] << ", cannot be more than " << t_max << '\n';
            return 12;
        }
    } else {
        T = t_def;
    }

    std::cout << std::setw(width) << std::right << "taxicab range N = " << N << '\n';
    std::cout << std::setw(width) << std::right << "integer cube range R = " << R << '\n';
    std::cout << std::setw(width) << std::right << "number of worker threads T = " << T << '\n';

    std::chrono::milliseconds timeout{10};
    std::chrono::milliseconds check{1};

#ifdef SOLUTION_INT
    TaxiCabNumberInt
#else
    TaxiCabNumberStr
#endif
    taxicab_number{N, R, T, timeout, check};

    taxicab_number.run();

    return 0;
}
