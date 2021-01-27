#include "gtest/gtest.h"
#include "../../include/taxicab_number.h"
#include "../include/test_common.h"

CommonTests<TaxiCabNumberStr> test_taxicab_str{"taxicab_str"};

TEST(TestTaxiCabStr, SizePow1) {
    test_taxicab_str.test_size_small(1, 1, 1);
}

TEST(TestTaxiCabStr, SizePow2) {
    test_taxicab_str.test_size_small(2, 1, 1);
}

TEST(TestTaxiCabStr, SizePow3) {
    test_taxicab_str.test_size_small(3, 2, 1);
}

TEST(TestTaxiCabStr, SizePow4) {
    test_taxicab_str.test_size_big(4, 2, 1);
}

TEST(TestTaxiCabStr, SizePow5) {
    test_taxicab_str.test_size_big(5, 2, 1);
}

TEST(TestTaxiCabStr, SizePow6) {
    test_taxicab_str.test_size_big(6, 2, 1);
}

#ifdef TEST_XLARGE
TEST(TestTaxiCabStr, SizePow7) {
    test_taxicab_str.test_size_big(7, 3, 2);
}

TEST(TestTaxiCabStr, SizePow8) {
    test_taxicab_str.test_size_big(8, 3, 2);
}
#endif
