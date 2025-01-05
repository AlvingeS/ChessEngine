#pragma once

#include <gtest/gtest.h>

#include "ChessEngine/src/model/board/Board.h"
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
    model::StateBitmasks stateBitmasks;
    model::PieceMap pieceMap;
    engine::SearchMemory searchMemory;
    model::ZHasher zHasher;
    MoveMaker moveMaker;
    MoveRetractor moveRetractor;
    MoveGenerator moveGenerator;
    std::string startingPos;
    model::Movelist movelist;

    BaseGenerator() : bitboards(model::Bitboards()),
                        stateBitmasks(model::StateBitmasks(bitboards)),
                        pieceMap(model::PieceMap(bitboards)),
                        searchMemory(engine::SearchMemory(0)),
                        zHasher(model::ZHasher(pieceMap)),
                        moveMaker(bitboards, stateBitmasks, pieceMap, zHasher, searchMemory),
                        moveRetractor(bitboards, stateBitmasks, pieceMap, zHasher, searchMemory),
                        moveGenerator(MoveGenerator(bitboards, stateBitmasks, moveMaker, moveRetractor)),
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