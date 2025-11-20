#include "perft_base_test.h"

#include "io/stockfish_perft_retriever.h"
namespace engine {

class perftPosFive : public PerftBase 
{
protected:
    void SetUp() override {
        PerftBase::SetUp();
    }

    // Nodes
    std::unordered_map<int, int> expected_nodes = {
        {0, 1},
        {1, 44},
        {2, 1486},
        {3, 62379},
        {4, 2103487},
        {5, 89941194}
    };
};

TEST_F(perftPosFive, perft_pos5) 
{
    if (enable_pos_5_test) {
        move_picker.set_board_from_fen(posFive);

        // Make dubug move
        // model::Move move = move_from_str_and_flag("a2a3", 0);
        // move_picker.make_move(move, true);
        int num_debug_moves = 0;

        int depth = long_runs ? pos_five_max_depth + 1 : pos_five_max_depth;
        depth -= num_debug_moves;
        bool w_to_start = num_debug_moves % 2 == 0;

        std::string debug_fen;
        if (num_debug_moves > 0) {
            debug_fen = move_picker.get_fen_from_board();
            debug_fen += w_to_start ? " w" : " b";
            debug_fen += " KQ - 1 8";
        }

        std::unordered_map<std::string, uint64_t> stockfish_results = io::stockfish::get_perft_results(num_debug_moves > 0 ? debug_fen : posFive, depth);

        move_picker.set_max_depth(depth);
        move_picker.minimax(0, w_to_start, 0);

        std::unordered_map<std::string, uint64_t> first_move_counts = node_count_per_first_move_as_map(w_to_start);
        compare_first_move_counts_to_stockfish(first_move_counts, stockfish_results);

        if (num_debug_moves == 0) {
            for (int i = 1; i <= move_picker.get_max_depth(); i++) {
                ASSERT_EQ(move_picker.node_count_[i], expected_nodes[i]);
            }
        }
    }
}

} // namespace engine