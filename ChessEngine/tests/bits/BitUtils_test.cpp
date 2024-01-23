#include <gtest/gtest.h>
#include "ChessEngine/bits/BitUtils.h"

namespace bits {
    class BitUtils : public ::testing::Test {
        protected:
            U64 a;
            U64 b;
            U64 c;
            void SetUp() override {
                a = 0x0000000000FF0501;
                b = 0x0000000000000000;
                c = 0xFFFFFFFFFFFFFFFF;
            }
    };
    
    TEST_F(BitUtils, getBitIndices_a_ShouldReturn0_8_10_16to23) {
        std::vector<int> expected = {0, 8, 10, 16, 17, 18, 19, 20, 21, 22, 23};
        std::vector<int> actual = getBitIndices(a);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitUtils, getBitIndices_b_ShouldReturnEmptyVector) {
        std::vector<int> expected = {};
        std::vector<int> actual = getBitIndices(b);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(BitUtils, getBitIndices_c_ShouldReturn0to63) {
        std::vector<int> expected = {};

        for (int i = 0; i < 64; i++) {
            expected.push_back(i);
        }
        
        std::vector<int> actual = getBitIndices(c);
        ASSERT_EQ(expected, actual);
    }
}