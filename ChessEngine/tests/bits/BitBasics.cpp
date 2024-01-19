#include <gtest/gtest.h>
#include "ChessEngine/bits/headers/BitBasics.h"

namespace bits {
    class BitBasics : public ::testing::Test {
        protected:
            bits::U64 a;
            bits::U64 b;
            bits::U64 c;
            bits::U64 d;

            void SetUp() override {
                a = 0x000000000000FF00ULL;
                b = 0x00000000000000FFULL;
                c = 0x00000000000000F0ULL;
                d = 0x0000000000000000ULL;
            }
    };

    TEST_F(BitBasics, AndBits) {
        bits::U64 expected = 0x0000000000000000ULL;
        bits::U64 actual = bits::bitwiseAnd(a, b);

        ASSERT_EQ(expected, actual) << "AndBits: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;

        expected = 0x00000000000000F0ULL;
        actual = bits::bitwiseAnd(b, c);

        ASSERT_EQ(expected, actual) << "AndBits: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;

        expected = 0x0000000000000000ULL;
        actual = bits::bitwiseAnd(c, d);

        ASSERT_EQ(expected, actual) << "AndBits: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;

        expected = 0x0000000000000000ULL;
        actual = bits::bitwiseAnd(a, d);
        ASSERT_EQ(expected, actual) << "AndBits: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;
    }

    TEST_F(BitBasics, OrBits) {
        bits::U64 expected = 0x000000000000FFFFULL;
        bits::U64 actual = bits::bitwiseOr(a, b);

        ASSERT_EQ(expected, actual) << "OrBits: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;

        expected = 0x00000000000000FFULL;
        actual = bits::bitwiseOr(b, c);

        ASSERT_EQ(expected, actual) << "OrBits: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;

        expected = 0x00000000000000F0ULL;
        actual = bits::bitwiseOr(c, d);

        ASSERT_EQ(expected, actual) << "OrBits: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;

        expected = 0x000000000000FF00ULL;
        actual = bits::bitwiseOr(a, d);
        ASSERT_EQ(expected, actual) << "OrBits: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;
    }


    TEST_F(BitBasics, XorBits) {
        bits::U64 expected = 0x000000000000FFFFULL;
        bits::U64 actual = bits::bitwiseXor(a, b);

        ASSERT_EQ(expected, actual) << "XorBits: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;

        expected = 0x000000000000000FULL;
        actual = bits::bitwiseXor(b, c);

        ASSERT_EQ(expected, actual) << "XorBits: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;

        expected = 0x00000000000000F0ULL;
        actual = bits::bitwiseXor(c, d);

        ASSERT_EQ(expected, actual) << "XorBits: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;

        expected = 0x000000000000FF00ULL;
        actual = bits::bitwiseXor(a, d);
        ASSERT_EQ(expected, actual) << "XorBits: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;
    }

    TEST_F(BitBasics, NotBits) {
        bits::U64 expected = 0xFFFFFFFFFFFF00FFULL;
        bits::U64 actual = bits::bitwiseNot(a);

        ASSERT_EQ(expected, actual) << "NotBits: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;

        expected = 0xFFFFFFFFFFFFFF00ULL;
        actual = bits::bitwiseNot(b);

        ASSERT_EQ(expected, actual) << "NotBits: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;

        expected = 0xFFFFFFFFFFFFFF0FULL;
        actual = bits::bitwiseNot(c);

        ASSERT_EQ(expected, actual) << "NotBits: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;

        expected = 0xFFFFFFFFFFFFFFFFULL;
        actual = bits::bitwiseNot(d);
        ASSERT_EQ(expected, actual) << "NotBits: Expected: 0x" << std::hex << expected 
                                    << ", Actual: 0x" << std::hex << actual;
    }

    TEST_F(BitBasics, GetBit) {
        bool expected = 0;
        bool actual = bits::getBit(a, 0);

        ASSERT_EQ(expected, actual) << "GetBit: Expected: " << expected 
                                    << ", Actual: " << actual;

        expected = 1;
        actual = bits::getBit(a, 9);

        ASSERT_EQ(expected, actual) << "GetBit: Expected: " << expected 
                                    << ", Actual: " << actual;

        expected = 1;
        actual = bits::getBit(a, 15);

        ASSERT_EQ(expected, actual) << "GetBit: Expected: " << expected 
                                    << ", Actual: " << actual;

        expected = 0;
        actual = bits::getBit(a, 16);

        ASSERT_EQ(expected, actual) << "GetBit: Expected: " << expected 
                                    << ", Actual: " << actual;
    }
}