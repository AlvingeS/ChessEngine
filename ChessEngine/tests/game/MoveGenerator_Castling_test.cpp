#include <gtest/gtest.h>
#include "ChessEngine/game/MoveGenerator.h"
#include <unordered_set>
#include "ChessEngine/game/Move.h"

namespace game {

    class MoveGeneratorCastlingTest : public ::testing::Test {
        protected:
            MoveGenerator moveGenerator;
            std::string startingPos;
            std::string fenOne;
            std::string fenTwo;
            std::string fenThree;
            std::string fenFour;

            void SetUp() override {
                moveGenerator = MoveGenerator();
                startingPos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
                fenOne = "r3k2r/8/8/8/8/8/8/R3K2R";
                fenTwo = "r1n1k1Nr/8/8/8/8/8/8/R2pK1PR";
                fenThree = "4k2r/r7/8/8/8/8/7R/R3K3";
            }

            void insertExpectedMoves(std::unordered_set<Move>& moves, int fromBitIndex, const std::vector<int>& toBitIndices, PieceType pieceType) {
                for (int toBitIndex : toBitIndices) {
                    moves.insert(Move(pieceType, fromBitIndex, toBitIndex)); // Replace PIECE_TYPE with actual type
                }
            }
    };

    TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_fenOne_ShouldReturn4Moves) {
        moveGenerator.setBoardFromFen(fenOne);
        moveGenerator.genCastlingMoves(true);
        moveGenerator.genCastlingMoves(false);

        castlingRights castlingRights = moveGenerator.getCastlingRights();    

        ASSERT_TRUE(castlingRights.whiteCanCastleKingSide);
        ASSERT_TRUE(castlingRights.whiteCanCastleQueenSide);
        ASSERT_TRUE(castlingRights.blackCanCastleKingSide);
        ASSERT_TRUE(castlingRights.blackCanCastleQueenSide);
    }

    TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_fenTwo_ShouldReturn0Moves) {
        moveGenerator.setBoardFromFen(fenTwo);
        moveGenerator.genCastlingMoves(true);
        moveGenerator.genCastlingMoves(false);

        castlingRights castlingRights = moveGenerator.getCastlingRights();    

        ASSERT_FALSE(castlingRights.whiteCanCastleKingSide);
        ASSERT_FALSE(castlingRights.whiteCanCastleQueenSide);
        ASSERT_FALSE(castlingRights.blackCanCastleKingSide);
        ASSERT_FALSE(castlingRights.blackCanCastleQueenSide);
    }

    TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_fenThree_ShouldReturn2Moves) {
        moveGenerator.setBoardFromFen(fenThree);
        moveGenerator.getBoard().setRookHMoved(true);
        moveGenerator.getBoard().setRookAMoved(false);
        moveGenerator.genCastlingMoves(true);
        moveGenerator.genCastlingMoves(false);

        castlingRights castlingRights = moveGenerator.getCastlingRights();    

        ASSERT_FALSE(castlingRights.whiteCanCastleKingSide);
        ASSERT_TRUE(castlingRights.whiteCanCastleQueenSide);
        ASSERT_TRUE(castlingRights.blackCanCastleKingSide);
        ASSERT_FALSE(castlingRights.blackCanCastleQueenSide);
    }

    TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_fenFour_ShouldReturn0Moves) {
        moveGenerator.setBoardFromFen(fenFour);
        moveGenerator.getBoard().setKingMoved(true);
        moveGenerator.getBoard().setKingMoved(false);
        moveGenerator.genCastlingMoves(true);
        moveGenerator.genCastlingMoves(false);

        castlingRights castlingRights = moveGenerator.getCastlingRights();    

        ASSERT_FALSE(castlingRights.whiteCanCastleKingSide);
        ASSERT_FALSE(castlingRights.whiteCanCastleQueenSide);
        ASSERT_FALSE(castlingRights.blackCanCastleKingSide);
        ASSERT_FALSE(castlingRights.blackCanCastleQueenSide);
    }

    TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_fenStartingPos_ShouldReturn0Moves) {
        moveGenerator.setBoardFromFen(startingPos);
        moveGenerator.genCastlingMoves(true);
        moveGenerator.genCastlingMoves(false);

        castlingRights castlingRights = moveGenerator.getCastlingRights();    

        ASSERT_FALSE(castlingRights.whiteCanCastleKingSide);
        ASSERT_FALSE(castlingRights.whiteCanCastleQueenSide);
        ASSERT_FALSE(castlingRights.blackCanCastleKingSide);
        ASSERT_FALSE(castlingRights.blackCanCastleQueenSide);
    }
}
