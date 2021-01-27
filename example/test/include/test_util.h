#ifndef TEST_UTIL_H
#define TEST_UTIL_H

#include "../../include/taxicab_number.h"

extern std::map<uint32_t, std::vector<std::pair<uint16_t, uint16_t>>> ref_taxicab;

int taxicab_count(uint32_t max);
bool check_taxicab(std::vector<TaxiCab>& calc_taxicab);

#endif
