#pragma once

#include "engine/pickmove/move_picker.h"

#include "io/fen.h"

#include "logic/attack_tables/attack_tables.h"
#include "model/position/position.h"

#include <cstdint>
#include <gtest/gtest.h>
#include <unordered_map>

namespace engine {

class PerftBase : public ::testing::Test 
{

struct from_to_bit_idxs {
    int from;
    int to;
};

protected:
    bool long_runs = true;
    bool mega_runs = false;

    // Long run should be depth 6, mega run should be depth 7
    bool enable_start_pos_test = true;
    int start_pos_max_depth = 5;

    // Long run should be depth 5
    bool enable_pos_2_test = true;
    int posTwomax_depth = 4;

    // Long run should be depth 7
    bool enable_pos_3_test = true;
    int pos_three_max_depth = 6;

    // Long run should be depth 5
    bool enable_pos_5_test = true;
    int pos_five_max_depth = 4;

    std::string startPos;
    std::string pos_two;
    std::string pos_three;
    std::string posFive;

    MovePicker move_picker;

    PerftBase() : move_picker(20) {}

    virtual void SetUp() override 
    {
        logic::attack_tables::init_attack_tables();
        startPos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
        pos_two = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 0";
        pos_three = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -";
        posFive = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8";
    }

    char col_to_char(int col) {
        switch (col) {
            case 0:
                return 'h';
            case 1:
                return 'g';
            case 2:
                return 'f';
            case 3:
                return 'e';
            case 4:
                return 'd';
            case 5:
                return 'c';
            case 6:
                return 'b';
            case 7:
                return 'a';
            default:
                return 'x';
        }
    }

    std::string move_to_str(model::Move move, bool w_started) 
    {
        if (move.is_any_castle()) {
            return move.is_king_castle() ? (w_started ? "e1g1" : "e8g8") 
                                        : (w_started ? "e1c1" : "e8c8");
        }
        
        int from = move.get_from_sq();
        int to = move.get_to_sq();
        
        int from_row = from / 8;
        int from_col = from % 8;

        char from_col_char = col_to_char(from_col);

        int to_row = to / 8;
        int to_col = to % 8;
        char to_col_char = col_to_char(to_col);

        std::string move_str = "";
        move_str += from_col_char;
        move_str += std::to_string(from_row + 1);
        move_str += to_col_char;
        move_str += std::to_string(to_row + 1);

        if (move.is_any_promo()) {
            switch (move.get_flag()) {
                case model::Move::KNIGHT_PROMO_FLAG:
                    move_str += (w_started) ? "n" : "N";
                    break;
                case model::Move::BISHOP_PROMO_FLAG:
                    move_str += (w_started) ? "b" : "B";
                    break;
                case model::Move::ROOK_PROMO_FLAG:
                    move_str += (w_started) ? "r" : "R";
                    break;
                case model::Move::QUEEN_PROMO_FLAG:
                    move_str += (w_started) ? "q" : "Q";
                    break;
                default:
                    break;
                case model::Move::KNIGHT_PROMO_CAPTURE_FLAG:
                    move_str += (w_started) ? "n" : "N";
                    break;
                case model::Move::BISHOP_PROMO_CAPTURE_FLAG:
                    move_str += (w_started) ? "b" : "B";
                    break;
                case model::Move::ROOK_PROMO_CAPTURE_FLAG:
                    move_str += (w_started) ? "r" : "R";
                    break;
                case model::Move::QUEEN_PROMO_CAPTURE_FLAG:
                    move_str += (w_started) ? "q" : "Q";
                    break;
            }
        }

        return move_str;
    }

    std::unordered_map<model::Move, uint64_t> node_count_per_first_move_as_map() 
    {
        std::unordered_map<model::Move, uint64_t> node_count_per_first_move_map{};

        for (size_t i = 0; i < move_picker.node_count_per_first_move_.size(); i++) {
            model::Move move = move_picker.first_moves_[i];
            
            if (move.value() != 0)
                node_count_per_first_move_map[move] = move_picker.node_count_per_first_move_[i];
        }

        return node_count_per_first_move_map;
    }

    void compare_first_move_counts_to_stockfish(
        const std::unordered_map<model::Move, uint64_t> first_move_counts,
        const std::unordered_map<model::Move, uint64_t> stockfish_results,
        bool w_started)
    {
        std::ostringstream errors;
        bool has_errors = false;

        for (const auto& move_count_pair : first_move_counts) {
            const model::Move& move = move_count_pair.first;
            uint64_t count = move_count_pair.second;

            auto fount_it = stockfish_results.find(move);
            if (fount_it == stockfish_results.end()) {
                errors << "Move: " << move_to_str(move, w_started) << " not found in stockfish results.\n";
                has_errors = true;
            } else {
                uint64_t stockfishCount = fount_it->second;
                if (count != stockfishCount) {
                    errors << "Move: " << move_to_str(move, w_started) << " failed. Expected: " << stockfishCount << ", Got: " << count << ".\n";
                    has_errors = true;
                }
            }
        }

        // If there were any errors, print them and fail the test
        if (has_errors) {
            std::cerr << errors.str();
            ASSERT_TRUE(false);
        }
    }
};

} // namespace engine