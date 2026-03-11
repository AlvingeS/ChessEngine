#pragma once

#include "model/position/position.h"
#include "model/move/movelist.h"
#include "model/constants.h"

#include "logic/zobrist/z_hasher.h"
#include "logic/makemove/move_maker.h"
#include "logic/makemove/move_retractor.h"
#include "logic/movegen/move_gen.h"

#include "engine/perft/perft.h"

#include "tests/test_config.h"

#include "io/fen.h"

#include <gtest/gtest.h>

#include <fstream>
#include <string>
#include <iostream>

class MassFenTestFixture : public ::testing::Test 
{

protected:
    MassFenTestFixture() 
        : pos(model::Position())
        , z_hasher(logic::ZHasher(pos, 123))
        , z_hasher_control(logic::ZHasher(pos, 123))
        , move_maker(logic::MoveMaker(pos, z_hasher))
        , move_retractor(logic::MoveRetractor(pos, z_hasher))
        , move_gen(logic::MoveGen(pos, move_maker, move_retractor))
        , perft(engine::Perft(mass_fen_perft_depth))
    {};

    model::Position pos;
    logic::ZHasher z_hasher;
    logic::ZHasher z_hasher_control;
    logic::MoveMaker move_maker;
    logic::MoveRetractor move_retractor;
    logic::MoveGen move_gen;
    engine::Perft perft;

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

            perft.set_max_depth(mass_fen_perft_depth);
            perft.reset_stats();
            perft.reset_stacks();
            io::fen::set_pos_from_fen(fen, perft.get_pos());
            perft.z_hash_from_position();

            fn(fen);
        }
    }
};