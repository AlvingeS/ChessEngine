#include "engine/perft/perft.h"
#include "tests/test_config.h"

#include "io/fen.h"
#include "io/stockfish_perft_retriever.h"

#include "logic/attack_tables/attack_tables.h"
#include "model/position/position.h"

#include <cstdint>
#include <vector>
#include <gtest/gtest.h>

namespace {
struct PerftDataRow {
    std::optional<long long> nodes         = std::nullopt;
    std::optional<long long> captures      = std::nullopt;
    std::optional<long long> ep_captures   = std::nullopt;
    std::optional<long long> castles       = std::nullopt;
    std::optional<long long> promos        = std::nullopt;
    std::optional<long long> checks        = std::nullopt;
    std::optional<long long> disc_checks   = std::nullopt;
    std::optional<long long> double_checks = std::nullopt;
    std::optional<long long> checkmates    = std::nullopt;
};

struct TestPosition {
    std::string name;
    std::string fen;
    std::vector<PerftDataRow> data;
    int standard_depth;
    int max_depth;

    TestPosition(std::string name, std::string fen, int standard_depth, int max_depth)
        : name(std::move(name))
        , fen(std::move(fen))
        , data(max_depth)
        , standard_depth(standard_depth)
        , max_depth(max_depth)
    {}
};

void PrintTo(const TestPosition& pos, std::ostream* os)
{
    *os << pos.name;
}

std::vector<TestPosition> build_perft_data()
{
    TestPosition start_pos{"start_pos", "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 5, 9};
    TestPosition pos2{"pos2", "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -", 4, 6};
    TestPosition pos3{"pos3", "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 6, 8};
    TestPosition pos4{"pos4", "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 4, 6};
    TestPosition pos5{"pos5", "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 4, 5};
    TestPosition pos6{"pos6", "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10", 4, 9};
    TestPosition pos_exp{"pos_exp", "4k3/7R/8/6B1/4P3/5B2/3P4/1K6 b - - 1 1", 4, 6};
    TestPosition castle_brings_you_into_check_from_opp_king{"castle_brings_you_into_check_from_opp_king", "8/7p/8/1pbp4/3N4/2P5/Pk3P1P/R3K1R1 w Q - 0 1", 4, 6};
    TestPosition double_pawn_push_check_ep_capture_blocked_by_pin{"double_pawn_push_check_ep_capture_blocked_by_pin", "8/K7/5p2/5k2/5p2/5Q2/5PPP/8 w - - 3 44", 4, 6};
    TestPosition block_check_with_promotion{"block_check_with_promotion", "8/8/8/8/8/2QK4/1p6/k7 w - - 36 81", 4, 6};
    TestPosition pawn_can_both_capture_and_block_to_stop_check{"pawn_can_both_capture_and_block_to_stop_check", "r3k3/5p1p/1R6/3b4/3B4/P2PK2n/5PrP/R7 b q - 2 26", 4, 6};
    TestPosition pinned_bish_cannot_capture_checker{"pinned_bish_cannot_capture_checker", "4r1k1/1p3p2/5p2/p6P/P4P2/3nB1P1/4K3/3r3b b - - 0 34", 4, 6};
    TestPosition dont_stop_check_by_jumping_over_king{"dont_stop_check_by_jumping_over_king", "6k1/5pb1/3q2pp/2pP4/8/5K1P/1P1Q1PP1/8 b - - 1 29", 4, 6};
    TestPosition stop_check_by_ep_capture{"stop_check_by_ep_capture", "6nr/p2Q3p/2rB1p2/1k1N2p1/Pp2P3/8/2P2PPP/R5K1 b - a3 0 25", 4, 6};

    // Nodes, captures, ep_captures, castles, promos, checks, disc_checks, double_checks, checkmates
    start_pos.data[0] = {20, 0, 0, 0, 0, 0, 0, 0, 0};
    start_pos.data[1] = {400, 0, 0, 0, 0, 0, 0, 0, 0};
    start_pos.data[2] = {8902, 34, 0, 0, 0, 12, 0, 0, 0};
    start_pos.data[3] = {197281, 1576, 0, 0, 0, 469, 0, 0, 8};
    start_pos.data[4] = {4865609, 82719, 258, 0, 0, 27351, 6, 0, 347};
    start_pos.data[5] = {119060324, 2812008, 5248, 0, 0, 809099, 329, 46, 10828};
    start_pos.data[6] = {3195901860LL, 108329926LL, 319617LL, 883453LL, 0LL, 33103848LL, 18026LL, 1628LL, 435767LL};
    start_pos.data[7] = {84998978956LL, 3523740106LL, 7187977LL, 23605205LL, 0LL, 968981593LL, 847039LL, 147215LL, 9852036LL};
    start_pos.data[8] = {2439530234167LL, 125208536153LL, 319496827LL, 1784356000LL, 17334376LL, 36095901903LL, 37101713,5547231LL, 400191963LL};

    pos2.data[0] = {48, 8, 0, 2, 0, 0, 0, 0, 0};
    pos2.data[1] = {2039, 351, 1, 91, 0, 3, 0, 0, 0};
    pos2.data[2] = {97862, 17102, 45, 3162, 0, 993, 0, 0, 1};
    pos2.data[3] = {4085603, 757163, 1929, 128013, 15172, 25523, 42, 6, 43};
    pos2.data[4] = {193690690, 35043416, 73365, 4993637, 8392, 3309887, 19883, 2637, 30171};
    pos2.data[5] = {8031647685LL, 1558445089LL, 3577504LL, 184513607LL, 56627920LL, 92238050LL, 568417LL, 54948LL, 360003LL};

    pos3.data[0] = {14, 1, 0, 0, 0, 2, 0, 0, 0};
    pos3.data[1] = {191, 14, 0, 0, 0, 10, 0, 0, 0};
    pos3.data[2] = {2812, 209, 2, 0, 0, 267, 3, 0, 0};
    pos3.data[3] = {43238, 3348, 123, 0, 0, 1680, 106, 0, 17};
    pos3.data[4] = {674624, 52051, 1165, 0, 0, 52950, 1292, 3, 0};
    pos3.data[5] = {11030083, 940350, 33325, 0, 7552, 452473, 26067, 0, 2733};
    pos3.data[6] = {178633661, 14519036, 294874, 0, 140024, 12797406, 370630, 3612, 87};
    pos3.data[7] = {3009794393LL, 267586558LL, 8009239LL, 0LL, 6578076LL, 135626805LL, 7181487LL, 1630LL, 450410LL};

    pos4.data[0] = {6, 0, 0, 0, 0, 0, std::nullopt, std::nullopt, 0};
    pos4.data[1] = {264, 87, 0, 6, 48, 10, std::nullopt, std::nullopt, 0};
    pos4.data[2] = {9467, 1021, 4, 0, 120, 38, std::nullopt, std::nullopt, 22};
    pos4.data[3] = {422333, 131393, 0, 7795, 60032, 15492, std::nullopt, std::nullopt, 5};
    pos4.data[4] = {15833292, 2046173, 6512, 0, 329464, 200568, std::nullopt, std::nullopt, 50562};
    pos4.data[5] = {706045033LL, 210369132LL, 212LL, 10882006LL, 81102984LL, 26973664LL, std::nullopt, std::nullopt, 81076LL};

    pos5.data[0] = {44};
    pos5.data[1] = {1486};
    pos5.data[2] = {62379};
    pos5.data[3] = {2103487};
    pos5.data[4] = {89941194};

    pos6.data[0] = {46};
    pos6.data[1] = {2079};
    pos6.data[2] = {89890};
    pos6.data[3] = {3894594};
    pos6.data[4] = {164075551};
    pos6.data[5] = {6923051137LL};
    pos6.data[6] = {287188994746LL};
    pos6.data[7] = {11923589843526LL};
    pos6.data[8] = {490154852788714LL};

    return {
        start_pos,
        pos2,
        pos3,
        pos4,
        pos5,
        pos6,
        pos_exp,
        castle_brings_you_into_check_from_opp_king,
        double_pawn_push_check_ep_capture_blocked_by_pin,
        block_check_with_promotion,
        pawn_can_both_capture_and_block_to_stop_check,
        pinned_bish_cannot_capture_checker,
        dont_stop_check_by_jumping_over_king,
        stop_check_by_ep_capture,
    };
}

} // namespace

class PerftTest : public ::testing::TestWithParam<TestPosition>
{

protected:
    engine::Perft perft{20};

    static void SetUpTestSuite()
    {
        logic::attack_tables::init_attack_tables();
    }
};

TEST_P(PerftTest, MatchesExpectedResults)
{
    const TestPosition& test_pos = GetParam();
    const int depth =  std::min(test_pos.standard_depth + perft_depth_offset, test_pos.max_depth);

    perft.set_pos_from_fen(test_pos.fen);
    model::Position pos_copy = perft.get_pos_copy();

    std::unordered_map<model::Move, uint64_t> stockfish_results = io::stockfish::get_perft_results(
        test_pos.fen,
        depth,
        perft.get_pos()
    );

    perft.set_max_depth(depth);
    perft.minimax(0, 0, verbose);

    std::unordered_map<model::Move, uint64_t> first_move_counts = perft.get_node_count_per_first_move_map();
    auto sf_results = io::stockfish::compare_first_move_counts_to_stockfish(first_move_counts, stockfish_results, pos_copy.is_w);

    // If there were any errors, print them and fail the test
    if (sf_results.first) {
        std::cerr << sf_results.second;
        EXPECT_TRUE(false);
    }

    for (int i = 0; i < depth; i++) {
        if (test_pos.data[i].nodes.has_value()) {
            EXPECT_EQ(perft.node_count_[i], test_pos.data[i].nodes);
        }

        if (test_pos.data[i].captures.has_value()) {
            EXPECT_EQ(perft.capture_count_[i], test_pos.data[i].captures);
        }

        if (test_pos.data[i].ep_captures.has_value()) {
            EXPECT_EQ(perft.ep_capture_count_[i], test_pos.data[i].ep_captures);
        }

        if (test_pos.data[i].castles.has_value()) {
            EXPECT_EQ(perft.casle_count_[i], test_pos.data[i].castles);
        }

        if (test_pos.data[i].promos.has_value()) {
            EXPECT_EQ(perft.promo_count_[i], test_pos.data[i].promos);
        }
        
        if (i < depth - 1 && test_pos.data[i].checks.has_value()) {
            EXPECT_EQ(perft.check_count_[i], test_pos.data[i].checks);
        }

        // TODO: Implement disc checks count and assert
        // TODO: Implement double checks count and assert

        if (i < depth - 1 && test_pos.data[i].checkmates.has_value()) {
            EXPECT_EQ(perft.checkmate_count_[i], test_pos.data[i].checkmates);
        }
    }
}

INSTANTIATE_TEST_SUITE_P(
    PerftSuite,
    PerftTest,
    ::testing::ValuesIn(build_perft_data()),
    [](const ::testing::TestParamInfo<TestPosition>& info)
    {
        return info.param.name;
    }
);