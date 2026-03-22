#include "logic/makemove/move_maker.h"

#include "model/constants.h"
#include "model/position/position.h"
#include "model/move/move.h"

namespace logic {

MoveMaker::MoveMaker(model::Position& pos, ZHasher& z_hasher) 
    : pos_(pos)
    , z_hasher_(z_hasher)
{}

UndoInfo MoveMaker::make_move(const model::Move& move)
{
    auto undo_info = UndoInfo();

    // Store undo-info state before the move
    store_state(undo_info);

    // If the move is a castle, update and return
    if (move.is_any_castle()) {
        make_castle_move(move.is_king_castle());

        update_ep_target_mask(move);

        auto king_type = pos_.is_w ? model::Piece::Type::W_KING
                                   : model::Piece::Type::B_KING;
        
        undo_info.moved_piece_type = king_type;

        update_castle_rights(move, undo_info);

        pos_.is_w = !pos_.is_w;
        z_hasher_.toggle_side_to_move();

        return undo_info;
    }

    // Get the from and to idxs
    sq_idx from_sq = move.from();
    sq_idx to_sq   = move.to();

    // Pick up the piece from the from square and get the moved piece type
    model::Piece::Type moved_piece_type = remove_moved_piece_from_board(from_sq);

    // If the move is a capture, handle memory and remove the captured piece
    if (move.is_any_capture()) {
        // Calculate sq of captured piece, might be EP
        sq_idx capture_sq = utils::determine_capture_sq(move, pos_.is_w);

        model::Piece::Type captured_piece_type = pos_.piece_map.get_piece_type_at(capture_sq);
        remove_captured_piece_from_board(move.is_ep_capture(), capture_sq, captured_piece_type);
        
        undo_info.captured_piece_type = captured_piece_type;
    }

    // Update the moved piece type if the move is a promotion    
    if (move.is_any_promo())
        moved_piece_type = utils::get_promotion_piece_type(move.flag(), pos_.is_w);

    undo_info.moved_piece_type = moved_piece_type;

    // Place the moved piece on the to square
    place_moved_piece_on_board(to_sq, moved_piece_type);

    // Update occupied and empty squares bitmasks
    pos_.occ_masks.update_occupancy_masks();

    // Update ep target mask based on if the move was a double pawn push or not
    update_ep_target_mask(move);

    update_castle_rights(move, undo_info);

    pos_.is_w = !pos_.is_w;
    z_hasher_.toggle_side_to_move();

    return undo_info;
}

void MoveMaker::store_state(UndoInfo& undo_info)
{
    undo_info.c_rights = pos_.c_rights;
    undo_info.ep_target_mask = pos_.ep_target_mask;
    undo_info.z_hash = z_hasher_.value();
}

void MoveMaker::update_ep_target_mask(const model::Move& move)
{
    // XOR out old ep mask
    if (pos_.ep_target_mask != 0ULL)
        z_hasher_.xor_ep_file_at(utils::file_from_sq(utils::lsb_idx(pos_.ep_target_mask)));

    if (!move.is_double_pawn_push()) {
        pos_.ep_target_mask = 0ULL;
    } else {
        pos_.ep_target_mask = pos_.is_w ? (1ULL << (move.to() - 8)) 
                                        : (1ULL << (move.to() + 8));
    }

    // XOR in new ep mask
    if (pos_.ep_target_mask != 0ULL)
        z_hasher_.xor_ep_file_at(utils::file_from_sq(utils::lsb_idx(pos_.ep_target_mask)));
}

void MoveMaker::update_castle_rights(const model::Move& move, UndoInfo& undo_info) 
{
    castle_rights old_castle_rights = pos_.c_rights;

    // If no one has any rights then there is nothing to update
    if (pos_.c_rights == 0)
        return;

    // If it is white but he has no rights then there is nothing to update
    if (pos_.is_w && (pos_.c_rights & masks::W_BOTH_SIDES_CASTLE_RIGHTS_MASK) == 0)
        return;

    // If it is black but he has no rights then there is nothing to update
    if (!pos_.is_w && (pos_.c_rights & masks::B_BOTH_SIDES_CASTLE_RIGHTS_MASK) == 0)
        return;

    // If the move is castle, remove all rights for that player
    if (move.is_any_castle()) {
        if (pos_.is_w) {
            pos_.c_rights &= ~masks::W_BOTH_SIDES_CASTLE_RIGHTS_MASK;
        } else {
            pos_.c_rights &= ~masks::B_BOTH_SIDES_CASTLE_RIGHTS_MASK;
        }
    }

    if (pos_.is_w) {
        // If move is made by white king, remove all white castling rights
        if (undo_info.moved_piece_type == model::Piece::Type::W_KING) {
            pos_.c_rights &= ~masks::W_BOTH_SIDES_CASTLE_RIGHTS_MASK;
            z_hasher_.update_castle_rights(old_castle_rights, pos_.c_rights);
            return;
        }
        
        // If the the players king or any of the rooks was not moved or an opponent rook has not been taken
        // there is nothing to do. 
        bool rook_moved = (move.from() == constants::W_KSIDE_ROOK_START_SQ 
                        || move.from() == constants::W_QSIDE_ROOK_START_SQ)
                        && undo_info.moved_piece_type == model::Piece::Type::W_ROOK;

        bool opp_rook_captured = move.is_any_capture() 
                              && undo_info.captured_piece_type == model::Piece::Type::B_ROOK
                              && (move.to() == constants::B_KSIDE_ROOK_START_SQ 
                              ||  move.to() == constants::B_QSIDE_ROOK_START_SQ); 


        if (!rook_moved && !opp_rook_captured)
            return;

        if (rook_moved) {
            if (move.from() == constants::W_KSIDE_ROOK_START_SQ) {
                pos_.c_rights &= ~masks::W_KSIDE_CASTLE_RIGHTS_MASK; // If move was made from kside, remove kside c_rights
            } else {
                pos_.c_rights &= ~masks::W_QSIDE_CASTLE_RIGHTS_MASK; // Else, move was made from qside, remove qside c_rights
            }
        }

        if (opp_rook_captured) {
            if (move.to() == constants::B_KSIDE_ROOK_START_SQ) {
                pos_.c_rights &= ~masks::B_KSIDE_CASTLE_RIGHTS_MASK; // If rook captured is on kside, remove opp kside c_rights
            } else {
                pos_.c_rights &= ~masks::B_QSIDE_CASTLE_RIGHTS_MASK; // If rook captured is on qside, remove opp qside c_rights
            }
        }
    } else {
        // If move is made by black king, remove all black castling rights
        if (undo_info.moved_piece_type == model::Piece::Type::B_KING) {
            pos_.c_rights &= ~masks::B_BOTH_SIDES_CASTLE_RIGHTS_MASK;
            z_hasher_.update_castle_rights(old_castle_rights, pos_.c_rights);
            return;
        }
        
        // If the the players king or any of the rooks was not moved or an opponent rook has not been taken
        // there is nothing to do. 
        bool rook_moved = (move.from() == constants::B_KSIDE_ROOK_START_SQ 
                        || move.from() == constants::B_QSIDE_ROOK_START_SQ)
                        && undo_info.moved_piece_type == model::Piece::Type::B_ROOK;

        bool opp_rook_captured = move.is_any_capture() 
                              && undo_info.captured_piece_type == model::Piece::Type::W_ROOK
                              && (move.to() == constants::W_KSIDE_ROOK_START_SQ 
                              ||  move.to() == constants::W_QSIDE_ROOK_START_SQ); 


        if (!rook_moved && !opp_rook_captured)
            return;

        if (rook_moved) {
            if (move.from() == constants::B_KSIDE_ROOK_START_SQ) {
                pos_.c_rights &= ~masks::B_KSIDE_CASTLE_RIGHTS_MASK; // If move was made from kside, remove kside c_rights
            } else {
                pos_.c_rights &= ~masks::B_QSIDE_CASTLE_RIGHTS_MASK; // Else, move was made from qside, remove qside c_rights
            }
        }

        if (opp_rook_captured) {
            if (move.to() == constants::W_KSIDE_ROOK_START_SQ) {
                pos_.c_rights &= ~masks::W_KSIDE_CASTLE_RIGHTS_MASK; // If rook captured is on kside, remove opp kside c_rights
            } else {
                pos_.c_rights &= ~masks::W_QSIDE_CASTLE_RIGHTS_MASK; // If rook captured is on qside, remove opp qside c_rights
            }
        }
    }

    z_hasher_.update_castle_rights(old_castle_rights, pos_.c_rights);
}

void MoveMaker::make_castle_move(bool is_kside)
{
    sq_idx king_from_sq, king_to_sq, rook_from_sq, rook_to_sq;

    if (pos_.is_w) {
        king_from_sq = constants::W_KING_START_SQ;
        king_to_sq   = is_kside ? constants::W_KSIDE_KING_CASTLE_TO_SQ : constants::W_QSIDE_KING_CASTLE_TO_SQ;
        rook_from_sq = is_kside ? constants::W_KSIDE_ROOK_START_SQ     : constants::W_QSIDE_ROOK_START_SQ;
        rook_to_sq   = is_kside ? constants::W_KSIDE_ROOK_CASTLE_TO_SQ : constants::W_QSIDE_ROOK_CASTLE_TO_SQ;

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

        z_hasher_.xor_piece_type_at(king_from_sq, model::Piece::Type::W_KING);
        z_hasher_.xor_piece_type_at(king_to_sq,   model::Piece::Type::W_KING);
        z_hasher_.xor_piece_type_at(rook_from_sq, model::Piece::Type::W_ROOK);
        z_hasher_.xor_piece_type_at(rook_to_sq,   model::Piece::Type::W_ROOK);
    } else {
        king_from_sq = constants::B_KING_START_SQ;
        king_to_sq   = is_kside ? constants::B_KSIDE_KING_CASTLE_TO_SQ : constants::B_QSIDE_KING_CASTLE_TO_SQ;
        rook_from_sq = is_kside ? constants::B_KSIDE_ROOK_START_SQ     : constants::B_QSIDE_ROOK_START_SQ;
        rook_to_sq   = is_kside ? constants::B_KSIDE_ROOK_CASTLE_TO_SQ : constants::B_QSIDE_ROOK_CASTLE_TO_SQ;

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

        z_hasher_.xor_piece_type_at(king_from_sq, model::Piece::Type::B_KING);
        z_hasher_.xor_piece_type_at(king_to_sq,   model::Piece::Type::B_KING);
        z_hasher_.xor_piece_type_at(rook_from_sq, model::Piece::Type::B_ROOK);
        z_hasher_.xor_piece_type_at(rook_to_sq,   model::Piece::Type::B_ROOK);
    }

    pos_.occ_masks.update_occupancy_masks();
}

model::Piece::Type MoveMaker::remove_moved_piece_from_board(sq_idx from_sq) 
{
    // Determine the piece type of the piece being moved
    model::Piece::Type  moved_piece_type = pos_.piece_map.get_piece_type_at(from_sq);

    // Update zobrist hash
    z_hasher_.xor_piece_type_at(from_sq, moved_piece_type);

    // Clear the piece from bbs, squarelookup and gamestate bitmasks
    pos_.bbs.clear_piece_type_bit(from_sq, moved_piece_type);
    pos_.piece_map.set_piece_type_at(from_sq, model::Piece::Type::EMPTY);

    pos_.is_w ? pos_.occ_masks.clear_w_pieces_bit(from_sq) 
              : pos_.occ_masks.clear_b_pieces_bit(from_sq);

    return moved_piece_type;
}

void MoveMaker::place_moved_piece_on_board(
    sq_idx to_sq, 
    model::Piece::Type moved_piece_type) 
{
    pos_.bbs.set_piece_type_bit(to_sq, moved_piece_type);
    pos_.piece_map.set_piece_type_at(to_sq, moved_piece_type);

    z_hasher_.xor_piece_type_at(to_sq, moved_piece_type);

    pos_.is_w ? pos_.occ_masks.set_w_pieces_bit(to_sq) 
              : pos_.occ_masks.set_b_pieces_bit(to_sq);
}

void MoveMaker::remove_captured_piece_from_board(bool is_ep, sq_idx capture_sq, model::Piece::Type captured_piece_type)
{
    // Remove captured piece from models
    pos_.bbs.clear_piece_type_bit(capture_sq, captured_piece_type);

    pos_.is_w ? pos_.occ_masks.clear_b_pieces_bit(capture_sq) 
              : pos_.occ_masks.clear_w_pieces_bit(capture_sq);

    pos_.piece_map.set_piece_type_at(capture_sq, model::Piece::Type::EMPTY); 

    z_hasher_.xor_piece_type_at(capture_sq, captured_piece_type);
}

} // namespace logic