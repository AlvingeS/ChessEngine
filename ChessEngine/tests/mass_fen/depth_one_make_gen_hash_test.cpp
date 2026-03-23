#include "mass_fen_test_fixture.h"

#include "io/stockfish_perft_retriever.h"

class DepthOneMakeGenHashTest : public MassFenTestFixture  
{};

TEST_F(DepthOneMakeGenHashTest, CheckCorrectMoveGenMakeMoveAndHashingForMassFen)
{
    for_each_fen_position(std::string(TEST_DATA_DIR) + mass_fen_make_unmake_samples, [&](const std::string& fen){
        model::Movelist pseudo_legal_moves;
        move_gen.gen_moves(pseudo_legal_moves);
        model::Position pos_copy = pos;

        std::vector<model::Move> legal_moves;
        
        EXPECT_EQ(z_hasher.value(), z_hasher_control.value());

        std::ostringstream hash_errors;
        size_t num_hash_errors = 0;
        size_t num_correct_hash = 0;
        size_t num_total_moves = 0;

        for (size_t i = 0; i < constants::MAX_LEGAL_MOVES; i++) {
            auto move = pseudo_legal_moves.get_move_at(i);

            if (move.value() == 0) {
                break;
            }

            num_total_moves++;

            const bool mover_is_w = pos.is_w;

            int debug_move_val = 61572;
            bitmask debug_occ_mask_val = 360869686735015985;

            if (move.value() == debug_move_val && pos.bbs.get_occ() == debug_occ_mask_val) {
                std::cout << "Reached debug point!" << std::endl;
            }

            auto undo_info = move_maker.make_move(move);

            const uint64_t inc = z_hasher.value();
            z_hasher_control.hash_from_position(pos);
            const uint64_t full = z_hasher_control.value();

            if (inc != full) {
                num_hash_errors++;
            hash_errors << "Move: " << io::stockfish::move_to_str(move, mover_is_w)
                        << " cap=" << move.is_any_capture()
                        << " dpp=" << move.is_double_pawn_push()
                        << " ep=" << move.is_ep_capture()
                        << " promo=" << move.is_any_promo()
                        << " castle=" << move.is_any_castle()
                        << "\n";
            } else {
                num_correct_hash++;
            }

            if (!move_gen.in_check()) {
                legal_moves.push_back(move);
            }

            move_retractor.unmake_move(move, undo_info);
        }

        if (num_hash_errors > 0) {
            std::cerr << "FEN: " << fen << "\n"
                    << "hash errors: " << num_hash_errors << "/" << num_total_moves
                    << " (correct: " << num_correct_hash << ")\n"
                    << hash_errors.str();
            FAIL();
        }


        auto sf_legal_moves = io::stockfish::get_legal_moves_from_fen(fen, pos_copy);

        std::sort(legal_moves.begin(), legal_moves.end(),
            [](const model::Move& a, const model::Move& b) {
                return a.value() < b.value();
            });
        
        std::sort(sf_legal_moves.begin(), sf_legal_moves.end(),
            [](const model::Move& a, const model::Move& b) {
                return a.value() < b.value();
            });

        std::ostringstream errors;
        bool has_errors = false;

        EXPECT_EQ(legal_moves.size(), sf_legal_moves.size());

        size_t i = 0, j = 0;
        while (i < legal_moves.size() && j < sf_legal_moves.size()) {
            if (legal_moves[i].value() == sf_legal_moves[j].value()) {
                i++;
                j++;
            } else if (legal_moves[i].value() < sf_legal_moves[j].value()) {
                errors << "Move: " << io::stockfish::move_to_str(legal_moves[i], pos.is_w)
                       << " (" << legal_moves[i].value() << ") only found in generated moves, not sf. FEN: " << fen << "\n"
                       << "Occupancy mask: " << pos.bbs.get_occ() << "\n";
                i++;
                has_errors = true;
            } else {
                errors << "Move: " << io::stockfish::move_to_str(sf_legal_moves[j], pos.is_w)
                    << " (" << legal_moves[i].value() << ") only found in sf moves, not generated. FEN: " << fen << "\n"
                    << "Occupancy mask: " << pos.bbs.get_occ() << "\n";
                j++;
                has_errors = true;
            }
        }

        while (i < legal_moves.size()) {
            errors << "Move: " << io::stockfish::move_to_str(legal_moves[i], pos.is_w)
                   << " (" << legal_moves[i].value() << ") only found in generated moves, not sf. FEN: " << fen << "\n"
                   << "Occupancy mask: " << pos.bbs.get_occ() << "\n";
;
            i++;
            has_errors = true;
        }

        while (j < sf_legal_moves.size()) {
            errors << "Move: " << io::stockfish::move_to_str(sf_legal_moves[j], pos.is_w)
                   << " (" << legal_moves[i].value() << ") only found in sf moves, not generated. FEN: " << fen << "\n"
                   << "Occupancy mask: " << pos.bbs.get_occ() << "\n";
            j++;
            has_errors = true;
        }

        // If there were any errors, print them and fail the test
        if (has_errors) {
            std::cerr << errors.str();
            FAIL();
        }
    });
}