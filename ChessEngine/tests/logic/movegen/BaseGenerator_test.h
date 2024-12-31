#pragma once

#include <gtest/gtest.h>

#include "ChessEngine/src/model/board/Bitboards.h"
#include "ChessEngine/src/model/board/GameStateBitmasks.h"
#include "ChessEngine/src/model/board/SquaresLookup.h"
#include "ChessEngine/src/model/board/ZHasher.h"
#include "ChessEngine/src/model/move/Move.h"
#include "ChessEngine/src/model/move/Movelist.h"
#include "ChessEngine/src/logic/movegen/MoveGenerator.h"

#include "ChessEngine/src/engine/search/SearchMemory.h"
#include "ChessEngine/src/io/Fen.h"


#include <unordered_set>

namespace logic {

class BaseGenerator : public ::testing::Test 
{
protected:
    model::Bitboards bitboards;
    model::GameStateBitmasks gameStateBitmasks;
    model::SquaresLookup squaresLookup;
    engine::SearchMemory searchMemory;
    model::ZHasher zHasher;
    MoveMaker moveMaker;
    MoveRetractor moveRetractor;
    MoveGenerator moveGenerator;
    std::string startingPos;
    model::Movelist movelist;

    BaseGenerator() : bitboards(model::Bitboards()),
                        gameStateBitmasks(model::GameStateBitmasks(bitboards)),
                        squaresLookup(model::SquaresLookup(bitboards)),
                        searchMemory(engine::SearchMemory(0)),
                        zHasher(model::ZHasher(squaresLookup)),
                        moveMaker(bitboards, gameStateBitmasks, squaresLookup, zHasher, searchMemory),
                        moveRetractor(bitboards, gameStateBitmasks, squaresLookup, zHasher, searchMemory),
                        moveGenerator(MoveGenerator(bitboards, gameStateBitmasks, moveMaker, moveRetractor)),
                        movelist(model::Movelist()) {}

    virtual void SetUp() override {
        // board = model::ChessBoard();
        // moveGenerator = MoveGenerator(board);
        startingPos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";;
    }

    virtual void TearDown() override {
        bitboards.resetBitboards();
    }

    void insertExpectedMoves(std::unordered_set<model::Move>& moves, int fromBitIndex, const std::vector<int>& toBitIndices, const std::vector<int>& flags) {
        for (size_t i = 0; i < toBitIndices.size(); i++) {
            moves.insert(model::Move(fromBitIndex, toBitIndices[i], flags[i]));
        }
    }

    model::Movelist getMoves() {
        return movelist;
    }
};

} // namespace logic