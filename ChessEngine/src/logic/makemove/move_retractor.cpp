#include "logic/makemove/move_retractor.h"

#include "model/position/board.h"
#include "model/move/move.h"

namespace logic {

MoveRetractor::MoveRetractor(
    model::Board& board
) : bitboards_(board.bitboards), 
    state_bitmasks_(board.occupancy_masks), 
    piece_map_(board.piece_map), 
    z_hasher_(board.z_hasher)
{}

void MoveRetractor::unmake_castle_move(bool was_w, bool wasKingSide)
{
    int from_king_sq_idx, to_king_sq_idx, from_rook_sq_idx, to_rook_sq_idx;

    if (was_w) {
        from_king_sq_idx = 3;
        to_king_sq_idx = wasKingSide ? 1 : 5;
        from_rook_sq_idx = wasKingSide ? 0 : 7;
        to_rook_sq_idx = wasKingSide ? 2 : 4;
        
        bitboards_.clear_w_king_bit(to_king_sq_idx);
        bitboards_.set_w_king_bit(from_king_sq_idx);
        bitboards_.clear_w_rooks_bit(to_rook_sq_idx);
        bitboards_.set_w_rooks_bit(from_rook_sq_idx);

        state_bitmasks_.set_w_pieces_bit(from_king_sq_idx);
        state_bitmasks_.clear_w_pieces_bit(to_king_sq_idx);
        state_bitmasks_.set_w_pieces_bit(from_rook_sq_idx);
        state_bitmasks_.clear_w_pieces_bit(to_rook_sq_idx);

        piece_map_.set_piece_type_at_index(from_king_sq_idx, model::Piece::Type::W_KING);
        piece_map_.set_piece_type_at_index(to_king_sq_idx, model::Piece::Type::EMPTY);
        piece_map_.set_piece_type_at_index(from_rook_sq_idx, model::Piece::Type::W_ROOK);
        piece_map_.set_piece_type_at_index(to_rook_sq_idx, model::Piece::Type::EMPTY);
    } else {
        from_king_sq_idx = 59;
        to_king_sq_idx = wasKingSide ? 57 : 61;
        from_rook_sq_idx = wasKingSide ? 56 : 63;
        to_rook_sq_idx = wasKingSide ? 58 : 60;

        bitboards_.set_b_king_bit(from_king_sq_idx);
        bitboards_.clear_b_king_bit(to_king_sq_idx);
        bitboards_.set_b_rooks_bit(from_rook_sq_idx);
        bitboards_.clear_b_rooks_bit(to_rook_sq_idx);

        state_bitmasks_.set_b_pieces_bit(from_king_sq_idx);
        state_bitmasks_.clear_b_pieces_bit(to_king_sq_idx);
        state_bitmasks_.set_b_pieces_bit(from_rook_sq_idx);
        state_bitmasks_.clear_b_pieces_bit(to_rook_sq_idx);

        piece_map_.set_piece_type_at_index(to_king_sq_idx, model::Piece::Type::EMPTY);
        piece_map_.set_piece_type_at_index(from_king_sq_idx, model::Piece::Type::B_KING);
        piece_map_.set_piece_type_at_index(to_rook_sq_idx, model::Piece::Type::EMPTY);
        piece_map_.set_piece_type_at_index(from_rook_sq_idx, model::Piece::Type::B_ROOK);
    }

    state_bitmasks_.update_occupancy_masks();
}

void MoveRetractor::revert_temporary_king_move(bool was_w, bool is_kside) 
{
    int from = is_kside ? (was_w ? 2 : 58) 
                          : (was_w ? 4 : 60);

    int to = was_w ? 3 : 59;

    if (was_w) {
        bitboards_.clear_w_king_bit(from);
        bitboards_.set_w_king_bit(to);
    } else {
        bitboards_.clear_b_king_bit(from);
        bitboards_.set_b_king_bit(to);
    }
}

void MoveRetractor::remove_previously_moved_piece_from_board(
    const model::Move& move,
    int to_sq_idx,
    model::Piece::Type previously_moved_piece_type,
    bool was_w) 
{
    // Square lookup is dependent on if there was a capture or promotion,
    // handled by the place_back_captured_piece_on_board method 

    // If the move was not a promotion, remove the piece in the bitboard
    // Else, remove the bit for the promoted piece
    if (not move.is_any_promo()) {
        bitboards_.clear_piece_type_bit(to_sq_idx, previously_moved_piece_type);
    } else {
        model::Piece::Type promotionPieceType = MoveUtils::get_promotion_piece_type(move.get_flag(), was_w);
        bitboards_.clear_piece_type_bit(to_sq_idx, promotionPieceType);
    }

    was_w ? state_bitmasks_.clear_w_pieces_bit(to_sq_idx) 
             : state_bitmasks_.clear_b_pieces_bit(to_sq_idx);
}


void MoveRetractor::place_back_captured_piece_on_board(
    bool is_ep,
    int capture_sq_idx,
    int to_sq_idx,
    bool was_w,
    model::Piece::Type previously_captured_piece_type) 
{
    bitboards_.set_piece_type_bit(capture_sq_idx, previously_captured_piece_type);
    piece_map_.set_piece_type_at_index(capture_sq_idx, previously_captured_piece_type);

    // If the move was an ep capture, the to square will be empty
    if (is_ep) {
        piece_map_.set_piece_type_at_index(to_sq_idx, model::Piece::Type::EMPTY);
    }

    was_w ? state_bitmasks_.set_b_pieces_bit(capture_sq_idx) 
             : state_bitmasks_.set_w_pieces_bit(capture_sq_idx);
}

void MoveRetractor::place_back_moved_piece_on_board(
    bool was_w, 
    int from_sq_idx, 
    model::Piece::Type  moved_piece_type)
{
    bitboards_.set_piece_type_bit(from_sq_idx, moved_piece_type);
    piece_map_.set_piece_type_at_index(from_sq_idx, moved_piece_type);

    was_w ? state_bitmasks_.set_w_pieces_bit(from_sq_idx) 
             : state_bitmasks_.set_b_pieces_bit(from_sq_idx);
}

model::Piece::Type MoveRetractor::determine_moved_piece_type(
    const model::Move& move, 
    bool was_w,
    int to_sq_idx) const
{
    // Piece type of piece being moved
    model::Piece::Type  previously_moved_piece_type;

    // If the move was a promotion, set the moved piece to a pawn of the same color
    // Else, set the moved piece to the piece occupying the to square
    previously_moved_piece_type = move.is_any_promo() ? (was_w ? model::Piece::Type::W_PAWN
                                                             : model::Piece::Type::B_PAWN)
                                                 : piece_map_.get_piece_type_at_index(to_sq_idx);

    return previously_moved_piece_type;
}

void MoveRetractor::unmake_move(
    const model::Move& previousMove, 
    bool was_w, 
    MoveResult previousMoveResults) 
{
    // If the move is a castle, update the bitboards and return
    if (previousMove.is_any_castle()) {
        unmake_castle_move(was_w, previousMove.is_king_castle());
        return;
    }

    // Get the from and to indices
    // Things get a bit tricky here because the move is being unmade, and so
    // we are "moving to" the from square and "moving from" the to square
    int from_sq_idx = previousMove.get_bit_index_from();
    int to_sq_idx = previousMove.get_bit_index_to();

    // Determine the piece type of the piece that was previously moved,
    // takes into consideration if the move was a promotion
    model::Piece::Type  previously_moved_piece_type = determine_moved_piece_type(previousMove, was_w, to_sq_idx);

    // We do the move in reverse, so now we pick up the previously moved piece
    remove_previously_moved_piece_from_board(previousMove, to_sq_idx, previously_moved_piece_type, was_w);

    // We place back the captured piece if there was one
    if (previousMove.is_any_capture()) {
       // Calculate the index of the previously captured piece, might be EP
        int capture_sq_idx = MoveUtils::determine_capture_sq_idx(previousMove, was_w, to_sq_idx);

        place_back_captured_piece_on_board(previousMove.is_ep_capture(), capture_sq_idx, to_sq_idx, was_w, previousMoveResults.captured_piece_type);
    } else {
        // If there was no capture, we place back an empty square on the to square
        piece_map_.set_piece_type_at_index(to_sq_idx, model::Piece::Type::EMPTY);
    }

    // Place the moved piece back on the from square
    place_back_moved_piece_on_board(was_w, from_sq_idx, previously_moved_piece_type);

    state_bitmasks_.update_occupancy_masks();
}

} // namespace logic