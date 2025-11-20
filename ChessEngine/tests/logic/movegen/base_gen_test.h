#pragma once

#include <gtest/gtest.h>

#include "model/position/board.h"
#include "model/move/move.h"
#include "model/move/movelist.h"
#include "logic/movegen/move_gen.h"

#include "logic/makemove/move_maker.h"
#include "logic/makemove/move_retractor.h"

#include "engine/pickmove/search_memory.h"
#include "io/fen.h"


#include <unordered_set>

namespace logic {

class BaseGenerator : public ::testing::Test 
{
protected:
    model::Board board;
    model::Bitboards& bitboards;
    model::OccupancyMasks& occupancy_masks;
    model::PieceMap& piece_map;
    model::ZHasher& z_hasher;
    engine::SearchMemory searchMemory;
    MoveMaker move_maker;
    MoveRetractor move_retractor;
    MoveGenerator moveGenerator;
    std::string startingPos;
    model::Movelist movelist;

    BaseGenerator() 
        : board(),
          bitboards(board.bitboards),
          occupancy_masks(board.occupancy_masks),
          piece_map(board.piece_map),
          z_hasher(board.z_hasher),
          searchMemory(engine::SearchMemory(0)),
          move_maker(board),
          move_retractor(board),
          moveGenerator(MoveGenerator(board, move_maker, move_retractor)),
          movelist(model::Movelist()) {}

    virtual void SetUp() override {
        // board = model::ChessBoard();
        // moveGenerator = MoveGenerator(board);
        attack_tables::init_attack_tables();
        startingPos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";;
    }

    virtual void TearDown() override {
        bitboards.reset_bitboards();
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