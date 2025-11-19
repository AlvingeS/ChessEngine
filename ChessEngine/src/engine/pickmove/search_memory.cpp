#include "engine/pickmove/search_memory.h"

#include "model/position/piece_type.h"

namespace engine {
    
SearchMemory::SearchMemory(int maxDepth) : max_depth_(maxDepth) 
{
    castle_rights_.resize(max_depth_ + 1);
    last_captured_pieces_.resize(max_depth_ + 1);
    ep_targets_.resize(max_depth_ + 1);
    no_captures_or_pawn_moves_counts_.resize(max_depth_ + 1);

    for (int i = 0; i <= max_depth_; i++) {
        castle_rights_[i] = 0b1111;
        last_captured_pieces_[i] = model::Piece::Type::EMPTY;
        ep_targets_[i] = 0ULL;
        no_captures_or_pawn_moves_counts_[i] = 0;
    }
}

void SearchMemory::remove_castle_rights_for_remaining_depths(
    int currentDepth,
    unsigned char rightsToRemove) 
{
    for (int i = currentDepth + 1; i < max_depth_; i++) {
        castle_rights_[i] &= ~rightsToRemove;
    }
}

void SearchMemory::restore_castle_rights_for_remaining_depths(int currentDepth) 
{
    for (int i = currentDepth + 1; i < max_depth_; i++) {
        castle_rights_[i] = castle_rights_[currentDepth];
    }
}

void SearchMemory::set_castle_rights(
    int currentDepth, 
    const model::Move& move, 
    bool is_w, 
    model::Piece::Type moved_piece_type) 
{
    if (move.is_any_castle()) {
        remove_castle_rights_for_remaining_depths(
            currentDepth, 
            is_w ? w_both_sides_castle_rights : b_both_sides_castle_rights
        );
    }

    if (moved_piece_type == model::Piece::Type::W_KING || moved_piece_type == model::Piece::Type::B_KING) {
        if (is_w) {
            if (castle_rights_[currentDepth] & w_both_sides_castle_rights)
                remove_castle_rights_for_remaining_depths(currentDepth, w_both_sides_castle_rights);
        } else {
            if (castle_rights_[currentDepth] & b_both_sides_castle_rights)
                remove_castle_rights_for_remaining_depths(currentDepth, b_both_sides_castle_rights);
        }
    }

    if (moved_piece_type == model::Piece::Type::W_ROOK || moved_piece_type == model::Piece::Type::B_ROOK) {
        if (is_w) {
            if (move.get_bit_index_from() == 0) {
                if (castle_rights_[currentDepth] & w_kside_castle_rights)
                    remove_castle_rights_for_remaining_depths(currentDepth, w_kside_castle_rights);
            } else if (move.get_bit_index_from() == 7) {
                if (castle_rights_[currentDepth] & w_qside_castle_rights)
                    remove_castle_rights_for_remaining_depths(currentDepth, w_qside_castle_rights);
            }
        } else {
            if (move.get_bit_index_from() == 56) {
                if (castle_rights_[currentDepth] & b_kside_castle_rights)
                    remove_castle_rights_for_remaining_depths(currentDepth, b_kside_castle_rights);
            } else if (move.get_bit_index_from() == 63) {
                if (castle_rights_[currentDepth] & b_qside_castle_rights)
                    remove_castle_rights_for_remaining_depths(currentDepth, b_qside_castle_rights);
            }
        }
    }
}

void SearchMemory::unset_castle_rights(int currentDepth) 
{
    if (castle_rights_[currentDepth] != castle_rights_[currentDepth + 1]) {
        restore_castle_rights_for_remaining_depths(currentDepth);
    }
}

void SearchMemory::override_castle_rights(unsigned char rights) 
{
    for (int i = 0; i < max_depth_; i++) {
        castle_rights_[i] = rights;
    }
}

void SearchMemory::handle_no_capture_count(
    const model::Move& move, 
    int currentDepth, 
    model::Piece::Type  moved_piece_type)
{
    // If the move is a capture, reset the no capture count
    if (move.is_any_capture()) {
        reset_no_captures_or_pawn_moves_count_at_depth(currentDepth + 1);
        return;
    }

    // If the move is a pawn move, reset the no capture count
    if (moved_piece_type == model::Piece::Type::W_PAWN || moved_piece_type == model::Piece::Type::B_PAWN) {
        reset_no_captures_or_pawn_moves_count_at_depth(currentDepth + 1);
        return;
    }

    // If the move is not a capture or pawn move, increment the no capture count
    increment_no_captures_or_pawn_moves_at_depth(currentDepth + 1);
}

void SearchMemory::handle_ep_memory(
    const model::Move& move, 
    bool is_w, 
    int currentDepth, 
    int toIndex) 
{
    if (not move.is_double_pawn_push()) {
        set_ep_target_at_depth(currentDepth + 1, 0ULL);
        return;
    }

    if (move.is_double_pawn_push()) {
        bitmask enPessantTarget = is_w ? (1ULL << (toIndex - 8)) 
                                          : (1ULL << (toIndex + 8));

        set_ep_target_at_depth(currentDepth + 1, enPessantTarget);
        
        // FIXME: Temporary because I don't know how to implement this haha
        // z_hasher_.hash_en_pessant_file(toIndex % 8);
    }
}

} // namespace engine