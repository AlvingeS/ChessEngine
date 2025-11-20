#include <gtest/gtest.h>

#include "model/types.h"
#include "logic/attack_tables/attack_tables.h"

namespace logic {

class Kingbitboards : public ::testing::Test 
{
protected:
    void SetUp() override {
        attack_tables::init_attack_tables();
    }
};

TEST_F(Kingbitboards, getKingBitmask_0_ShouldReturn0x0000000000000302) 
{
    bitmask expected = 0x0000000000000302ULL;
    bitmask actual = attack_tables::king[0];
    ASSERT_EQ(expected, actual);
}

TEST_F(Kingbitboards, getKingBitmask_7_ShouldReturn0x000000000000C040) 
{
    bitmask expected = 0x000000000000C040ULL;
    bitmask actual = attack_tables::king[7];
    ASSERT_EQ(expected, actual);
}

TEST_F(Kingbitboards, getKingBitmask_9_ShouldReturn0x0000000000070507) 
{
    bitmask expected = 0x0000000000070507ULL;
    bitmask actual = attack_tables::king[9];
    ASSERT_EQ(expected, actual);
}

TEST_F(Kingbitboards, getKingBitmask_56_ShouldReturn0x0203000000000000) 
{
    bitmask expected = 0x0203000000000000ULL;
    bitmask actual = attack_tables::king[56];
    ASSERT_EQ(expected, actual);
}

TEST_F(Kingbitboards, getKingBitmask_63_ShouldReturn0x40C0000000000000) 
{
    bitmask expected = 0x40C0000000000000ULL;
    bitmask actual = attack_tables::king[63];
    ASSERT_EQ(expected, actual);
}

} // namespace logic