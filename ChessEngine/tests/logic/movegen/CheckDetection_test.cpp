#include <gtest/gtest.h>

#include "BaseGenerator_test.h"

namespace logic {

class MoveGeneratorCheckDetectionTest : public BaseGenerator {
protected:
    std::string fenPawnChecks;
    std::string fenKnightChecks;
    std::string fenBishopChecksTrue;
    std::string fenBishopChecksFalse;
    std::string fenRookChecksTrue;
    std::string fenRookChecksFalse;
    std::string fenQueenChecksTrue;
    std::string fenQueenChecksFalse;

    void SetUp() override {
        BaseGenerator::SetUp();
        fenPawnChecks = "8/8/5k2/6P1/8/2p5/3K4/8";
        fenKnightChecks = "8/8/5k2/3N4/8/3n4/1K6/8";
        fenBishopChecksTrue = "8/4B3/5k2/4b3/8/8/1K6/8";
        fenBishopChecksFalse = "3B4/4N3/5k2/4b3/3p4/8/1K6/8";
        fenRookChecksTrue = "8/8/1R3k2/8/8/8/1K6/1r6";
        fenRookChecksFalse = "5R2/5p2/5k2/8/8/8/1K2N2r/8";
        fenQueenChecksTrue = "8/8/5k1Q/8/8/8/1K6/q7";
        fenQueenChecksFalse = "8/8/5kqQ/1q6/1P6/8/1K6/8";
    }
};

TEST_F(MoveGeneratorCheckDetectionTest, checkDetection_fenPawnChecksWhite_ShouldReturnTrue) 
{
    io::setBoardFromFen(fenPawnChecks, bitboards, gameStateBitmasks, squaresLookup);
    ASSERT_TRUE(moveGenerator.isInCheck(true));
}

TEST_F(MoveGeneratorCheckDetectionTest, checkDetection_fenPawnChecksBlack_ShouldReturnTrue)
{
    io::setBoardFromFen(fenPawnChecks, bitboards, gameStateBitmasks, squaresLookup);
    ASSERT_TRUE(moveGenerator.isInCheck(false));
}

TEST_F(MoveGeneratorCheckDetectionTest, checkDetection_fenKnightChecksWhite_ShouldReturnTrue)
{
    io::setBoardFromFen(fenKnightChecks, bitboards, gameStateBitmasks, squaresLookup);
    ASSERT_TRUE(moveGenerator.isInCheck(true));
}

TEST_F(MoveGeneratorCheckDetectionTest, checkDetection_fenKnightChecksBlack_ShouldReturnTrue)
{
    io::setBoardFromFen(fenKnightChecks, bitboards, gameStateBitmasks, squaresLookup);
    ASSERT_TRUE(moveGenerator.isInCheck(false));
}

TEST_F(MoveGeneratorCheckDetectionTest, checkDetection_fenBishopChecksTrueWhite_ShouldReturnTrue)
{
    io::setBoardFromFen(fenBishopChecksTrue, bitboards, gameStateBitmasks, squaresLookup);
    ASSERT_TRUE(moveGenerator.isInCheck(true));
}

TEST_F(MoveGeneratorCheckDetectionTest, checkDetection_fenBishopChecksTrueBlack_ShouldReturnTrue)
{
    io::setBoardFromFen(fenBishopChecksTrue, bitboards, gameStateBitmasks, squaresLookup);
    ASSERT_TRUE(moveGenerator.isInCheck(false));
}

TEST_F(MoveGeneratorCheckDetectionTest, checkDetection_fenBishopChecksFalseWhite_ShouldReturnFalse)
{
    io::setBoardFromFen(fenBishopChecksFalse, bitboards, gameStateBitmasks, squaresLookup);
    ASSERT_FALSE(moveGenerator.isInCheck(true));
}

TEST_F(MoveGeneratorCheckDetectionTest, checkDetection_fenBishopChecksFalseBlack_ShouldReturnFalse)
{
    io::setBoardFromFen(fenBishopChecksFalse, bitboards, gameStateBitmasks, squaresLookup);
    ASSERT_FALSE(moveGenerator.isInCheck(false));
}

TEST_F(MoveGeneratorCheckDetectionTest, checkDetection_fenRookChecksTrueWhite_ShouldReturnTrue)
{
    io::setBoardFromFen(fenRookChecksTrue, bitboards, gameStateBitmasks, squaresLookup);
    ASSERT_TRUE(moveGenerator.isInCheck(true));
}

TEST_F(MoveGeneratorCheckDetectionTest, checkDetection_fenRookChecksTrueBlack_ShouldReturnTrue)
{
    io::setBoardFromFen(fenRookChecksTrue, bitboards, gameStateBitmasks, squaresLookup);
    ASSERT_TRUE(moveGenerator.isInCheck(false));
}

TEST_F(MoveGeneratorCheckDetectionTest, checkDetection_fenRookChecksFalseWhite_ShouldReturnFalse)
{
    io::setBoardFromFen(fenRookChecksFalse, bitboards, gameStateBitmasks, squaresLookup);
    ASSERT_FALSE(moveGenerator.isInCheck(true));
}

TEST_F(MoveGeneratorCheckDetectionTest, checkDetection_fenRookChecksFalseBlack_ShouldReturnFalse)
{
    io::setBoardFromFen(fenRookChecksFalse, bitboards, gameStateBitmasks, squaresLookup);
    ASSERT_FALSE(moveGenerator.isInCheck(false));
}

TEST_F(MoveGeneratorCheckDetectionTest, checkDetection_fenQueenChecksTrueWhite_ShouldReturnTrue)
{
    io::setBoardFromFen(fenQueenChecksTrue, bitboards, gameStateBitmasks, squaresLookup);
    ASSERT_TRUE(moveGenerator.isInCheck(true));
}

TEST_F(MoveGeneratorCheckDetectionTest, checkDetection_fenQueenChecksTrueBlack_ShouldReturnTrue)
{
    io::setBoardFromFen(fenQueenChecksTrue, bitboards, gameStateBitmasks, squaresLookup);
    ASSERT_TRUE(moveGenerator.isInCheck(false));
}

TEST_F(MoveGeneratorCheckDetectionTest, checkDetection_fenQueenChecksFalseWhite_ShouldReturnFalse)
{
    io::setBoardFromFen(fenQueenChecksFalse, bitboards, gameStateBitmasks, squaresLookup);
    ASSERT_FALSE(moveGenerator.isInCheck(true));
}

TEST_F(MoveGeneratorCheckDetectionTest, checkDetection_fenQueenChecksFalseBlack_ShouldReturnFalse)
{
    io::setBoardFromFen(fenQueenChecksFalse, bitboards, gameStateBitmasks, squaresLookup);
    ASSERT_FALSE(moveGenerator.isInCheck(false));
}

TEST_F(MoveGeneratorCheckDetectionTest, checkDetection_startingPosWhite_ShouldReturnFalse)
{
    io::setBoardFromFen(startingPos, bitboards, gameStateBitmasks, squaresLookup);
    ASSERT_FALSE(moveGenerator.isInCheck(true));
}

TEST_F(MoveGeneratorCheckDetectionTest, checkDetection_startingPosBlack_ShouldReturnFalse)
{
    io::setBoardFromFen(startingPos, bitboards, gameStateBitmasks, squaresLookup);
    ASSERT_FALSE(moveGenerator.isInCheck(false));
}

} // namespace logic
