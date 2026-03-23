#pragma once

#include "model/position/position.h"
#include "model/move/movelist.h"
#include "model/move/move.h"

#include "logic/eval/eval.h"
#include "logic/makemove/undo_info.h"
#include "logic/movegen/move_gen.h"
#include "logic/makemove/move_maker.h"
#include "logic/makemove/move_retractor.h"

#include "io/fen.h"

#include <string>

namespace engine {

class Perft {

public:
    Perft(int max_depth);
    
    void minimax(
        int current_depth,
        int first_move_idx,
        bool verbose = true
    );

    void record_perft_stats(
        int current_depth,
        int &first_move_idx,
        size_t i,
        const model::Move& current_move
    );
    
    long long sum_nodes_to_depth(int depth) const;
    std::vector<long long> node_count_per_first_move_;
    std::vector<model::Move> first_moves_;
    std::vector<long long> node_count_;
    std::vector<long long> capture_count_;
    std::vector<long long> ep_capture_count_;
    std::vector<long long> casle_count_;
    std::vector<long long> promo_count_;
    std::vector<long long> check_count_;
    std::vector<long long> double_check_count_;
    std::vector<long long> checkmate_count_;

    inline model::Position get_pos_copy()
    {
        return pos_;
    }

    void set_max_depth(int max_depth) 
    {
        max_depth_ = max_depth;
    }

    int get_max_depth() const {
        return max_depth_;
    }

    void set_pos_from_fen(const std::string& fen)
    {
        io::fen::set_pos_from_fen(fen, pos_);
    }

    model::Position& get_pos() {
        return pos_;
    }

    void z_hash_from_position() {
        z_hasher_.hash_from_position(pos_);
    }

    void reset_stats();
    void reset_stacks();

    std::unordered_map<model::Move, uint64_t> get_node_count_per_first_move_map();

private:
    model::Position pos_;
    logic::ZHasher z_hasher_;
    logic::MoveMaker move_maker_;
    logic::MoveRetractor move_retractor_;
    logic::MoveGen move_generator_;
    logic::Eval eval_;
    int max_depth_;

    std::vector<model::Movelist> move_lists_;
    std::vector<int> no_captures_or_pawn_moves_counts_; 
    std::vector<logic::UndoInfo> undo_stack_;
};

} // namespace engine