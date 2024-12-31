#include <gtest/gtest.h>

#include "ChessEngine/src/logic/movegen/bitmasks/KnightBitmasks.h"

namespace logic {

class KnightBitmasks : public ::testing::Test 
{
protected:

    void SetUp() override {

    }
};

TEST_F(KnightBitmasks, getKnightBitmask_0_ShouldReturn0x0000000000020400) 
{
    bitmask expected = 0x0000000000020400ULL;
    bitmask actual = getKnightBitmask(0);
    ASSERT_EQ(expected, actual);
}

TEST_F(KnightBitmasks, getKnightBitmask_1_ShouldReturn0x0000000000050800) 
{
    bitmask expected = 0x0000000000050800ULL;
    bitmask actual = getKnightBitmask(1);
    ASSERT_EQ(expected, actual);
}

TEST_F(KnightBitmasks, getKnightBitmask_2_ShouldReturn0x00000000000A1100) 
{
    bitmask expected = 0x00000000000A1100ULL;
    bitmask actual = getKnightBitmask(2);
    ASSERT_EQ(expected, actual);
}

TEST_F(KnightBitmasks, getKnightBitmask_15_ShouldReturn0x0000000040200020) 
{
    bitmask expected = 0x0000000040200020ULL;
    bitmask actual = getKnightBitmask(15);
    ASSERT_EQ(expected, actual);
}

TEST_F(KnightBitmasks, getKnightBitmask_20_ShouldReturn0x0000002844004428) 
{
    bitmask expected = 0x0000002844004428ULL;
    bitmask actual = getKnightBitmask(20);
    ASSERT_EQ(expected, actual);
}

TEST_F(KnightBitmasks, getKnightBitmask_49_ShouldReturn0x0800080500000000) 
{
    bitmask expected = 0x0800080500000000ULL;
    bitmask actual = getKnightBitmask(49);
    ASSERT_EQ(expected, actual);
}

TEST_F(KnightBitmasks, getKnightBitmask_60_ShouldReturn0x0044280000000000) 
{
    bitmask expected = 0x0044280000000000ULL;
    bitmask actual = getKnightBitmask(60);
    ASSERT_EQ(expected, actual);
}

TEST_F(KnightBitmasks, getKnightBitmask_63_ShouldReturn0x20400000000000)
{
    bitmask expected = 0x20400000000000ULL;
    bitmask actual = getKnightBitmask(63);
    ASSERT_EQ(expected, actual);
}

} // namespace logic
