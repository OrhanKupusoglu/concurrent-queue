#include "../include/test_util.h"

int taxicab_count(uint32_t max) {
    int count = 0;

    for (auto& ref : ref_taxicab) {
        if (ref.first <= max) {
            ++count;
        } else {
            break;
        }
    }

    return count;
}

bool check_taxicab(std::vector<TaxiCab>& calc_taxicab) {
    bool res = true;

    for (auto& calc : calc_taxicab) {
        auto found = ref_taxicab.find(calc.taxicab_no);
        if (found == ref_taxicab.end()) {
            std::cerr << "ERROR: taxicab number not found in the reference list: " << calc.taxicab_no << '\n';
            res = false;
            break;
        } else {
            int i = 0;
            auto& ref_cube = found->second;
            auto& calc_cube = calc.cube;
            for (auto& r_cube : ref_cube) {
                auto& c_cube = calc_cube[i++];
                std::cout << "\t" << calc.taxicab_no << " = " << c_cube.first << "^3 + " << c_cube.second << "^3\n";
                if ((r_cube.first != c_cube.first) || (r_cube.second != c_cube.second)) {
                    std::cerr << "ERROR: taxicab number's cubes do not match the reference list: " << calc.taxicab_no << '\n';
                    res = false;
                    break;
                }
            }
        }
    }

    return res;
}
