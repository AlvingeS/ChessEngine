#pragma once

#include "ChessEngine/common.h"

#include <gtest/gtest.h>
#include <unordered_set>

#include "ChessEngine/game/BitBoards.h"
#include "ChessEngine/game/GameStateBitMasks.h"
#include "ChessEngine/game/SquaresLookup.h"
#include "ChessEngine/movegen/MoveGenerator.h"
#include "ChessEngine/game/Move.h"
#include "ChessEngine/perft/SearchMemory.h"
#include "ChessEngine/game/ZHasher.h"
#include "ChessEngine/utils/Fen.h"

namespace movegen {
    class BaseGenerator : public ::testing::Test {
        protected:
            game::BitBoards bitboards;
            game::GameStateBitMasks gameStateBitmasks;
            game::SquaresLookup squaresLookup;
            perft::SearchMemory searchMemory;
            game::ZHasher zHasher;
            game::MoveMaker moveMaker;
            MoveGenerator moveGenerator;
            std::string startingPos;
            std::vector<game::Move> moveList;

            BaseGenerator() : bitboards(game::BitBoards()),
                              gameStateBitmasks(game::GameStateBitMasks(bitboards)),
                              squaresLookup(game::SquaresLookup(bitboards)),
                              searchMemory(perft::SearchMemory(0)),
                              zHasher(game::ZHasher()),
                              moveMaker(bitboards, squaresLookup, gameStateBitmasks, searchMemory, zHasher),
                              moveGenerator(MoveGenerator(bitboards, gameStateBitmasks, moveMaker)) {}

            virtual void SetUp() override {
                // board = game::ChessBoard();
                // moveGenerator = MoveGenerator(board);
                startingPos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
                moveList = std::vector<game::Move>(movegen::MoveGenerator::MAX_LEGAL_MOVES);
            }

            virtual void TearDown() override {
                bitboards.resetBitboards();
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