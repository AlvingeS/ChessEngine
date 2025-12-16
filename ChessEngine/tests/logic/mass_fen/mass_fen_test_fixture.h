#pragma once

#include "model/position/position.h"
#include "model/move/movelist.h"
#include "model/constants.h"

#include "logic/zobrist/z_hasher.h"
#include "logic/makemove/move_maker.h"
#include "logic/makemove/move_retractor.h"
#include "logic/movegen/move_gen.h"

#include "io/fen.h"

#include <gtest/gtest.h>

#include <fstream>
#include <string>
#include <iostream>

namespace logic {

class MassFenTestFixture : public ::testing::Test 
{

protected:
    MassFenTestFixture() 
        : pos(model::Position())
        , z_hasher(ZHasher(pos, 123))
        , z_hasher_control(ZHasher(pos, 123))
        , move_maker(MoveMaker(pos, z_hasher))
        , move_retractor(MoveRetractor(pos, z_hasher))
        , move_gen(MoveGen(pos, move_maker, move_retractor))
    {};

    model::Position pos;
    ZHasher z_hasher;
    ZHasher z_hasher_control;
    MoveMaker move_maker;
    MoveRetractor move_retractor;
    MoveGen move_gen;

    virtual void SetUp() override 
    {
        logic::attack_tables::init_attack_tables();
    }

    template <class F>
    void for_each_fen_position(const std::string& path, F&& fn) {
        std::ifstream f(path);
        if (!f) throw std::runtime_error("Could not open: " + path);

        std::string fen;
        while (std::getline(f, fen)) {
            if (fen.empty()) continue;

            io::fen::set_pos_from_fen(fen, pos);
            z_hasher.hash_from_position(pos);
            z_hasher_control.hash_from_position(pos);

            fn(fen);
        }
    }
};

} // namespace engine