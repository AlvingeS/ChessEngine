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
    model::Board board;
    model::Bitboards& bitboards;
    model::StateBitmasks& stateBitmasks;
    model::PieceMap& pieceMap;
    model::ZHasher& zHasher;
    engine::SearchMemory searchMemory;
    MoveMaker moveMaker;
    MoveRetractor moveRetractor;
    MoveGenerator moveGenerator;
    std::string startingPos;
    model::Movelist movelist;

    BaseGenerator() 
        : board(),
          bitboards(board.bitboards),
          stateBitmasks(board.stateBitmasks),
          pieceMap(board.pieceMap),
          zHasher(board.zHasher),
          searchMemory(engine::SearchMemory(0)),
          moveMaker(board, searchMemory),
          moveRetractor(board, searchMemory),
          moveGenerator(MoveGenerator(board, moveMaker, moveRetractor)),
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