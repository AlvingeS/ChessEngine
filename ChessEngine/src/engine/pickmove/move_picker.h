#pragma once

#include "engine/pickmove/time_manager.h"

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

namespace engine {

class MovePicker {

public:
    MovePicker();
    model::Move pick_move(TimeManager& time_manager);
    
    void set_pos_from_fen(const std::string& fen) {
        io::fen::set_pos_from_fen(fen, pos_);
    }

    inline bool get_is_w() { return pos_.is_w; }
    inline void request_stop() { stop_ = true; }

    void z_hash_from_position() {
        z_hasher_.hash_from_position(pos_);
    }

    void make_move(const model::Move& move) {
        move_maker_.make_move(move);
    }

    void reset_position() {
        pos_ = model::Position();
    }    

    model::Movelist gen_moves() {
        auto ml = model::Movelist();
        move_generator_.gen_moves(ml);
        return ml;
    }

    void reset_stacks();

private:
    int negamax(int depth, int alpha, int beta, const TimeManager& tm);

    model::Position pos_;
    logic::ZHasher z_hasher_;
    logic::MoveMaker move_maker_;
    logic::MoveRetractor move_retractor_;
    logic::MoveGen move_generator_;
    logic::Eval eval_;

    long long node_count_;
    std::atomic<bool> stop_;
    static constexpr int BIG_NUMBER{999999};
    static constexpr int MAX_SAFE_DEPTH{64};
    static constexpr int NODE_CHECK_INTERVAL{2048};

    std::vector<model::Movelist> move_lists_;
    std::vector<int> no_captures_or_pawn_moves_counts_; 
    std::vector<logic::UndoInfo> undo_stack_;

};

} // namespace engine