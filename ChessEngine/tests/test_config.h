#pragma once

#include <string>

static constexpr bool big_tests = true;

// Perft
static constexpr bool verbose = false;
static constexpr int perft_depth_offset = big_tests ? 2 : 1;

// Mass fen low-depth perft
static constexpr int mass_fen_perft_depth = big_tests ? 4 : 3;
static constexpr const char* mass_fen_perft_samples =
    big_tests ? "/sampled_1000_A.csv" : "/sampled_150_A.csv";

// Mass fen make/unmake move hash test
static constexpr const char* mass_fen_make_unmake_samples =
    big_tests ? "/sampled_1000_B.csv" : "/sampled_150_B.csv";