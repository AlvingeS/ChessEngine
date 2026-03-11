#include "mass_fen_test_fixture.h"

#include "tests/test_config.h"
#include "io/stockfish_perft_retriever.h"

class LowDepthPerft : public MassFenTestFixture  
{};

TEST_F(LowDepthPerft, LowDepthPerft)
{
    for_each_fen_position(std::string(TEST_DATA_DIR) + mass_fen_perft_samples, [&](const std::string& fen){
        
        auto pm_copy = perft.get_piece_map_copy();
        auto is_w_copy = perft.get_is_w_copy();

        std::unordered_map<model::Move, uint64_t> stockfish_results = io::stockfish::get_perft_results(fen, mass_fen_perft_depth, pm_copy);

        perft.set_max_depth(mass_fen_perft_depth);
        perft.minimax(0, 0, true);
        
        std::unordered_map<model::Move, uint64_t> first_move_counts = perft.get_node_count_per_first_move_map();
        auto sf_results = io::stockfish::compare_first_move_counts_to_stockfish(first_move_counts, stockfish_results, is_w_copy);

        // If there were any errors, print them and fail the test
        if (sf_results.first) {
            std::cerr << sf_results.second;
            ASSERT_TRUE(false);
        }
        
    });
}