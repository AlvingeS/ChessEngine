#pragma once

#include <gtest/gtest.h>

#include "ChessEngine/src/game/board/Bitboards.h"
#include "ChessEngine/src/game/board/GameStateBitmasks.h"
#include "ChessEngine/src/game/board/SquaresLookup.h"
#include "ChessEngine/src/game/board/ZHasher.h"
#include "ChessEngine/src/game/movegen/MoveGenerator.h"
#include "ChessEngine/src/game/movegen/Movelist.h"
#include "ChessEngine/src/game/move/Move.h"

#include "ChessEngine/src/ponder/perft/SearchMemory.h"
#include "ChessEngine/src/utils/Fen.h"

#include <unordered_set>

namespace movegen {
    class BaseGenerator : public ::testing::Test {
        protected:
            board::Bitboards bitboards;
            board::GameStateBitmasks gameStateBitmasks;
            board::SquaresLookup squaresLookup;
            perft::SearchMemory searchMemory;
            board::ZHasher zHasher;
            move::MoveMaker moveMaker;
            move::MoveRetractor moveRetractor;
            MoveGenerator moveGenerator;
            std::string startingPos;
            Movelist movelist;

            BaseGenerator() : bitboards(board::Bitboards()),
                              gameStateBitmasks(board::GameStateBitmasks(bitboards)),
                              squaresLookup(board::SquaresLookup(bitboards)),
                              searchMemory(perft::SearchMemory(0)),
                              zHasher(board::ZHasher(squaresLookup)),
                              moveMaker(bitboards, gameStateBitmasks, squaresLookup, searchMemory, zHasher),
                              moveRetractor(bitboards, gameStateBitmasks, squaresLookup, searchMemory, zHasher),
                              moveGenerator(MoveGenerator(bitboards, gameStateBitmasks, moveMaker, moveRetractor)),
                              movelist(Movelist()) {}

            virtual void SetUp() override {
                // board = board::ChessBoard();
                // moveGenerator = MoveGenerator(board);
                startingPos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";;
            }

            virtual void TearDown() override {
                bitboards.resetBitboards();
            }

        void insertExpectedMoves(std::unordered_set<move::Move>& moves, int fromBitIndex, const std::vector<int>& toBitIndices, const std::vector<int>& flags) {
            for (size_t i = 0; i < toBitIndices.size(); i++) {
                moves.insert(move::Move(fromBitIndex, toBitIndices[i], flags[i]));
            }
        }

        Movelist getMoves() {
            return movelist;
        }
    };
}