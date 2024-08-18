#pragma once

#include "ChessEngine/common.h"

#include <gtest/gtest.h>
#include <unordered_set>

#include "ChessEngine/board/Bitboards.h"
#include "ChessEngine/board/GameStateBitmasks.h"
#include "ChessEngine/board/SquaresLookup.h"
#include "ChessEngine/movegen/MoveGenerator.h"
#include "ChessEngine/movegen/Movelist.h"
#include "ChessEngine/move/Move.h"
#include "ChessEngine/perft/SearchMemory.h"
#include "ChessEngine/board/ZHasher.h"
#include "ChessEngine/utils/Fen.h"

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
                              zHasher(board::ZHasher()),
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