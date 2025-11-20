#include <gtest/gtest.h>

#include "logic/attack_tables/attack_tables.h"

namespace logic {

class KnightBitmasksTest : public ::testing::Test 
{
protected:

    void SetUp() override {
        attack_tables::init_attack_tables();
    }
};

TEST_F(KnightBitmasksTest, getKnightBitmask_0_ShouldReturn0x0000000000020400) 
{
    bitmask expected = 0x0000000000020400ULL;
    bitmask actual = attack_tables::knight[0];
    ASSERT_EQ(expected, actual);
}

TEST_F(KnightBitmasksTest, getKnightBitmask_1_ShouldReturn0x0000000000050800) 
{
    bitmask expected = 0x0000000000050800ULL;
    bitmask actual = attack_tables::knight[1];
    ASSERT_EQ(expected, actual);
}

TEST_F(KnightBitmasksTest, getKnightBitmask_2_ShouldReturn0x00000000000A1100) 
{
    bitmask expected = 0x00000000000A1100ULL;
    bitmask actual = attack_tables::knight[2];
    ASSERT_EQ(expected, actual);
}

TEST_F(KnightBitmasksTest, getKnightBitmask_15_ShouldReturn0x0000000040200020) 
{
    bitmask expected = 0x0000000040200020ULL;
    bitmask actual = attack_tables::knight[15];
    ASSERT_EQ(expected, actual);
}

TEST_F(KnightBitmasksTest, getKnightBitmask_20_ShouldReturn0x0000002844004428) 
{
    bitmask expected = 0x0000002844004428ULL;
    bitmask actual = attack_tables::knight[20];
    ASSERT_EQ(expected, actual);
}

TEST_F(KnightBitmasksTest, getKnightBitmask_49_ShouldReturn0x0800080500000000) 
{
    bitmask expected = 0x0800080500000000ULL;
    bitmask actual = attack_tables::knight[49];
    ASSERT_EQ(expected, actual);
}

TEST_F(KnightBitmasksTest, getKnightBitmask_60_ShouldReturn0x0044280000000000) 
{
    bitmask expected = 0x0044280000000000ULL;
    bitmask actual = attack_tables::knight[60];
    ASSERT_EQ(expected, actual);
}

TEST_F(KnightBitmasksTest, getKnightBitmask_63_ShouldReturn0x20400000000000)
{
    bitmask expected = 0x20400000000000ULL;
    bitmask actual = attack_tables::knight[63];
    ASSERT_EQ(expected, actual);
}

} // namespace logic
