#pragma once

#include "engine/search/time_manager.h"
#include "engine/search/transposition_table.h"
#include "engine/search/game_history.h"

#include "model/position/position.h"
#include "model/move/movelist.h"
#include "model/move/move.h"

#include "logic/eval/eval.h"
#include "logic/makemove/undo_info.h"
#include "logic/movegen/move_gen.h"
#include "logic/makemove/move_maker.h"
#include "logic/makemove/move_retractor.h"

#include "io/fen.h"

#include <atomic>
#include <optional>

namespace engine {

class Searcher {

public:
    Searcher();
    model::Move search(TimeManager& time_manager);
    void bench();

    // Wrapper functions
    void set_pos_from_fen(const std::string& fen);
    void make_move(const model::Move& move);
    model::Movelist gen_moves();

    // Inline wrapper functions
    inline bool get_is_w() { return pos_.is_w; }
    inline void request_stop() { stop_ = true; }
    inline void z_hash_from_position() { z_hasher_.hash_from_position(pos_); }
    inline void reset_position() { pos_ = model::Position(); }

    // Reset functions
    void reset_stacks();
    inline void clear_tt() { tt_.clear(); }
    inline void resize_tt(size_t mb) { tt_.resize(mb); }
    inline void clear_game_hist() { game_hist_.clear(); }

private:
    eval_t negamax(int depth, eval_t alpha, eval_t beta, int ply, bool allow_null_move, const TimeManager& tm);
    eval_t quiescence(eval_t alpha, eval_t beta, int ply, const TimeManager& tm);
    bool has_non_pawn_material() const;
    void make_null_move();
    void undo_null_move(bitmask ep_target_mask);

    model::Position pos_;
    logic::ZHasher z_hasher_;
    logic::MoveMaker move_maker_;
    logic::MoveRetractor move_retractor_;
    logic::MoveGen move_generator_;
    logic::Eval eval_;
    
    long long node_count_;
    std::atomic<bool> stop_;
    static constexpr int BIG_NUMBER{INT16_MAX};
    static constexpr int MAX_SAFE_DEPTH{256};
    static constexpr int NODE_CHECK_INTERVAL{2048};
    static constexpr eval_t QUISCIENCE_DELTA_MARGIN{200};
    static constexpr int NULL_MOVE_DEPTH_REDUCTION{2};

    engine::TT tt_;
    long long tt_hits_;
    engine::GameHistory game_hist_;
    std::array<std::array<model::Move, 2>, MAX_SAFE_DEPTH> killer_moves_{};
    long long killer_move_swaps_;

    std::vector<model::Movelist> move_lists_;
    std::vector<int> no_captures_or_pawn_moves_counts_; 
    std::vector<logic::UndoInfo> undo_stack_;
};

} // namespace engine