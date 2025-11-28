#include "logic/makemove/move_retractor.h"

#include "logic/utils.h"

#include "model/position/position.h"
#include "model/move/move.h"

namespace logic {

MoveRetractor::MoveRetractor(model::Position& pos) : pos_(pos) {}

void MoveRetractor::unmake_castle_move(bool was_w, bool was_kside)
{
    sq_idx king_from_sq, king_to_sq, rook_from_sq, rook_to_sq;

    if (was_w) {
        king_from_sq = 3;
        king_to_sq   = was_kside ? 1 : 5;
        rook_from_sq = was_kside ? 0 : 7;
        rook_to_sq = was_kside ? 2 : 4;
        
        pos_.bbs.clear_w_king_bit(king_to_sq);
        pos_.bbs.set_w_king_bit(king_from_sq);
        pos_.bbs.clear_w_rooks_bit(rook_to_sq);
        pos_.bbs.set_w_rooks_bit(rook_from_sq);

        pos_.occ_masks.set_w_pieces_bit(king_from_sq);
        pos_.occ_masks.clear_w_pieces_bit(king_to_sq);
        pos_.occ_masks.set_w_pieces_bit(rook_from_sq);
        pos_.occ_masks.clear_w_pieces_bit(rook_to_sq);

        pos_.piece_map.set_piece_type_at(king_from_sq, model::Piece::Type::W_KING);
        pos_.piece_map.set_piece_type_at(king_to_sq, model::Piece::Type::EMPTY);
        pos_.piece_map.set_piece_type_at(rook_from_sq, model::Piece::Type::W_ROOK);
        pos_.piece_map.set_piece_type_at(rook_to_sq, model::Piece::Type::EMPTY);
    } else {
        king_from_sq = 59;
        king_to_sq = was_kside ? 57 : 61;
        rook_from_sq = was_kside ? 56 : 63;
        rook_to_sq = was_kside ? 58 : 60;

        pos_.bbs.set_b_king_bit(king_from_sq);
        pos_.bbs.clear_b_king_bit(king_to_sq);
        pos_.bbs.set_b_rooks_bit(rook_from_sq);
        pos_.bbs.clear_b_rooks_bit(rook_to_sq);

        pos_.occ_masks.set_b_pieces_bit(king_from_sq);
        pos_.occ_masks.clear_b_pieces_bit(king_to_sq);
        pos_.occ_masks.set_b_pieces_bit(rook_from_sq);
        pos_.occ_masks.clear_b_pieces_bit(rook_to_sq);

        pos_.piece_map.set_piece_type_at(king_to_sq, model::Piece::Type::EMPTY);
        pos_.piece_map.set_piece_type_at(king_from_sq, model::Piece::Type::B_KING);
        pos_.piece_map.set_piece_type_at(rook_to_sq, model::Piece::Type::EMPTY);
        pos_.piece_map.set_piece_type_at(rook_from_sq, model::Piece::Type::B_ROOK);
    }

    pos_.occ_masks.update_occupancy_masks();
}

void MoveRetractor::revert_temporary_king_move(bool was_w, bool is_kside) 
{
    sq_idx from_sq = is_kside ? (was_w ? 2 : 58) 
                              : (was_w ? 4 : 60);

    sq_idx to_sq = was_w ? 3 : 59;

    if (was_w) {
        pos_.bbs.clear_w_king_bit(from_sq);
        pos_.bbs.set_w_king_bit(to_sq);
    } else {
        pos_.bbs.clear_b_king_bit(from_sq);
        pos_.bbs.set_b_king_bit(to_sq);
    }
}

void MoveRetractor::remove_previously_moved_piece_from_board(
    const model::Move& move,
    sq_idx to_sq,
    model::Piece::Type previously_moved_piece_type,
    bool was_w) 
{
    // Square lookup is dependent on if there was a capture or promotion,
    // handled by the place_back_captured_piece_on_board method 

    // If the move was not a promotion, remove the piece in the bitboard
    // Else, remove the bit for the promoted piece
    if (not move.is_any_promo()) {
        pos_.bbs.clear_piece_type_bit(to_sq, previously_moved_piece_type);
    } else {
        model::Piece::Type promotionPieceType = utils::get_promotion_piece_type(move.get_flag(), was_w);
        pos_.bbs.clear_piece_type_bit(to_sq, promotionPieceType);
    }

    was_w ? pos_.occ_masks.clear_w_pieces_bit(to_sq) 
          : pos_.occ_masks.clear_b_pieces_bit(to_sq);
}


void MoveRetractor::place_back_captured_piece_on_board(
    bool is_ep,
    sq_idx capture_sq,
    sq_idx to_sq,
    bool was_w,
    model::Piece::Type previously_captured_piece_type) 
{
    pos_.bbs.set_piece_type_bit(capture_sq, previously_captured_piece_type);
    pos_.piece_map.set_piece_type_at(capture_sq, previously_captured_piece_type);

    // If the move was an ep capture, the to square will be empty
    if (is_ep) {
        pos_.piece_map.set_piece_type_at(to_sq, model::Piece::Type::EMPTY);
    }

    was_w ? pos_.occ_masks.set_b_pieces_bit(capture_sq) 
          : pos_.occ_masks.set_w_pieces_bit(capture_sq);
}

void MoveRetractor::place_back_moved_piece_on_board(
    bool was_w, 
    sq_idx from_sq, 
    model::Piece::Type  moved_piece_type)
{
    pos_.bbs.set_piece_type_bit(from_sq, moved_piece_type);
    pos_.piece_map.set_piece_type_at(from_sq, moved_piece_type);

    was_w ? pos_.occ_masks.set_w_pieces_bit(from_sq) 
          : pos_.occ_masks.set_b_pieces_bit(from_sq);
}

model::Piece::Type MoveRetractor::determine_moved_piece_type(
    const model::Move& move, 
    bool was_w,
    sq_idx to_sq) const
{
    // Piece type of piece being moved
    model::Piece::Type  previously_moved_piece_type;

    // If the move was a promotion, set the moved piece to a pawn of the same color
    // Else, set the moved piece to the piece occupying the to square
    previously_moved_piece_type = move.is_any_promo() ? (was_w ? model::Piece::Type::W_PAWN
                                                               : model::Piece::Type::B_PAWN)
                                                      : pos_.piece_map.get_piece_type_at(to_sq);

    return previously_moved_piece_type;
}

void MoveRetractor::restore_state(logic::UndoInfo& undo_info)
{
    pos_.c_rights = undo_info.c_rights;
    pos_.ep_target_mask = undo_info.ep_target_mask;
}

void MoveRetractor::unmake_move(
    const model::Move& previous_move, 
    bool was_w, 
    logic::UndoInfo& undo_info) 
{
    // If the move is a castle, update the bbs and return
    if (previous_move.is_any_castle()) {
        unmake_castle_move(was_w, previous_move.is_king_castle());
        restore_state(undo_info);
        return;
    }

    // Get the from and to idxs
    // Things get a bit tricky here because the move is being unmade, and so
    // we are "moving to" the from square and "moving from" the to square
    sq_idx from_sq = previous_move.get_from_sq();
    sq_idx to_sq   = previous_move.get_to_sq();

    // Determine the piece type of the piece that was previously moved,
    // takes into consideration if the move was a promotion
    model::Piece::Type  previously_moved_piece_type = determine_moved_piece_type(previous_move, was_w, to_sq);

    // We do the move in reverse, so now we pick up the previously moved piece
    remove_previously_moved_piece_from_board(previous_move, to_sq, previously_moved_piece_type, was_w);

    // We place back the captured piece if there was one
    if (previous_move.is_any_capture()) {
       // Calculate the index of the previously captured piece, might be EP
        sq_idx capture_sq = utils::determine_capture_sq(previous_move, was_w);

        place_back_captured_piece_on_board(previous_move.is_ep_capture(), capture_sq, to_sq, was_w, undo_info.captured_piece_type);
    } else {
        // If there was no capture, we place back an empty square on the to square
        pos_.piece_map.set_piece_type_at(to_sq, model::Piece::Type::EMPTY);
    }

    // Place the moved piece back on the from square
    place_back_moved_piece_on_board(was_w, from_sq, previously_moved_piece_type);

    pos_.occ_masks.update_occupancy_masks();

    restore_state(undo_info);
}

} // namespace logic