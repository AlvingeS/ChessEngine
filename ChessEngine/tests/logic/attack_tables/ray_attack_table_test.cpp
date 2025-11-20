#include <gtest/gtest.h>

#include "logic/attack_tables/attack_tables.h"

namespace logic {

class RayBitmasks_test : public ::testing::Test 
{
protected:
    int idx_one;
    int idx_two;
    int idx_three;
    int idx_four;
    int idx_five;
    int idx_six;
    bitmask w_pieces_mask;
    bitmask b_pieces_mask;

    void SetUp() override {
        attack_tables::init_attack_tables();
        idx_one = 0;
        idx_two = 7;
        idx_three = 28;
        idx_four = 32;
        idx_five = 56;
        idx_six = 63;
        w_pieces_mask = 0x000000000000FFFFULL;
        b_pieces_mask = 0xFFFF000000000000ULL;
    }
};

TEST_F(RayBitmasks_test, getNorthRay_Pos0_ShouldReturn0x0101010101010100) 
{
    bitmask expected = 0x0101010101010100ULL;
    bitmask actual = attack_tables::line_ray[idx_one][LineDir::N];
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getNorthRay_Pos7_ShouldReturn0x8080808080808000) 
{
    bitmask expected = 0x8080808080808000ULL;
    bitmask actual = attack_tables::line_ray[idx_two][LineDir::N];
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getNorthRay_Pos28_ShouldReturn0x1010101000000000) 
{
    bitmask expected = 0x1010101000000000ULL;
    bitmask actual = attack_tables::line_ray[idx_three][LineDir::N];
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getNorthRay_Pos32_ShouldReturn0x0101010000000000) 
{
    bitmask expected = 0x0101010000000000ULL;
    bitmask actual = attack_tables::line_ray[idx_four][LineDir::N];
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getNorthRay_Pos56_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = attack_tables::line_ray[idx_five][LineDir::N];
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getNorthRay_Pos63_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = attack_tables::line_ray[idx_six][LineDir::N];
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getNorthEastRay_Pos0_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = attack_tables::diag_ray[idx_one][DiagDir::NE];
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getNorthEastRay_Pos7_ShouldReturn0x0102040810204000) 
{
    bitmask expected = 0x0102040810204000ULL;
    bitmask actual = attack_tables::diag_ray[idx_two][DiagDir::NE];
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getNorthEastRay_Pos28_ShouldReturn0x0102040800000000) 
{
    bitmask expected = 0x0102040800000000ULL;
    bitmask actual = attack_tables::diag_ray[idx_three][DiagDir::NE];
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getNorthEastRay_Pos32_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000;
    bitmask actual = attack_tables::diag_ray[idx_four][DiagDir::NE];
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getNorthEastRay_Pos56_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000;
    bitmask actual = attack_tables::diag_ray[idx_five][DiagDir::NE];
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getNorthEastRay_Pos63_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000;
    bitmask actual = attack_tables::diag_ray[idx_six][DiagDir::NE];
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getEastRay_Pos0_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = attack_tables::line_ray[idx_one][LineDir::E];
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getEastRay_Pos7_ShouldReturn0x000000000000007F) 
{
    bitmask expected = 0x000000000000007FULL;
    bitmask actual = attack_tables::line_ray[idx_two][LineDir::E];
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getEastRay_Pos28_ShouldReturn0x000000000F000000) 
{
    bitmask expected = 0x000000000F000000ULL;
    bitmask actual = attack_tables::line_ray[idx_three][LineDir::E];
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getEastRay_Pos32_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = attack_tables::line_ray[idx_four][LineDir::E];
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getEastRay_Pos56_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = attack_tables::line_ray[idx_five][LineDir::E];
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getEastRay_Pos63_ShouldReturn0x7F000000000000000) 
{
    bitmask expected = 0x7F00000000000000ULL;
    bitmask actual = attack_tables::line_ray[idx_six][LineDir::E];
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthEastRay_Pos0_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = attack_tables::diag_ray[idx_one][DiagDir::SE];
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthEastRay_Pos7_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = attack_tables::diag_ray[idx_two][DiagDir::SE];
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthEastRay_Pos28_ShouldReturn0x0000000000080402) 
{
    bitmask expected = 0x0000000000080402ULL;
    bitmask actual = attack_tables::diag_ray[idx_three][DiagDir::SE];
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthEastRay_Pos32_ShouldReturn0x0000000000000000)
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = attack_tables::diag_ray[idx_four][DiagDir::SE];
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthEastRay_Pos56_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = attack_tables::diag_ray[idx_five][DiagDir::SE];
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthEastRay_Pos63_ShouldReturn0x0040201008040201) 
{
    bitmask expected = 0x0040201008040201ULL;
    bitmask actual = attack_tables::diag_ray[idx_six][DiagDir::SE];
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthRay_Pos0_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = attack_tables::line_ray[idx_one][LineDir::S];
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthRay_Pos7_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = attack_tables::line_ray[idx_two][LineDir::S];
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthRay_Pos28_ShouldReturn0x0000000000101010) 
{
    bitmask expected = 0x0000000000101010ULL;
    bitmask actual = attack_tables::line_ray[idx_three][LineDir::S];
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthRay_Pos32_ShouldReturn0x0000000001010101) 
{
    bitmask expected = 0x0000000001010101ULL;
    bitmask actual = attack_tables::line_ray[idx_four][LineDir::S];
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthRay_Pos56_ShouldReturn0x001010101010101) 
{
    bitmask expected = 0x001010101010101ULL;
    bitmask actual = attack_tables::line_ray[idx_five][LineDir::S];
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthRay_Pos63_ShouldReturn0x0080808080808080) 
{
    bitmask expected = 0x0080808080808080ULL;
    bitmask actual = attack_tables::line_ray[idx_six][LineDir::S];
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthWestRay_Pos0_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = attack_tables::diag_ray[idx_one][DiagDir::SW];
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthWestRay_Pos7_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = attack_tables::diag_ray[idx_two][DiagDir::SW];
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthWestRay_Pos28_ShouldReturn0x0000000000204080) 
{
    bitmask expected = 0x0000000000204080ULL;
    bitmask actual = attack_tables::diag_ray[idx_three][DiagDir::SW];
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthWestRay_Pos32_ShouldReturn0x0000000002040810) 
{
    bitmask expected = 0x0000000002040810ULL;
    bitmask actual = attack_tables::diag_ray[idx_four][DiagDir::SW];
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthWestRay_Pos56_ShouldReturn0x002040810204080) 
{
    bitmask expected = 0x002040810204080ULL;
    bitmask actual = attack_tables::diag_ray[idx_five][DiagDir::SW];
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthWestRay_Pos63_ShouldReturn0x0000000000000000)
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = attack_tables::diag_ray[idx_six][DiagDir::SW];
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getWestRay_Pos0_ShouldReturn0x00000000000000FE) 
{
    bitmask expected = 0x00000000000000FEULL;
    bitmask actual = attack_tables::line_ray[idx_one][LineDir::W];
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getWestRay_Pos7_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = attack_tables::line_ray[idx_two][LineDir::W];
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getWestRay_Pos28_ShouldReturn0x00000000E0000000) 
{
    bitmask expected = 0x00000000E0000000ULL;
    bitmask actual = attack_tables::line_ray[idx_three][LineDir::W];
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getWestRay_Pos32_ShouldReturn0x000000FE00000000) 
{
    bitmask expected = 0x000000FE00000000ULL;
    bitmask actual = attack_tables::line_ray[idx_four][LineDir::W];
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getWestRay_Pos56_ShouldReturn0xFE00000000000000) 
{
    bitmask expected = 0xFE00000000000000ULL;
    bitmask actual = attack_tables::line_ray[idx_five][LineDir::W];
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getWestRay_Pos63_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = attack_tables::line_ray[idx_six][LineDir::W];
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getNorthWestRay_Pos0_ShouldReturn0x8040201008040200) 
{
    bitmask expected = 0x8040201008040200ULL;
    bitmask actual = attack_tables::diag_ray[idx_one][DiagDir::NW];
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getNorthWestRay_Pos7_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = attack_tables::diag_ray[idx_two][DiagDir::NW];
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getNorthWestRay_Pos28_ShouldReturn0x0080402000000000) 
{
    bitmask expected = 0x0080402000000000ULL;
    bitmask actual = attack_tables::diag_ray[idx_three][DiagDir::NW];
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getNorthWestRay_Pos32_ShouldReturn0x0804020000000000) 
{
    bitmask expected = 0x0804020000000000ULL;
    bitmask actual = attack_tables::diag_ray[idx_four][DiagDir::NW];
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getNorthWestRay_Pos56_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = attack_tables::diag_ray[idx_five][DiagDir::NW];
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getNorthWestRay_Pos63_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = attack_tables::diag_ray[idx_six][DiagDir::NW];
    ASSERT_EQ(expected, actual);
}

} // namespace logic
