#pragma once

#include "engine/pickmove/search_memory.h"

#include "logic/eval/eval.h"

#include "model/position/board.h"

#include "model/position/piece_map.h"
#include "logic/movegen/move_gen.h"
#include "model/move/movelist.h"
#include "logic/makemove/move_maker.h"
#include "logic/makemove/move_retractor.h"
#include "logic/makemove/move_utils.h"
#include "model/move/move.h"
#include "io/fen.h"

#include <string>

namespace engine {

class MovePicker {

public:
    MovePicker(int maxDepth);
    
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

    void gen_moves(
        bool is_w,
        int current_depth,
        bitmask ep_target_mask, 
        unsigned char castle_rights
    );

    logic::MoveResult make_move(model::Move move, bool is_w);
    void unmake_move(model::Move move, bool is_w, logic::MoveResult prevousMoveResult);
    
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

    void set_max_depth(int maxDepth) 
    {
        max_depth_ = maxDepth;
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
        io::fen::set_board_from_fen(fen, bbs_, occupancy_masks_, piece_map_);
    }

    std::string get_fen_from_board() const
    {
        return io::fen::get_fen_from_board(piece_map_);
    }

    bool diff_between_occupancy_masks() const
    {
        return (occupancy_masks_.get_b_pieces_mask() | occupancy_masks_.get_w_pieces_mask()) != occupancy_masks_.get_occupied_sqrs_mask();
    }
    
private:
    model::Board board_;
    model::Bitboards& bbs_;
    model::PieceMap& piece_map_;
    model::OccupancyMasks& occupancy_masks_;
    model::ZHasher& z_hasher_;
    
    SearchMemory search_memory_;
    logic::MoveMaker move_maker_;
    logic::MoveRetractor move_retractor_;
    logic::MoveGen move_generator_;
    logic::Eval eval_;
    int max_depth_;

    int pseudo_legal_moves_count_;
    std::vector<model::Movelist> move_lists_;
    std::vector<model::Piece::Type> last_captured_pieces_;
    std::vector<int> no_captures_or_pawn_moves_counts_; 

    bool too_many_pieces_on_board();
};

} // namespace engine