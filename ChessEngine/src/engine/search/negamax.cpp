#include "engine/search/searcher.h"

#include "engine/search/mvv_lva_table.h"

namespace engine {

eval_t Searcher::negamax(int depth, eval_t alpha, eval_t beta, int ply, bool allow_null_move, const TimeManager& tm) {
    node_count_++;
    if (node_count_ % NODE_CHECK_INTERVAL == 0) {
        if (tm.time_is_up()) {
            stop_ = true;
            return 0;
        }
    }

    if (stop_) return 0;

    // Try to lookup the move from TT
    auto tt_entry = tt_.lookup(z_hasher_.value()); 
    bool tt_miss = tt_entry == nullptr;
    if (!tt_miss && tt_entry->depth >= depth) {
        switch (tt_entry->flag) {
            case TTFlag::EXACT:
                tt_hits_++;
                return tt_entry->score;
            case TTFlag::LOWER_BOUND:
                alpha = std::max(alpha, tt_entry->score);
                break;
            case TTFlag::UPPER_BOUND:
                beta = std::min(beta, tt_entry->score);
                break;
            case TTFlag::NONE:
                break;
        }
        if (alpha >= beta) {
            tt_hits_++;
            return alpha;
        }
    }

    // Start quescence search if at depth 0
    if (depth == 0) {
        return quiescence(alpha, beta, ply, tm);
    }

    // Null move pruning
    if (!move_generator_.in_check() && depth >= NULL_MOVE_DEPTH_REDUCTION + 1 && allow_null_move && has_non_pawn_material()) {
        bitmask ep_target_mask = pos_.ep_target_mask;
        make_null_move();
        eval_t score = -negamax(depth - 1 - NULL_MOVE_DEPTH_REDUCTION, -beta, -beta + 1, ply + 1, false, tm);
        undo_null_move(ep_target_mask);

        if (score >= beta) return beta;
    }

    // Save for game history comparison
    eval_t original_alpha = alpha;

    // Generate moves
    auto legality_info = move_generator_.gen_moves(move_lists_[ply], false);

    // If we found an entry in TT that was not EXACT, we can use the previously found best move
    // we swap it into the first slot
    if (!tt_miss) {
        int swap_idx = 0;
        for (int i = 0; i < move_lists_[ply].get_move_idx(); i++) {
            if (move_lists_[ply].get_move_at(i).value() == tt_entry->best_move_val) {
                swap_idx = i;
                break;
            }
        }

        move_lists_[ply].swap(0, swap_idx);
    }

    // For tt entry
    model::Move best_move = model::Move();
    eval_t best_score = -BIG_NUMBER;

    bool all_captures_played = false;
    int killer_move_idx = 0;

    // Loop over all moves
    for (int i = 0; i < constants::MAX_LEGAL_MOVES; i++) {
        model::Move move = move_lists_[ply].get_move_at(i);

        if (move.value() == 0) {
            // If i == 0 then there are no legal moves
            if (i == 0) {
                if (legality_info.in_check()) {
                    tt_.store(z_hasher_.value(), -BIG_NUMBER + ply, depth, TTFlag::EXACT, 0);
                    return -BIG_NUMBER + ply;
                } else {
                    tt_.store(z_hasher_.value(), 0, depth, TTFlag::EXACT, 0);
                    return 0;
                }
            }
            break;
        }

        // Find the highest capture value capture and swap it in
        if (i > 0 && !all_captures_played) {
            int mvv_lva_max_idx = i;
            int mvv_lva_max_score = INT32_MIN;
            for (int j = i; j < move_lists_[ply].get_move_idx(); j++) {
                auto next_move_candidate = move_lists_[ply].get_move_at(j);
                if (next_move_candidate.is_any_capture()) {
                    PieceType attacker = pos_.bbs.get_piece_type_at(next_move_candidate.from());
                    PieceType victim   = pos_.bbs.get_piece_type_at(next_move_candidate.to());

                    if (MVV_LVA_TABLE[victim % 6][attacker % 6] > mvv_lva_max_score) {
                        mvv_lva_max_idx = j;
                        mvv_lva_max_score = MVV_LVA_TABLE[victim % 6][attacker % 6];
                    }
                }

            }
            
            // If the max score is unchanged, then no more captures was found
            all_captures_played = mvv_lva_max_score == INT32_MIN;

            move_lists_[ply].swap(i, mvv_lva_max_idx);
            move = move_lists_[ply].get_move_at(i);
        }

        if (all_captures_played && killer_move_idx < 2 && killer_moves_[ply][killer_move_idx].value() != 0) {
            for (int j = i; j < move_lists_[ply].get_move_idx(); j++) {
                auto next_move_canditate = move_lists_[ply].get_move_at(j);
                if (next_move_canditate == killer_moves_[ply][killer_move_idx]) {
                    move_lists_[ply].swap(i, j);
                    move = move_lists_[ply].get_move_at(i);
                    killer_move_idx++;
                    killer_move_swaps_++;
                    break;
                }
            }
        }

        int previous_last_irreversible_move_idx = game_hist_.get_last_irreversible_move_idx();
        undo_stack_[ply] = move_maker_.make_move(move);

        // If move is ep, make the move, check if in check, if so - undo
        if (move.is_ep_capture()) {
            if (move_generator_.in_check())  {
                move_retractor_.unmake_move(move, undo_stack_[ply]);   
                continue;
            }
        }
        bool single_pawn_push = false;
        if (pos_.is_w) {
            single_pawn_push = pos_.bbs.get_piece_type_at(move.to()) == PieceType::W_PAWN && move.is_quiet();
        } else {
            single_pawn_push = pos_.bbs.get_piece_type_at(move.to()) == PieceType::B_PAWN && move.is_quiet();
        }

        uint64_t z_hash = z_hasher_.value();
        if (GameHistory::is_move_irreversible(move, single_pawn_push, pos_.c_rights != undo_stack_[ply].c_rights)) {
            game_hist_.push_irreversible(z_hash);
        } else {
            game_hist_.push(z_hash);
        }

        // We have reached this position 2 times before, and just moved into it again
        // this means that we end the game in draw by three-fold repetition
        int repetitions = game_hist_.count(z_hash);
        if (repetitions == 2 || (repetitions == 1 && ply > 2)) {
            move_retractor_.unmake_move(move, undo_stack_[ply]);
            game_hist_.pop(previous_last_irreversible_move_idx);
            return 0;
        }

        eval_t score = -negamax(depth - 1, -beta, -alpha, ply + 1, true, tm);
        
        move_retractor_.unmake_move(move, undo_stack_[ply]);
        game_hist_.pop(previous_last_irreversible_move_idx);
        if (stop_) break;

        if (score > best_score) {
            best_score = score;
            best_move = move;
        }

        alpha = std::max(score, alpha);

        if (alpha >= beta) {
            if (!move.is_any_capture()) {
                killer_moves_[ply][1] = killer_moves_[ply][0];
                killer_moves_[ply][0] = move;
            }

            break;
        }
    }

    if (stop_) return alpha;

    TTFlag flag = TTFlag::EXACT;
    if (alpha >= beta) {
        flag = TTFlag::LOWER_BOUND;
    } else if (alpha == original_alpha) {
        flag = TTFlag::UPPER_BOUND;
    }

    tt_.store(z_hasher_.value(), alpha, depth, flag, best_move.value());

    return alpha;
}

} // namespace engine