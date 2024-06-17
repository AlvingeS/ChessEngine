#pragma once
#include "ChessEngine/movegen/MoveGenerator.h"
#include "ChessEngine/game/Move.h"
#include "ChessEngine/search/SearchMemory.h"
#include "ChessEngine/game/ZHasher.h"
#include <gtest/gtest.h>
#include <unordered_set>
#include <vector>

namespace movegen {
    class BaseGenerator : public ::testing::Test {
        protected:
            game::ChessBoard board;
            search::SearchMemory searchMemory;
            game::ZHasher zHasher;
            game::MoveMaker moveMaker;
            MoveGenerator moveGenerator;
            std::string startingPos;
            std::vector<game::Move> moveList;

            BaseGenerator() : board(game::ChessBoard()),
                              searchMemory(search::SearchMemory(0)),
                              zHasher(game::ZHasher()),
                              moveMaker(board, searchMemory, zHasher),
                              moveGenerator(MoveGenerator(board, moveMaker)) {}

            virtual void SetUp() override {
                // board = game::ChessBoard();
                // moveGenerator = MoveGenerator(board);
                startingPos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
                moveList = std::vector<game::Move>(movegen::MoveGenerator::MAX_LEGAL_MOVES);
            }

            virtual void TearDown() override {
                board.resetBitboards();
            }

        void insertExpectedMoves(std::unordered_set<game::Move>& moves, int fromBitIndex, const std::vector<int>& toBitIndices, const std::vector<int>& flags) {
            for (size_t i = 0; i < toBitIndices.size(); i++) {
                moves.insert(game::Move(fromBitIndex, toBitIndices[i], flags[i]));
            }
        }

        std::vector<game::Move> getMoves() {
            return moveList;
        }
    };
}