#include <gtest/gtest.h>
#include "ChessEngine/utils/headers/bit_logic.h"

namespace utils {
    class BitLogic : public ::testing::Test {
        protected:
            utils::U64 a;
            utils::U64 b;
            utils::U64 c;
            utils::U64 d;

            void SetUp() override {
                a = 0x000000000000FF00ULL;
                b = 0x00000000000000FFULL;
                c = 0x00000000000000F0ULL;
                d = 0x0000000000000000ULL;
            }
    };

    TEST_F(BitLogic, AndBits) {
        utils::U64 expected = 0x0000000000000000ULL;
        utils::U64 actual = utils::and_bits(a, b);

        ASSERT_EQ(expected, actual) << "AndBits: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;

        expected = 0x00000000000000F0ULL;
        actual = utils::and_bits(b, c);

        ASSERT_EQ(expected, actual) << "AndBits: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;

        expected = 0x0000000000000000ULL;
        actual = utils::and_bits(c, d);

        ASSERT_EQ(expected, actual) << "AndBits: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;

        expected = 0x0000000000000000ULL;
        actual = utils::and_bits(a, d);
        ASSERT_EQ(expected, actual) << "AndBits: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;
    }

    TEST_F(BitLogic, OrBits) {
        utils::U64 expected = 0x000000000000FFFFULL;
        utils::U64 actual = utils::or_bits(a, b);

        ASSERT_EQ(expected, actual) << "OrBits: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;

        expected = 0x00000000000000FFULL;
        actual = utils::or_bits(b, c);

        ASSERT_EQ(expected, actual) << "OrBits: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;

        expected = 0x00000000000000F0ULL;
        actual = utils::or_bits(c, d);

        ASSERT_EQ(expected, actual) << "OrBits: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;

        expected = 0x000000000000FF00ULL;
        actual = utils::or_bits(a, d);
        ASSERT_EQ(expected, actual) << "OrBits: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;
    }


    TEST_F(BitLogic, XorBits) {
        utils::U64 expected = 0x000000000000FFFFULL;
        utils::U64 actual = utils::xor_bits(a, b);

        ASSERT_EQ(expected, actual) << "XorBits: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;

        expected = 0x000000000000000FULL;
        actual = utils::xor_bits(b, c);

        ASSERT_EQ(expected, actual) << "XorBits: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;

        expected = 0x00000000000000F0ULL;
        actual = utils::xor_bits(c, d);

        ASSERT_EQ(expected, actual) << "XorBits: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;

        expected = 0x000000000000FF00ULL;
        actual = utils::xor_bits(a, d);
        ASSERT_EQ(expected, actual) << "XorBits: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;
    }

    TEST_F(BitLogic, NotBits) {
        utils::U64 expected = 0xFFFFFFFFFFFF00FFULL;
        utils::U64 actual = utils::not_bits(a);

        ASSERT_EQ(expected, actual) << "NotBits: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;

        expected = 0xFFFFFFFFFFFFFF00ULL;
        actual = utils::not_bits(b);

        ASSERT_EQ(expected, actual) << "NotBits: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;

        expected = 0xFFFFFFFFFFFFFF0FULL;
        actual = utils::not_bits(c);

        ASSERT_EQ(expected, actual) << "NotBits: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;

        expected = 0xFFFFFFFFFFFFFFFFULL;
        actual = utils::not_bits(d);
        ASSERT_EQ(expected, actual) << "NotBits: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;
    }

    TEST_F(BitLogic, GetBit) {
        bool expected = 0;
        bool actual = utils::get_bit(a, 0);

        ASSERT_EQ(expected, actual) << "GetBit: Expected: " << expected 
                                    << ", Actual: " << actual;

        expected = 1;
        actual = utils::get_bit(a, 9);

        ASSERT_EQ(expected, actual) << "GetBit: Expected: " << expected 
                                    << ", Actual: " << actual;

        expected = 1;
        actual = utils::get_bit(a, 15);

        ASSERT_EQ(expected, actual) << "GetBit: Expected: " << expected 
                                    << ", Actual: " << actual;

        expected = 0;
        actual = utils::get_bit(a, 16);

        ASSERT_EQ(expected, actual) << "GetBit: Expected: " << expected 
                                    << ", Actual: " << actual;
    }
}