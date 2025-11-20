#pragma once

#include "model/types.h"
#include "model/move/move.h"
#include "model/position/piece_type.h"

namespace {
    constexpr unsigned char w_kside_castle_rights       = 0b0001;
    constexpr unsigned char w_qside_castle_rights       = 0b0010;
    constexpr unsigned char w_both_sides_castle_rights  = 0b0011;
    constexpr unsigned char b_kside_castle_rights       = 0b0100;
    constexpr unsigned char b_qside_castle_rights       = 0b1000;
    constexpr unsigned char b_both_sides_castle_rights  = 0b1100;
} // namespace

namespace engine {

class SearchMemory {

public:
    SearchMemory(int maxDepth);

    unsigned char get_castling_rights_at_depth(int depth) const
    {
        return castle_rights_[depth];
    }

    model::Piece::Type get_last_captured_piece_at_depth(int depth) const
    {
        return last_captured_pieces_[depth];
    }

    void set_last_captured_piece_at_depth(
        int current_depth,
        model::Piece::Type piece_type) 
    {
        last_captured_pieces_[current_depth] = piece_type;
    }

    void set_castle_rights(
        int current_depth, 
        const model::Move& move, 
        bool is_w, 
        model::Piece::Type moved_piece_type
    );

    void unset_castle_rights(int current_depth);

    bitmask get_ep_target_mask_at_depth(int depth) const
    {
        return ep_target_masks_[depth];
    }

    void set_ep_target_at_depth(int depth, bitmask ep_target_mask) 
    {
        ep_target_masks_[depth] = ep_target_mask;
    }

    int get_no_captures_or_pawn_moves_count_at_depth(int depth) const
    {
        return no_captures_or_pawn_moves_counts_[depth];
    }

    void increment_no_captures_or_pawn_moves_at_depth(int depth) 
    {
        no_captures_or_pawn_moves_counts_[depth]++;
    }

    void decrement_no_captures_or_pawn_moves_count_at_depth(int depth) 
    {
        no_captures_or_pawn_moves_counts_[depth]--;
    }

    void reset_no_captures_or_pawn_moves_count_at_depth(int depth) 
    {
        no_captures_or_pawn_moves_counts_[depth] = 0;
    }

    void handle_no_capture_count(
        const model::Move& move, 
        int current_depth, 
        model::Piece::Type  moved_piece_type
    );

    void handle_ep_memory(
            const model::Move& move, 
            bool is_w, 
            int current_depth, 
            int to_idx
    );

private:
    int max_depth_;
    std::vector<unsigned char> castle_rights_;
    std::vector<model::Piece::Type> last_captured_pieces_;
    std::vector<bitmask> ep_target_masks_;
    std::vector<int> no_captures_or_pawn_moves_counts_;

    void remove_castle_rights_for_remaining_depths(
        int current_depth, 
        unsigned char rights_to_remove
    );
    
    void restore_castle_rights_for_remaining_depths(int current_depth);
    void override_castle_rights(unsigned char rights);

};

} // namespace engine
