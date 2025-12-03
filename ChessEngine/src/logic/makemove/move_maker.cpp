#include "logic/makemove/move_maker.h"

#include "model/constants.h"
#include "model/position/position.h"
#include "model/move/move.h"

namespace logic {

MoveMaker::MoveMaker(model::Position& pos, ZHasher& z_hasher) 
    : pos_(pos)
    , z_hasher_(z_hasher)
{}

UndoInfo MoveMaker::make_move(const model::Move& move, bool is_w)
{
    auto undo_info = UndoInfo();

    // Store undo-info state before the move
    store_state(undo_info);

    // If the move is a castle, update and return
    if (move.is_any_castle()) {
        make_castle_move(is_w, move.is_king_castle());

        update_ep_target_mask(move, is_w);

        auto king_type = is_w ? model::Piece::Type::W_KING
                              : model::Piece::Type::B_KING;
        undo_info.moved_piece_type = king_type;

        update_castle_rights(move, is_w, undo_info);

        return undo_info;
    }

    // Get the from and to idxs
    sq_idx from_sq = move.get_from_sq();
    sq_idx to_sq   = move.get_to_sq();

    // Pick up the piece from the from square and get the moved piece type
    model::Piece::Type moved_piece_type = remove_moved_piece_from_board(is_w, from_sq);

    // If the move is a capture, handle memory and remove the captured piece
    if (move.is_any_capture()) {
        // Calculate sq of captured piece, might be EP
        sq_idx capture_sq = utils::determine_capture_sq(move, is_w);

        model::Piece::Type captured_piece_type = pos_.piece_map.get_piece_type_at(capture_sq);
        remove_captured_piece_from_board(move.is_ep_capture(), is_w, capture_sq, captured_piece_type);
        
        undo_info.captured_piece_type = captured_piece_type;
    }

    // Update the moved piece type if the move is a promotion    
    if (move.is_any_promo())
        moved_piece_type = utils::get_promotion_piece_type(move.get_flag(), is_w);

    undo_info.moved_piece_type = moved_piece_type;

    // Place the moved piece on the to square
    place_moved_piece_on_board(is_w, to_sq, moved_piece_type);

    // Update occupied and empty squares bitmasks
    pos_.occ_masks.update_occupancy_masks();

    // Update ep target mask based on if the move was a double pawn push or not
    update_ep_target_mask(move, is_w);

    // Todo: search_memory_.handle_no_capture_count(move, current_depth, move_result.moved_piece_type);

    update_castle_rights(move, is_w, undo_info);

    return undo_info;
}

void MoveMaker::store_state(UndoInfo& undo_info)
{
    undo_info.c_rights = pos_.c_rights;
    undo_info.ep_target_mask = pos_.ep_target_mask;
    undo_info.z_hash = z_hasher_.value();
}

void MoveMaker::update_ep_target_mask(const model::Move& move, bool is_w)
{
    if (!move.is_double_pawn_push()) {
        pos_.ep_target_mask = 0ULL;
    } else {
        pos_.ep_target_mask = is_w ? (1ULL << (move.get_to_sq() - 8)) 
                                   : (1ULL << (move.get_to_sq() + 8));
                                   // FIXME: Temporary because I don't know how to implement this haha
                                   // z_hasher_.xor_ep_file_at(to_sq % 8);
    }
}

void MoveMaker::update_castle_rights(const model::Move& move, bool is_w, UndoInfo& undo_info) 
{
    // If no one has any rights then there is nothing to update
    if (pos_.c_rights == 0)
        return;

    // If it is white but he has no rights then there is nothing to update
    if (is_w && (pos_.c_rights & masks::W_BOTH_SIDES_CASTLE_RIGHTS_MASK) == 0)
        return;

    // If it is black but he has no rights then there is nothing to update
    if (!is_w && (pos_.c_rights & masks::B_BOTH_SIDES_CASTLE_RIGHTS_MASK) == 0)
        return;

    // If the move is castle, remove all rights for that player
    if (move.is_any_castle()) {
        if (is_w) {
            pos_.c_rights &= ~masks::W_BOTH_SIDES_CASTLE_RIGHTS_MASK;
        } else {
            pos_.c_rights &= ~masks::B_BOTH_SIDES_CASTLE_RIGHTS_MASK;
        }
    }

    if (is_w) {
        // If move is made by white king, remove all white castling rights
        if (undo_info.moved_piece_type == model::Piece::Type::W_KING) {
            pos_.c_rights &= ~masks::W_BOTH_SIDES_CASTLE_RIGHTS_MASK;
            return;
        }
        
        // If the the players king or any of the rooks was not moved or an opponent rook has not been taken
        // there is nothing to do. 
        bool rook_moved = (move.get_from_sq() == constants::W_KSIDE_ROOK_START_SQ 
                        || move.get_from_sq() == constants::W_QSIDE_ROOK_START_SQ)
                        && undo_info.moved_piece_type == model::Piece::Type::W_ROOK;

        bool opp_rook_captured = move.is_any_capture() 
                              && undo_info.captured_piece_type == model::Piece::Type::B_ROOK
                              && (move.get_to_sq() == constants::B_KSIDE_ROOK_START_SQ 
                              ||  move.get_to_sq() == constants::B_QSIDE_ROOK_START_SQ); 


        if (!rook_moved && !opp_rook_captured)
            return;

        if (rook_moved) {
            if (move.get_from_sq() == constants::W_KSIDE_ROOK_START_SQ) {
                pos_.c_rights &= ~masks::W_KSIDE_CASTLE_RIGHTS_MASK; // If move was made from kside, remove kside c_rights
            } else {
                pos_.c_rights &= ~masks::W_QSIDE_CASTLE_RIGHTS_MASK; // Else, move was made from qside, remove qside c_rights
                return;
            }
        }

        if (opp_rook_captured) {
            if (move.get_to_sq() == constants::B_KSIDE_ROOK_START_SQ) {
                pos_.c_rights &= ~masks::B_KSIDE_CASTLE_RIGHTS_MASK; // If rook captured is on kside, remove opp kside c_rights
            } else {
                pos_.c_rights &= ~masks::B_QSIDE_CASTLE_RIGHTS_MASK; // If rook captured is on qside, remove opp qside c_rights
            }
        }
    } else {
        // If move is made by white king, remove all white castling rights
        if (undo_info.moved_piece_type == model::Piece::Type::B_KING) {
            pos_.c_rights &= ~masks::B_BOTH_SIDES_CASTLE_RIGHTS_MASK;
            return;
        }
        
        // If the the players king or any of the rooks was not moved or an opponent rook has not been taken
        // there is nothing to do. 
        bool rook_moved = (move.get_from_sq() == constants::B_KSIDE_ROOK_START_SQ 
                        || move.get_from_sq() == constants::B_QSIDE_ROOK_START_SQ)
                        && undo_info.moved_piece_type == model::Piece::Type::B_ROOK;

        bool opp_rook_captured = move.is_any_capture() 
                              && undo_info.captured_piece_type == model::Piece::Type::W_ROOK
                              && (move.get_to_sq() == constants::W_KSIDE_ROOK_START_SQ 
                              ||  move.get_to_sq() == constants::W_QSIDE_ROOK_START_SQ); 


        if (!rook_moved && !opp_rook_captured)
            return;

        if (rook_moved) {
            if (move.get_from_sq() == constants::B_KSIDE_ROOK_START_SQ) {
                pos_.c_rights &= ~masks::B_KSIDE_CASTLE_RIGHTS_MASK; // If move was made from kside, remove kside c_rights
            } else {
                pos_.c_rights &= ~masks::B_QSIDE_CASTLE_RIGHTS_MASK; // Else, move was made from qside, remove qside c_rights
                return;
            }
        }

        if (opp_rook_captured) {
            if (move.get_to_sq() == constants::W_KSIDE_ROOK_START_SQ) {
                pos_.c_rights &= ~masks::W_KSIDE_CASTLE_RIGHTS_MASK; // If rook captured is on kside, remove opp kside c_rights
            } else {
                pos_.c_rights &= ~masks::W_QSIDE_CASTLE_RIGHTS_MASK; // If rook captured is on qside, remove opp qside c_rights
            }
        }
    }
}

void MoveMaker::make_castle_move(bool is_w, bool is_kside)
{
    sq_idx king_from_sq, king_to_sq, rook_from_sq, rook_to_sq;

    if (is_w) {
        king_from_sq = 3;
        king_to_sq   = is_kside ? 1 : 5;
        rook_from_sq = is_kside ? 0 : 7;
        rook_to_sq   = is_kside ? 2 : 4;

        pos_.bbs.clear_w_king_bit(king_from_sq);
        pos_.bbs.set_w_king_bit(king_to_sq);
        pos_.bbs.clear_w_rooks_bit(rook_from_sq);
        pos_.bbs.set_w_rooks_bit(rook_to_sq);

        pos_.occ_masks.clear_w_pieces_bit(king_from_sq);
        pos_.occ_masks.set_w_pieces_bit(king_to_sq);
        pos_.occ_masks.clear_w_pieces_bit(rook_from_sq);
        pos_.occ_masks.set_w_pieces_bit(rook_to_sq);

        pos_.piece_map.set_piece_type_at(king_from_sq, model::Piece::Type::EMPTY);
        pos_.piece_map.set_piece_type_at(king_to_sq,   model::Piece::Type::W_KING);
        pos_.piece_map.set_piece_type_at(rook_from_sq, model::Piece::Type::EMPTY);
        pos_.piece_map.set_piece_type_at(rook_to_sq,   model::Piece::Type::W_ROOK);
    } else {
        king_from_sq = 59;
        king_to_sq   = is_kside ? 57 : 61;
        rook_from_sq = is_kside ? 56 : 63;
        rook_to_sq   = is_kside ? 58 : 60;

        pos_.bbs.clear_b_king_bit(king_from_sq);
        pos_.bbs.set_b_king_bit(king_to_sq);
        pos_.bbs.clear_b_rooks_bit(rook_from_sq);
        pos_.bbs.set_b_rooks_bit(rook_to_sq);

        pos_.occ_masks.clear_b_pieces_bit(king_from_sq);
        pos_.occ_masks.set_b_pieces_bit(king_to_sq);
        pos_.occ_masks.clear_b_pieces_bit(rook_from_sq);
        pos_.occ_masks.set_b_pieces_bit(rook_to_sq);

        pos_.piece_map.set_piece_type_at(king_from_sq, model::Piece::Type::EMPTY);
        pos_.piece_map.set_piece_type_at(king_to_sq,   model::Piece::Type::B_KING);
        pos_.piece_map.set_piece_type_at(rook_from_sq, model::Piece::Type::EMPTY);
        pos_.piece_map.set_piece_type_at(rook_to_sq,   model::Piece::Type::B_ROOK);
    }

    pos_.occ_masks.update_occupancy_masks();
}

void MoveMaker::make_temporary_king_move(bool is_w, bool is_kside)
{
    sq_idx from_sq = is_w ? 3 : 59;
    sq_idx to_sq   = is_kside ? (is_w ? 2 : 58)
                              : (is_w ? 4 : 60);

    if (is_w) {
        pos_.bbs.clear_w_king_bit(from_sq);
        pos_.bbs.set_w_king_bit(to_sq);
    } else {
        pos_.bbs.clear_b_king_bit(from_sq);
        pos_.bbs.set_b_king_bit(to_sq);
    }
}


model::Piece::Type MoveMaker::remove_moved_piece_from_board(bool is_w, sq_idx from_sq) 
{
    // Determine the piece type of the piece being moved
    model::Piece::Type  moved_piece_type = pos_.piece_map.get_piece_type_at(from_sq);

    // Update zobrist hash
    z_hasher_.xor_piece_type_at(from_sq, moved_piece_type);

    // Clear the piece from bbs, squarelookup and gamestate bitmasks
    pos_.bbs.clear_piece_type_bit(from_sq, moved_piece_type);
    pos_.piece_map.set_piece_type_at(from_sq, model::Piece::Type::EMPTY);

    is_w ? pos_.occ_masks.clear_w_pieces_bit(from_sq) 
         : pos_.occ_masks.clear_b_pieces_bit(from_sq);

    return moved_piece_type;
}

void MoveMaker::place_moved_piece_on_board(
    bool is_w, 
    sq_idx to_sq, 
    model::Piece::Type moved_piece_type) 
{
    pos_.bbs.set_piece_type_bit(to_sq, moved_piece_type);
    pos_.piece_map.set_piece_type_at(to_sq, moved_piece_type);

    z_hasher_.xor_piece_type_at(to_sq, moved_piece_type);

    is_w ? pos_.occ_masks.set_w_pieces_bit(to_sq) 
         : pos_.occ_masks.set_b_pieces_bit(to_sq);
}

void MoveMaker::remove_captured_piece_from_board(bool is_ep, bool is_w, sq_idx capture_sq, model::Piece::Type captured_piece_type) {
    // Remove captured piece from models
    pos_.bbs.clear_piece_type_bit(capture_sq, captured_piece_type);

    is_w ? pos_.occ_masks.clear_b_pieces_bit(capture_sq) 
         : pos_.occ_masks.clear_w_pieces_bit(capture_sq);

    z_hasher_.xor_piece_type_at(capture_sq, captured_piece_type);

    // Only clear from the squares lookup if the move was an ep capture
    // because the capture idx points to the square where the pawn was
    // and is now empty, the square we moved to will have been updated
    if (is_ep) {
        pos_.piece_map.set_piece_type_at(capture_sq, model::Piece::Type::EMPTY);
    }
}

} // namespace logic