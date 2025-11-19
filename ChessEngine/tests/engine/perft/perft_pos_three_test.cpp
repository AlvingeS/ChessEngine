#include "perft_base_test.h"

#include "io/stockfish_perft_retriever.h"

namespace engine {

class PerftPosThree : public PerftBase 
{
protected:
    void SetUp() override {
        PerftBase::SetUp();
    }

    // Nodes, captures, epCaptures, castling, promotion, check, checkmate
    std::unordered_map<int, std::vector<long>> expected_results {
        {0, {1,         0,          0,          0,      0,          0,          0}},
        {1, {14,        1,          0,          0,      0,          2,          0}},
        {2, {191,       14,         0,          0,      0,          10,         0}},
        {3, {2812,      209,        2,          0,      0,          267,        0}},
        {4, {43238,     3348,       123,        0,      0,          1680,       17}},
        {5, {674624,    52051,      1165,       0,      0,          52950,      0}},
        {6, {11030083,  940350,     33325,      0,      7552,       452473,     2733}},
        {7, {178633661, 14519036,   294874,     0,      140024,     12797406,   87}}
    };
};

TEST_F(PerftPosThree, perft_pos3) 
{
    if (enable_pos_3_test) {
        move_picker.set_board_from_fen(pos_three);

        // model::Move move = move_from_str_and_flag("a5a4", 0);
        // move_picker.make_move(move, true);
        // move = move_from_str_and_flag("h4g4", 0);
        // move_picker.make_move(move, false);
        // move = move_from_str_and_flag("a4b3", 0);
        // move_picker.make_move(move, true);
        // move = move_from_str_and_flag("g4f5", 0);
        // move_picker.make_move(move, false);
        // move = move_from_str_and_flag("b3c4", 0);
        // move_picker.make_move(move, true);
        // move = move_from_str_and_flag("f5e4", 0);
        // move_picker.make_move(move, false);

        int num_debug_moves = 0;

        int depth = long_runs ? pos_three_max_depth + 1 : pos_three_max_depth;
        depth -= num_debug_moves;
        bool white_to_start = num_debug_moves % 2 == 0;

        std::string debug_fen;
        if (num_debug_moves > 0) {
            debug_fen = move_picker.get_fen_from_board();
            debug_fen += white_to_start ? " w" : " b";
            debug_fen += " - -";
        }

        std::unordered_map<std::string, uint64_t> stockfish_results = io::stockfish::get_perft_results(num_debug_moves > 0 ? debug_fen : pos_three, depth);

        move_picker.set_max_depth(depth);
        move_picker.minimax(0, white_to_start, 0);

        std::unordered_map<std::string, uint64_t> first_move_counts = node_count_per_first_move_as_map(white_to_start);
        compare_first_move_counts_to_stockfish(first_move_counts, stockfish_results);

        if (num_debug_moves == 0) {
            for (long i = 1; i <= move_picker.get_max_depth(); i++) {
                ASSERT_EQ(move_picker.node_count_[i], expected_results[i][0]);
                ASSERT_EQ(move_picker.capture_count_[i], expected_results[i][1]);
                ASSERT_EQ(move_picker.ep_capture_count_[i], expected_results[i][2]);
                ASSERT_EQ(move_picker.casle_count_[i], expected_results[i][3]);
                ASSERT_EQ(move_picker.promo_count_[i], expected_results[i][4]);
                ASSERT_EQ(move_picker.check_count_[i], expected_results[i][5]);
                
                if (i < move_picker.get_max_depth()) {
                    ASSERT_EQ(move_picker.checkmate_count_[i], expected_results[i][6]);
                }       
            }
        }
    }
}

} // namespace engine