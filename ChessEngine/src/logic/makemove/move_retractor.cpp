#include "logic/makemove/move_retractor.h"

#include "logic/utils.h"

#include "model/position/position.h"
#include "model/move/move.h"
#include "model/constants.h"

namespace logic {

MoveRetractor::MoveRetractor(model::Position& pos, ZHasher& z_hasher) 
    : pos_(pos)
    , z_hasher_(z_hasher) 
{}

void MoveRetractor::unmake_castle_move(bool was_kside)
{
    sq_t king_from_sq, king_to_sq, rook_from_sq, rook_to_sq;
    bool was_white = !pos_.is_w;

    if (was_white) {
        king_from_sq = constants::W_KING_START_SQ;
        king_to_sq   = was_kside ? constants::W_KSIDE_KING_CASTLE_TO_SQ : constants::W_QSIDE_KING_CASTLE_TO_SQ;
        rook_from_sq = was_kside ? constants::W_KSIDE_ROOK_START_SQ     : constants::W_QSIDE_ROOK_START_SQ;
        rook_to_sq   = was_kside ? constants::W_KSIDE_ROOK_CASTLE_TO_SQ : constants::W_QSIDE_ROOK_CASTLE_TO_SQ;
        
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
        king_from_sq = constants::B_KING_START_SQ;
        king_to_sq   = was_kside ? constants::B_KSIDE_KING_CASTLE_TO_SQ : constants::B_QSIDE_KING_CASTLE_TO_SQ;
        rook_from_sq = was_kside ? constants::B_KSIDE_ROOK_START_SQ     : constants::B_QSIDE_ROOK_START_SQ;
        rook_to_sq   = was_kside ? constants::B_KSIDE_ROOK_CASTLE_TO_SQ : constants::B_QSIDE_ROOK_CASTLE_TO_SQ;


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

void MoveRetractor::remove_previously_moved_piece_from_board(
    const model::Move& move,
    sq_t to_sq,
    model::Piece::Type previously_moved_piece_type) 
{
    bool was_white = !pos_.is_w;

    // Square lookup is dependent on if there was a capture or promotion,
    // handled by the place_back_captured_piece_on_board method 

    // If the move was not a promotion, remove the piece in the bitboard
    // Else, remove the bit for the promoted piece
    if (not move.is_any_promo()) {
        pos_.bbs.clear_piece_type_bit(to_sq, previously_moved_piece_type);
    } else {
        model::Piece::Type promotionPieceType = utils::get_promotion_piece_type(move.flag(), was_white);
        pos_.bbs.clear_piece_type_bit(to_sq, promotionPieceType);
    }

    was_white ? pos_.occ_masks.clear_w_pieces_bit(to_sq) 
              : pos_.occ_masks.clear_b_pieces_bit(to_sq);
}


void MoveRetractor::place_back_captured_piece_on_board(
    bool is_ep,
    sq_t capture_sq,
    sq_t to_sq,
    model::Piece::Type previously_captured_piece_type) 
{
    bool was_white = !pos_.is_w;

    pos_.bbs.set_piece_type_bit(capture_sq, previously_captured_piece_type);
    pos_.piece_map.set_piece_type_at(capture_sq, previously_captured_piece_type);

    // If the move was an ep capture, the to square will be empty
    if (is_ep) {
        pos_.piece_map.set_piece_type_at(to_sq, model::Piece::Type::EMPTY);
    }

    was_white ? pos_.occ_masks.set_b_pieces_bit(capture_sq) 
              : pos_.occ_masks.set_w_pieces_bit(capture_sq);
}

void MoveRetractor::place_back_moved_piece_on_board(
    sq_t from_sq, 
    model::Piece::Type  moved_piece_type)
{
    bool was_white = !pos_.is_w;

    pos_.bbs.set_piece_type_bit(from_sq, moved_piece_type);
    pos_.piece_map.set_piece_type_at(from_sq, moved_piece_type);

    was_white ? pos_.occ_masks.set_w_pieces_bit(from_sq) 
              : pos_.occ_masks.set_b_pieces_bit(from_sq);
}

model::Piece::Type MoveRetractor::determine_moved_piece_type(
    const model::Move& move, 
    sq_t to_sq) const
{
    bool was_white = !pos_.is_w;

    // Piece type of piece being moved
    model::Piece::Type  previously_moved_piece_type;

    // If the move was a promotion, set the moved piece to a pawn of the same color
    // Else, set the moved piece to the piece occupying the to square
    previously_moved_piece_type = move.is_any_promo() ? (was_white ? model::Piece::Type::W_PAWN
                                                                   : model::Piece::Type::B_PAWN)
                                                      : pos_.piece_map.get_piece_type_at(to_sq);

    return previously_moved_piece_type;
}

void MoveRetractor::restore_state(logic::UndoInfo& undo_info)
{
    pos_.c_rights = undo_info.c_rights;
    pos_.ep_target_mask = undo_info.ep_target_mask;
    z_hasher_.set_z_hash(undo_info.z_hash);
    pos_.is_w = !pos_.is_w;
}

void MoveRetractor::unmake_move(
    const model::Move& previous_move, 
    logic::UndoInfo& undo_info) 
{
    bool was_white = !pos_.is_w;

    // If the move is a castle, update the bbs and return
    if (previous_move.is_any_castle()) {
        unmake_castle_move(previous_move.is_king_castle());
        restore_state(undo_info);
        return;
    }

    // Get the from and to idxs
    // Things get a bit tricky here because the move is being unmade, and so
    // we are "moving to" the from square and "moving from" the to square
    sq_t from_sq = previous_move.from();
    sq_t to_sq   = previous_move.to();

    // Determine the piece type of the piece that was previously moved,
    // takes into consideration if the move was a promotion
    model::Piece::Type  previously_moved_piece_type = determine_moved_piece_type(previous_move, to_sq);

    // We do the move in reverse, so now we pick up the previously moved piece
    remove_previously_moved_piece_from_board(previous_move, to_sq, previously_moved_piece_type);

    // We place back the captured piece if there was one
    if (previous_move.is_any_capture()) {
       // Calculate the index of the previously captured piece, might be EP
        sq_t capture_sq = utils::determine_capture_sq(previous_move, was_white);

        place_back_captured_piece_on_board(previous_move.is_ep_capture(), capture_sq, to_sq, undo_info.captured_piece_type);
    } else {
        // If there was no capture, we place back an empty square on the to square
        pos_.piece_map.set_piece_type_at(to_sq, model::Piece::Type::EMPTY);
    }

    // Place the moved piece back on the from square
    place_back_moved_piece_on_board(from_sq, previously_moved_piece_type);

    pos_.occ_masks.update_occupancy_masks();

    restore_state(undo_info);
}

} // namespace logic