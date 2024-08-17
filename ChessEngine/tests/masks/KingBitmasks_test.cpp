#include <gtest/gtest.h>
#include "ChessEngine/masks/KingBitmasks.h"

#include "ChessEngine/common.h"

namespace masks {
    class Kingbitboards : public ::testing::Test {
        protected:

            void SetUp() override {

            }
    };

    TEST_F(Kingbitboards, getKingBitmask_0_ShouldReturn0x0000000000000302) {
        bitboard expected = 0x0000000000000302ULL;
        bitboard actual = getKingBitmask(0);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(Kingbitboards, getKingBitmask_7_ShouldReturn0x000000000000C040) {
        bitboard expected = 0x000000000000C040ULL;
        bitboard actual = getKingBitmask(7);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(Kingbitboards, getKingBitmask_9_ShouldReturn0x0000000000070507) {
        bitboard expected = 0x0000000000070507ULL;
        bitboard actual = getKingBitmask(9);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(Kingbitboards, getKingBitmask_56_ShouldReturn0x0203000000000000) {
        bitboard expected = 0x0203000000000000ULL;
        bitboard actual = getKingBitmask(56);
        ASSERT_EQ(expected, actual);
    }

    TEST_F(Kingbitboards, getKingBitmask_63_ShouldReturn0x40C0000000000000) {
        bitboard expected = 0x40C0000000000000ULL;
        bitboard actual = getKingBitmask(63);
        ASSERT_EQ(expected, actual);
    }
}
