#include "logic/makemove/move_maker.h"

#include "logic/makemove/move_utils.h"

#include "model/position/board.h"
#include "model/move/move.h"

namespace logic {

MoveMaker::MoveMaker(model::Board& board)
    : bitboards_(board.bitboards) 
    , occupancy_masks_(board.occupancy_masks)
    , piece_map_(board.piece_map)
    , z_hasher_(board.z_hasher)
{}

MoveResult MoveMaker::make_move(const model::Move& move, bool is_w)
{
    auto move_result = MoveResult();

    // If the move is a castle, update and return
    if (move.is_any_castle()) {
        make_castle_move(is_w, move.is_king_castle());
        return move_result;
    }

    // Get the from and to indices
    int from_sq_idx = move.get_bit_idx_from();
    int to_sq_idx = move.get_bit_idx_to();

    // Pick up the piece from the from square and get the moved piece type
    model::Piece::Type moved_piece_type = remove_moved_piece_from_board(is_w, from_sq_idx);

    // If the move is a capture, handle memory and remove the captured piece
    if (move.is_any_capture()) {
        // Calculate idx of captured piece, might be EP
        int capture_dq_idx = MoveUtils::determine_capture_sq_idx(move, is_w, to_sq_idx);
        model::Piece::Type captured_piece_type = piece_map_.get_piece_type_at_idx(capture_dq_idx);
        remove_captured_piece_from_board(move.is_ep_capture(), is_w, capture_dq_idx, captured_piece_type);
        move_result.captured_piece_type = captured_piece_type;
    }

    // Update the moved piece type if the move is a promotion    
    if (move.is_any_promo())
        moved_piece_type = MoveUtils::get_promotion_piece_type(move.get_flag(), is_w);

    move_result.moved_piece_type = moved_piece_type;

    // Place the moved piece on the to square
    place_moved_piece_on_board(is_w, to_sq_idx, moved_piece_type);

    // Update occupied and empty squares bitmasks
    occupancy_masks_.update_occupancy_masks();

    return move_result;
}

void MoveMaker::make_castle_move(bool is_w, bool is_kside)
{
    int from_king_sq_idx, to_king_sq_idx, from_rook_sq_idx, to_rook_sq_idx;

    if (is_w) {
        from_king_sq_idx = 3;
        to_king_sq_idx = is_kside ? 1 : 5;
        from_rook_sq_idx = is_kside ? 0 : 7;
        to_rook_sq_idx = is_kside ? 2 : 4;

        bitboards_.clear_w_king_bit(from_king_sq_idx);
        bitboards_.set_w_king_bit(to_king_sq_idx);
        bitboards_.clear_w_rooks_bit(from_rook_sq_idx);
        bitboards_.set_w_rooks_bit(to_rook_sq_idx);

        occupancy_masks_.clear_w_pieces_bit(from_king_sq_idx);
        occupancy_masks_.set_w_pieces_bit(to_king_sq_idx);
        occupancy_masks_.clear_w_pieces_bit(from_rook_sq_idx);
        occupancy_masks_.set_w_pieces_bit(to_rook_sq_idx);

        piece_map_.set_piece_type_at_idx(from_king_sq_idx, model::Piece::Type::EMPTY);
        piece_map_.set_piece_type_at_idx(to_king_sq_idx, model::Piece::Type::W_KING);
        piece_map_.set_piece_type_at_idx(from_rook_sq_idx, model::Piece::Type::EMPTY);
        piece_map_.set_piece_type_at_idx(to_rook_sq_idx, model::Piece::Type::W_ROOK);
    } else {
        from_king_sq_idx = 59;
        to_king_sq_idx = is_kside ? 57 : 61;
        from_rook_sq_idx = is_kside ? 56 : 63;
        to_rook_sq_idx = is_kside ? 58 : 60;

        bitboards_.clear_b_king_bit(from_king_sq_idx);
        bitboards_.set_b_king_bit(to_king_sq_idx);
        bitboards_.clear_b_rooks_bit(from_rook_sq_idx);
        bitboards_.set_b_rooks_bit(to_rook_sq_idx);

        occupancy_masks_.clear_b_pieces_bit(from_king_sq_idx);
        occupancy_masks_.set_b_pieces_bit(to_king_sq_idx);
        occupancy_masks_.clear_b_pieces_bit(from_rook_sq_idx);
        occupancy_masks_.set_b_pieces_bit(to_rook_sq_idx);

        piece_map_.set_piece_type_at_idx(from_king_sq_idx, model::Piece::Type::EMPTY);
        piece_map_.set_piece_type_at_idx(to_king_sq_idx, model::Piece::Type::B_KING);
        piece_map_.set_piece_type_at_idx(from_rook_sq_idx, model::Piece::Type::EMPTY);
        piece_map_.set_piece_type_at_idx(to_rook_sq_idx, model::Piece::Type::B_ROOK);
    }

    occupancy_masks_.update_occupancy_masks();
}

void MoveMaker::make_temporary_king_move(bool is_w, bool is_kside)
{
    int from = is_w ? 3 : 59;

    int to = is_kside ? (is_w ? 2 : 58)
                        : (is_w ? 4 : 60);

    if (is_w) {
        bitboards_.clear_w_king_bit(from);
        bitboards_.set_w_king_bit(to);
    } else {
        bitboards_.clear_b_king_bit(from);
        bitboards_.set_b_king_bit(to);
    }
}


model::Piece::Type MoveMaker::remove_moved_piece_from_board(bool is_w, int from_sq_idx) 
{
    // Determine the piece type of the piece being moved
    model::Piece::Type  moved_piece_type = piece_map_.get_piece_type_at_idx(from_sq_idx);

    // Update zobrist hash
    z_hasher_.hash_square_piece_type(from_sq_idx, moved_piece_type);

    // Clear the piece from bitboards, squarelookup and gamestate bitmasks
    bitboards_.clear_piece_type_bit(from_sq_idx, moved_piece_type);
    piece_map_.set_piece_type_at_idx(from_sq_idx, model::Piece::Type::EMPTY);

    is_w ? occupancy_masks_.clear_w_pieces_bit(from_sq_idx) 
            : occupancy_masks_.clear_b_pieces_bit(from_sq_idx);

    return moved_piece_type;
}

void MoveMaker::place_moved_piece_on_board(
    bool is_w, 
    int to_sq_idx, 
    model::Piece::Type moved_piece_type) 
{
    bitboards_.set_piece_type_bit(to_sq_idx, moved_piece_type);
    piece_map_.set_piece_type_at_idx(to_sq_idx, moved_piece_type);

    z_hasher_.hash_square_piece_type(to_sq_idx, moved_piece_type);

    is_w ? occupancy_masks_.set_w_pieces_bit(to_sq_idx) 
            : occupancy_masks_.set_b_pieces_bit(to_sq_idx);
}

void MoveMaker::remove_captured_piece_from_board(bool is_ep, bool is_w, int capture_dq_idx, model::Piece::Type  captured_piece_type) {
    // Remove captured piece from board models
    bitboards_.clear_piece_type_bit(capture_dq_idx, captured_piece_type);

    is_w ? occupancy_masks_.clear_b_pieces_bit(capture_dq_idx) 
            : occupancy_masks_.clear_w_pieces_bit(capture_dq_idx);

    z_hasher_.hash_square_piece_type(capture_dq_idx, captured_piece_type);

    // Only clear from the squares lookup if the move was an ep capture
    // because the capture idx points to the square where the pawn was
    // and is now empty, the square we moved to will have been updated
    if (is_ep) {
        piece_map_.set_piece_type_at_idx(capture_dq_idx, model::Piece::Type::EMPTY);
    }
}

} // namespace logic