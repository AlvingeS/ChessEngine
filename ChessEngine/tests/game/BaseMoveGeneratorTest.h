#pragma once
#include "ChessEngine/game/MoveGenerator.h"
#include "ChessEngine/game/Move.h"
#include <gtest/gtest.h>
#include <unordered_set>
#include <vector>

namespace game {

    class BaseMoveGeneratorTest : public ::testing::Test {
        protected:
            ChessBoard board;
            MoveGenerator moveGenerator;
            std::string startingPos;
            std::vector<Move> moveList;

            BaseMoveGeneratorTest() : board(ChessBoard()), moveGenerator(MoveGenerator(board)) {}

            virtual void SetUp() override {
                board = ChessBoard();
                startingPos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
                moveList = std::vector<Move>(game::MoveGenerator::MAX_LEGAL_MOVES);
            }

        void insertExpectedMoves(std::unordered_set<Move>& moves, int fromBitIndex, const std::vector<int>& toBitIndices, const std::vector<int>& flags) {
            for (size_t i = 0; i < toBitIndices.size(); i++) {
                moves.insert(Move(fromBitIndex, toBitIndices[i], flags[i]));
            }
        }

        std::vector<Move> getMoves() {
            return moveList;
        }
    };
}