#include <gtest/gtest.h>

#include "logic/attack_tables/attack_tables.h"

namespace logic {

class PawnBitmasksTest : public ::testing::Test 
{
protected:
    void SetUp() override {
        attack_tables::init_attack_tables();
    }
};

TEST_F(PawnBitmasksTest, getCapturePawnMovesBitmask_8asWhite_ShouldReturn0x0000000001030000) {
    bitmask expected_capture = 0x0000000000020000ULL;
    bitmask expected_quiet = 0x0000000001010000ULL;
    bitmask actual_capture = attack_tables::w_pawn_capture[8];
    bitmask actual_quiet = attack_tables::w_pawn_quiet[8];
    ASSERT_EQ(expected_capture, actual_capture);
    ASSERT_EQ(expected_quiet, actual_quiet);
}

TEST_F(PawnBitmasksTest, getCapturePawnMovesBitmask_12asWhite_ShouldReturn0x0000000010380000) {
    bitmask expected_quiet = 0x0000000010100000ULL;
    bitmask expected_capture = 0x0000000000280000ULL;
    bitmask actual_capture = attack_tables::w_pawn_capture[12];
    bitmask actual_quiet = attack_tables::w_pawn_quiet[12];
    ASSERT_EQ(expected_capture, actual_capture);
    ASSERT_EQ(expected_quiet, actual_quiet);
}

TEST_F(PawnBitmasksTest, getCapturePawnMovesBitmask_15asWhite_ShouldReturn0x0000000080C00000) {
    bitmask expected_capture = 0x0000000000400000ULL;
    bitmask expected_quiet = 0x0000000080800000ULL;
    bitmask actual_capture = attack_tables::w_pawn_capture[15];
    bitmask actual_quiet = attack_tables::w_pawn_quiet[15];
    ASSERT_EQ(expected_capture, actual_capture);
    ASSERT_EQ(expected_quiet, actual_quiet);
}

TEST_F(PawnBitmasksTest, getCapturePawnMovesBitmask_20asWhite_ShouldReturn0x000000038000000) {
    bitmask expected_capture = 0x0000000028000000ULL;
    bitmask expected_quiet = 0x0000000010000000ULL;
    bitmask actual_capture = attack_tables::w_pawn_capture[20];
    bitmask actual_quiet = attack_tables::w_pawn_quiet[20];
    ASSERT_EQ(expected_capture, actual_capture);
    ASSERT_EQ(expected_quiet, actual_quiet);
}

TEST_F(PawnBitmasksTest, getCapturePawnMovesBitmask_48asBlack_ShouldReturn0x000030100000000) {
    bitmask expected_capture = 0x0000020000000000ULL;
    bitmask expected_quiet = 0x0000010100000000ULL;
    bitmask actual_capture = attack_tables::b_pawn_capture[48];
    bitmask actual_quiet = attack_tables::b_pawn_quiet[48];
    ASSERT_EQ(expected_capture, actual_capture);
    ASSERT_EQ(expected_quiet, actual_quiet);
}

TEST_F(PawnBitmasksTest, getCapturePawnMovesBitmask_52asBlack_ShouldReturn0x0000381000000000) {
    bitmask expected_capture = 0x0000280000000000ULL;
    bitmask expected_quiet = 0x0000101000000000ULL;
    bitmask actual_capture = attack_tables::b_pawn_capture[52];
    bitmask actual_quiet = attack_tables::b_pawn_quiet[52];
    ASSERT_EQ(expected_capture, actual_capture);
    ASSERT_EQ(expected_quiet, actual_quiet);
}

TEST_F(PawnBitmasksTest, getCapturePawnMovesBitmask_55asBlack_ShouldReturn0x0000C08000000000) {
    bitmask expected_capture = 0x0000400000000000ULL;
    bitmask expected_quiet = 0x0000808000000000ULL;
    bitmask actual_capture = attack_tables::b_pawn_capture[55];
    bitmask actual_quiet = attack_tables::b_pawn_quiet[55];
    ASSERT_EQ(expected_capture, actual_capture);
    ASSERT_EQ(expected_quiet, actual_quiet);
}

TEST_F(PawnBitmasksTest, getCapturePawnMovesBitmask_44asBlack_ShouldReturn0x0000003800000000) {
    bitmask expected_capture = 0x0000002800000000ULL;
    bitmask expected_quiet = 0x0000001000000000ULL;
    bitmask actual_capture = attack_tables::b_pawn_capture[44];
    bitmask actual_quiet = attack_tables::b_pawn_quiet[44];
    ASSERT_EQ(expected_capture, actual_capture);
    ASSERT_EQ(expected_quiet, actual_quiet);
}
    
} // namespace logic

