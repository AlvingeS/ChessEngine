#pragma once

#include <gtest/gtest.h>

#include "model/position/position.h"
#include "model/move/move.h"
#include "model/move/movelist.h"
#include "logic/movegen/move_gen.h"

#include "logic/makemove/move_maker.h"
#include "logic/makemove/move_retractor.h"

#include "io/fen.h"


#include <unordered_set>

namespace logic {

class BaseGenerator : public ::testing::Test 
{
protected:
    model::Position pos;
    MoveMaker move_maker;
    MoveRetractor move_retractor;
    MoveGen moveGenerator;
    std::string startingPos;
    model::Movelist movelist;

    BaseGenerator()
          : pos(model::Position())
          , move_maker(pos)
          , move_retractor(pos)
          , moveGenerator(MoveGen(pos, move_maker, move_retractor))
          , movelist(model::Movelist()) 
    {}

    virtual void SetUp() override {
        // board = model::ChessBoard();
        // moveGenerator = MoveGen(board);
        attack_tables::init_attack_tables();
        startingPos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";;
    }

    virtual void TearDown() override {
        pos.bbs.reset_bitboards();
    }

    void insertExpectedMoves(std::unordered_set<model::Move>& moves, sq_idx from_sq, const std::vector<sq_idx>& to_sqs, const std::vector<int>& flags) {
        for (size_t i = 0; i < to_sqs.size(); i++) {
            moves.insert(model::Move(from_sq, to_sqs[i], flags[i]));
        }
    }

    model::Movelist getMoves() {
        return movelist;
    }
};

} // namespace logic