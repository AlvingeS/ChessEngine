#include "perft_base_test.h"

#include "io/stockfish_perft_retriever.h"

namespace engine {

class PerftStartPos : public PerftBase
{
protected:
    void SetUp() override {
        PerftBase::SetUp();
    }
};


TEST_F(PerftStartPos, perft_starting_pos) 
{
    if (enable_start_pos_test) {
        int depth = start_pos_max_depth;

        auto pm_copy = move_picker.get_piece_map_copy();
        auto is_w_copy = move_picker.get_is_w_copy();

        std::unordered_map<model::Move, uint64_t> stockfish_results = io::stockfish::get_perft_results(startPos, depth, pm_copy);

        move_picker.set_max_depth(depth);

        move_picker.minimax(0, 0, true);
        std::unordered_map<model::Move, uint64_t> first_move_counts = move_picker.get_node_count_per_first_move_map();
        compare_first_move_counts_to_stockfish(first_move_counts, stockfish_results, is_w_copy);

        std::unordered_map<int, std::vector<uint64_t>> expected_results {
            {0, {1,          0,         0,      0,      0, 0,        0    }},
            {1, {20,         0,         0,      0,      0, 0,        0    }},
            {2, {400,        0,         0,      0,      0, 0,        0    }},
            {3, {8902,       34,        0,      0,      0, 12,       0    }},
            {4, {197281,     1576,      0,      0,      0, 469,      8    }},
            {5, {4865609,    82719,     258,    0,      0, 27351,    347  }},
            {6, {119060324,  2812008,   5248,   0,      0, 809099,   10828}},
            {7, {3195901860, 108329926, 319617, 883453, 0, 33103848, 0    }}
        };

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

} // namespace engine
