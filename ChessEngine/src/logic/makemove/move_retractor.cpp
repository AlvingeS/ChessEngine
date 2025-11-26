#include "logic/makemove/move_retractor.h"

#include "model/position/board.h"
#include "model/move/move.h"

namespace logic {

MoveRetractor::MoveRetractor(
    model::Board& board
) : bbs_(board.bbs), 
    occupancy_masks_(board.occupancy_masks), 
    piece_map_(board.piece_map), 
    z_hasher_(board.z_hasher)
{}

void MoveRetractor::unmake_castle_move(bool was_w, bool wasKingSide)
{
    sq_idx king_from_sq, king_to_sq, rook_from_sq, rook_to_sq;

    if (was_w) {
        king_from_sq = 3;
        king_to_sq   = wasKingSide ? 1 : 5;
        rook_from_sq = wasKingSide ? 0 : 7;
        rook_to_sq = wasKingSide ? 2 : 4;
        
        bbs_.clear_w_king_bit(king_to_sq);
        bbs_.set_w_king_bit(king_from_sq);
        bbs_.clear_w_rooks_bit(rook_to_sq);
        bbs_.set_w_rooks_bit(rook_from_sq);

        occupancy_masks_.set_w_pieces_bit(king_from_sq);
        occupancy_masks_.clear_w_pieces_bit(king_to_sq);
        occupancy_masks_.set_w_pieces_bit(rook_from_sq);
        occupancy_masks_.clear_w_pieces_bit(rook_to_sq);

        piece_map_.set_piece_type_at(king_from_sq, model::Piece::Type::W_KING);
        piece_map_.set_piece_type_at(king_to_sq, model::Piece::Type::EMPTY);
        piece_map_.set_piece_type_at(rook_from_sq, model::Piece::Type::W_ROOK);
        piece_map_.set_piece_type_at(rook_to_sq, model::Piece::Type::EMPTY);
    } else {
        king_from_sq = 59;
        king_to_sq = wasKingSide ? 57 : 61;
        rook_from_sq = wasKingSide ? 56 : 63;
        rook_to_sq = wasKingSide ? 58 : 60;

        bbs_.set_b_king_bit(king_from_sq);
        bbs_.clear_b_king_bit(king_to_sq);
        bbs_.set_b_rooks_bit(rook_from_sq);
        bbs_.clear_b_rooks_bit(rook_to_sq);

        occupancy_masks_.set_b_pieces_bit(king_from_sq);
        occupancy_masks_.clear_b_pieces_bit(king_to_sq);
        occupancy_masks_.set_b_pieces_bit(rook_from_sq);
        occupancy_masks_.clear_b_pieces_bit(rook_to_sq);

        piece_map_.set_piece_type_at(king_to_sq, model::Piece::Type::EMPTY);
        piece_map_.set_piece_type_at(king_from_sq, model::Piece::Type::B_KING);
        piece_map_.set_piece_type_at(rook_to_sq, model::Piece::Type::EMPTY);
        piece_map_.set_piece_type_at(rook_from_sq, model::Piece::Type::B_ROOK);
    }

    occupancy_masks_.update_occupancy_masks();
}

void MoveRetractor::revert_temporary_king_move(bool was_w, bool is_kside) 
{
    sq_idx from_sq = is_kside ? (was_w ? 2 : 58) 
                              : (was_w ? 4 : 60);

    sq_idx to_sq = was_w ? 3 : 59;

    if (was_w) {
        bbs_.clear_w_king_bit(from_sq);
        bbs_.set_w_king_bit(to_sq);
    } else {
        bbs_.clear_b_king_bit(from_sq);
        bbs_.set_b_king_bit(to_sq);
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
        bbs_.clear_piece_type_bit(to_sq, previously_moved_piece_type);
    } else {
        model::Piece::Type promotionPieceType = utils::get_promotion_piece_type(move.get_flag(), was_w);
        bbs_.clear_piece_type_bit(to_sq, promotionPieceType);
    }

    was_w ? occupancy_masks_.clear_w_pieces_bit(to_sq) 
          : occupancy_masks_.clear_b_pieces_bit(to_sq);
}


void MoveRetractor::place_back_captured_piece_on_board(
    bool is_ep,
    sq_idx capture_sq,
    sq_idx to_sq,
    bool was_w,
    model::Piece::Type previously_captured_piece_type) 
{
    bbs_.set_piece_type_bit(capture_sq, previously_captured_piece_type);
    piece_map_.set_piece_type_at(capture_sq, previously_captured_piece_type);

    // If the move was an ep capture, the to square will be empty
    if (is_ep) {
        piece_map_.set_piece_type_at(to_sq, model::Piece::Type::EMPTY);
    }

    was_w ? occupancy_masks_.set_b_pieces_bit(capture_sq) 
          : occupancy_masks_.set_w_pieces_bit(capture_sq);
}

void MoveRetractor::place_back_moved_piece_on_board(
    bool was_w, 
    sq_idx from_sq, 
    model::Piece::Type  moved_piece_type)
{
    bbs_.set_piece_type_bit(from_sq, moved_piece_type);
    piece_map_.set_piece_type_at(from_sq, moved_piece_type);

    was_w ? occupancy_masks_.set_w_pieces_bit(from_sq) 
          : occupancy_masks_.set_b_pieces_bit(from_sq);
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
                                                      : piece_map_.get_piece_type_at(to_sq);

    return previously_moved_piece_type;
}

void MoveRetractor::unmake_move(
    const model::Move& previous_move, 
    bool was_w, 
    utils::MoveResult previous_move_result) 
{
    // If the move is a castle, update the bbs and return
    if (previous_move.is_any_castle()) {
        unmake_castle_move(was_w, previous_move.is_king_castle());
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

        place_back_captured_piece_on_board(previous_move.is_ep_capture(), capture_sq, to_sq, was_w, previous_move_result.captured_piece_type);
    } else {
        // If there was no capture, we place back an empty square on the to square
        piece_map_.set_piece_type_at(to_sq, model::Piece::Type::EMPTY);
    }

    // Place the moved piece back on the from square
    place_back_moved_piece_on_board(was_w, from_sq, previously_moved_piece_type);

    occupancy_masks_.update_occupancy_masks();
}

} // namespace logic