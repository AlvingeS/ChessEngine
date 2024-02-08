#include <gtest/gtest.h>
#include "ChessEngine/game/MoveGenerator.h"
#include <unordered_set>
#include "ChessEngine/game/Move.h"

namespace game {

    class MoveGeneratorCountsTest : public ::testing::Test {
        protected:
            MoveGenerator moveGenerator;
            std::string startingPos;
            std::string fenOne;
            std::string fenTwo;

            void SetUp() override {
                moveGenerator = MoveGenerator();
                startingPos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
                fenOne = "8/1n1n4/5R2/2N5/4P3/8/5p2/7N";
            }

            void insertExpectedMoves(std::unordered_set<Move>& moves, int fromBitIndex, const std::vector<int>& toBitIndices, PieceType pieceType) {
                for (int toBitIndex : toBitIndices) {
                    moves.insert(Move(pieceType, fromBitIndex, toBitIndex)); // Replace PIECE_TYPE with actual type
                }
            }
    };

    TEST_F(MoveGeneratorCountsTest, genAllMoves_startPosWhite_ShouldReturn20Moves) {
        moveGenerator.setBoardFromFen(startingPos);
        moveGenerator.genMoves(true);

        std::vector<Move> moves = moveGenerator.getMoves();
        ASSERT_EQ(moveGenerator.getMoveIndex(), 20);
    }

    TEST_F(MoveGeneratorCountsTest, genAllMoves_startPosBlack_ShouldReturn20Moves) {
        moveGenerator.setBoardFromFen(startingPos);
        moveGenerator.genMoves(false);

        std::vector<Move> moves = moveGenerator.getMoves();
        ASSERT_EQ(moveGenerator.getMoveIndex(), 20);
    }
}
