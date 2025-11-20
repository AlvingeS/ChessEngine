#include "perft_base_test.h"

#include "io/stockfish_perft_retriever.h"

namespace engine {

class PerftPosTwo : public PerftBase 
{
protected:
    void SetUp() override {
        PerftBase::SetUp();
    }

    // Nodes, captures, epCaptures, castling, promotion, check, checkmate
    std::unordered_map<int, std::vector<uint64_t>> expected_results {
        {0, {1,         0,          0,          0,          0,          0,          0}},
        {1, {48,        8,          0,          2,          0,          0,          0}},
        {2, {2039,      351,        1,          91,         0,          3,          0}},
        {3, {97862,     17102,      45,         3162,       0,          993,        1}},
        {4, {4085603,   757163,     1929,       128013,     15172,      25523,      43}},
        {5, {193690690, 35043416,   73365,      4993637,    8392,       3309887,    30171}},
        {6, {0,         0,          0,          0,          0,          0,          0}}
    };
};

TEST_F(PerftPosTwo, perft_pos2) 
{
    if (enable_pos_2_test) {
        move_picker.set_board_from_fen(pos_two);

        // Make dubug move
        // model::Move move = move_from_str_and_flag("a2a3", 0);
        // move_picker.make_move(move, true);
        // move = move_from_str_and_flag("c7c6", 0);
        // move_picker.make_move(model::Move(0, 0, model::Move::QUEEN_CASTLE_FLAG), false);

        int num_debug_moves = 0;

        int depth = long_runs ? posTwoMaxDepth + 1 : posTwoMaxDepth;
        depth -= num_debug_moves;
        bool w_to_start = num_debug_moves % 2 == 0;

        std::string debug_fen;
        if (num_debug_moves > 0) {
            debug_fen = move_picker.get_fen_from_board();
            debug_fen += w_to_start ? " w" : " b";
            debug_fen += " KQkq -";
        }

        std::unordered_map<std::string, uint64_t> stockfish_results = io::stockfish::get_perft_results(num_debug_moves > 0 ? debug_fen : pos_two, depth);

        move_picker.set_max_depth(depth);
        move_picker.minimax(0, w_to_start, 0);
        
        std::unordered_map<std::string, uint64_t> first_move_counts = node_count_per_first_move_as_map(w_to_start);
        compare_first_move_counts_to_stockfish(first_move_counts, stockfish_results);
        
        if (num_debug_moves == 0) {
            for (int i = 1; i <= move_picker.get_max_depth(); i++) {
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