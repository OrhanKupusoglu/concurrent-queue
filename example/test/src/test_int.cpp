#include "gtest/gtest.h"
#include "../../include/taxicab_number.h"
#include "../include/test_common.h"

CommonTests<TaxiCabNumberInt> test_taxicab_int{"taxicab_int"};

TEST(TestTaxiCabInt, SizePow1) {
    test_taxicab_int.test_size_small(1, 1, 1);
}

TEST(TestTaxiCabInt, SizePow2) {
    test_taxicab_int.test_size_small(2, 1, 1);
}

TEST(TestTaxiCabInt, SizePow3) {
    test_taxicab_int.test_size_small(3, 2, 1);
}

TEST(TestTaxiCabInt, SizePow4) {
    test_taxicab_int.test_size_big(4, 2, 1);
}

TEST(TestTaxiCabInt, SizePow5) {
    test_taxicab_int.test_size_big(5, 2, 1);
}

TEST(TestTaxiCabInt, SizePow6) {
    test_taxicab_int.test_size_big(6, 2, 1);
}

#ifdef TEST_XLARGE
TEST(TestTaxiCabInt, SizePow7) {
    test_taxicab_int.test_size_big(7, 3, 2);
}

TEST(TestTaxiCabStr, SizePow8) {
    test_taxicab_str.test_size_big(8, 3, 2);
}
#endif
