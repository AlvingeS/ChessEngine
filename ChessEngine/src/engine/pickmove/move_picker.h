#pragma once

#include "logic/eval/eval.h"
#include "logic/makemove/undo_info.h"

#include "model/position/position.h"
#include "model/move/move.h"

#include "logic/movegen/move_gen.h"
#include "model/move/movelist.h"
#include "logic/makemove/move_maker.h"
#include "logic/makemove/move_retractor.h"
#include "logic/utils.h"

#include "io/fen.h"

#include <string>

namespace engine {

class MovePicker {

public:
    MovePicker(int max_depth);
    
    void minimax(
        int current_depth,
        bool is_maximizer, 
        int first_move_idx,
        bool recPerftStats = true,
        const model::Move& last_move = model::Move(),
        bool verbose = true
    );

    void record_perft_stats(
        bool is_maximizer,
        int current_depth,
        int &first_move_idx,
        size_t i,
        const model::Move& current_move,
        bool &ret_flag
    );
    
    int num_move_gen_calls_;
    int total_nodes_;

    long sum_nodes_to_depth(int depth) const;
    std::vector<long> node_count_per_first_move_;
    std::vector<model::Move> first_moves_;
    std::vector<long> node_count_;
    std::vector<long> capture_count_;
    std::vector<long> ep_capture_count_;
    std::vector<long> casle_count_;
    std::vector<long> promo_count_;
    std::vector<long> check_count_;
    std::vector<long> checkmate_count_;

    void debug_print(bool verbose) const;
    
    bool check_condition(
        int current_depth,
        bool is_maximizer, 
        int first_move_idx, 
        model::Move current_move, 
        model::Move last_move, 
        bool verbose, 
        size_t i
    ) const;

    void set_max_depth(int max_depth) 
    {
        max_depth_ = max_depth;
    }

    int get_max_depth() const {
        return max_depth_;
    }

    const logic::MoveGen& get_move_generator() const
    {
        return move_generator_;
    }

    void set_board_from_fen(const std::string& fen)
    {
        io::fen::set_board_from_fen(fen, pos_);
    }

    std::string get_fen_from_board() const
    {
        return io::fen::get_fen_from_board(pos_.piece_map);
    }

    bool diff_between_occupancy_masks() const
    {
        return (pos_.occ_masks.get_b_pieces_mask() | pos_.occ_masks.get_w_pieces_mask()) != pos_.occ_masks.get_occupied_squares_mask();
    }

private:
    model::Position pos_;
    logic::ZHasher z_hasher_;
    logic::MoveMaker move_maker_;
    logic::MoveRetractor move_retractor_;
    logic::MoveGen move_generator_;
    logic::Eval eval_;
    int max_depth_;

    int pseudo_legal_moves_count_;
    std::vector<model::Movelist> move_lists_;
    std::vector<int> no_captures_or_pawn_moves_counts_; 
    std::vector<logic::UndoInfo> undo_stack_;

    bool too_many_pieces_on_board();
};

} // namespace engine