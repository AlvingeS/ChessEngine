#include "logic/makemove/move_maker.h"

#include "logic/makemove/move_utils.h"

#include "model/position/board.h"
#include "model/move/move.h"

namespace logic {

MoveMaker::MoveMaker(model::Board& board)
    : bbs_(board.bbs) 
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

    // Get the from and to idxs
    sq_idx from_sq = move.get_from_sq();
    sq_idx to_sq   = move.get_to_sq();

    // Pick up the piece from the from square and get the moved piece type
    model::Piece::Type moved_piece_type = remove_moved_piece_from_board(is_w, from_sq);

    // If the move is a capture, handle memory and remove the captured piece
    if (move.is_any_capture()) {
        // Calculate sq of captured piece, might be EP
        sq_idx capture_sq = MoveUtils::determine_capture_sq(move, is_w);

        model::Piece::Type captured_piece_type = piece_map_.get_piece_type_at(capture_sq);
        remove_captured_piece_from_board(move.is_ep_capture(), is_w, capture_sq, captured_piece_type);
        
        move_result.captured_piece_type = captured_piece_type;
    }

    // Update the moved piece type if the move is a promotion    
    if (move.is_any_promo())
        moved_piece_type = MoveUtils::get_promotion_piece_type(move.get_flag(), is_w);

    move_result.moved_piece_type = moved_piece_type;

    // Place the moved piece on the to square
    place_moved_piece_on_board(is_w, to_sq, moved_piece_type);

    // Update occupied and empty squares bitmasks
    occupancy_masks_.update_occupancy_masks();

    return move_result;
}

void MoveMaker::make_castle_move(bool is_w, bool is_kside)
{
    sq_idx king_from_sq, king_to_sq, rook_from_sq, rook_to_sq;

    if (is_w) {
        king_from_sq = 3;
        king_to_sq   = is_kside ? 1 : 5;
        rook_from_sq = is_kside ? 0 : 7;
        rook_to_sq   = is_kside ? 2 : 4;

        bbs_.clear_w_king_bit(king_from_sq);
        bbs_.set_w_king_bit(king_to_sq);
        bbs_.clear_w_rooks_bit(rook_from_sq);
        bbs_.set_w_rooks_bit(rook_to_sq);

        occupancy_masks_.clear_w_pieces_bit(king_from_sq);
        occupancy_masks_.set_w_pieces_bit(king_to_sq);
        occupancy_masks_.clear_w_pieces_bit(rook_from_sq);
        occupancy_masks_.set_w_pieces_bit(rook_to_sq);

        piece_map_.set_piece_type_at(king_from_sq, model::Piece::Type::EMPTY);
        piece_map_.set_piece_type_at(king_to_sq,   model::Piece::Type::W_KING);
        piece_map_.set_piece_type_at(rook_from_sq, model::Piece::Type::EMPTY);
        piece_map_.set_piece_type_at(rook_to_sq,   model::Piece::Type::W_ROOK);
    } else {
        king_from_sq = 59;
        king_to_sq   = is_kside ? 57 : 61;
        rook_from_sq = is_kside ? 56 : 63;
        rook_to_sq   = is_kside ? 58 : 60;

        bbs_.clear_b_king_bit(king_from_sq);
        bbs_.set_b_king_bit(king_to_sq);
        bbs_.clear_b_rooks_bit(rook_from_sq);
        bbs_.set_b_rooks_bit(rook_to_sq);

        occupancy_masks_.clear_b_pieces_bit(king_from_sq);
        occupancy_masks_.set_b_pieces_bit(king_to_sq);
        occupancy_masks_.clear_b_pieces_bit(rook_from_sq);
        occupancy_masks_.set_b_pieces_bit(rook_to_sq);

        piece_map_.set_piece_type_at(king_from_sq, model::Piece::Type::EMPTY);
        piece_map_.set_piece_type_at(king_to_sq,   model::Piece::Type::B_KING);
        piece_map_.set_piece_type_at(rook_from_sq, model::Piece::Type::EMPTY);
        piece_map_.set_piece_type_at(rook_to_sq,   model::Piece::Type::B_ROOK);
    }

    occupancy_masks_.update_occupancy_masks();
}

void MoveMaker::make_temporary_king_move(bool is_w, bool is_kside)
{
    sq_idx from_sq = is_w ? 3 : 59;
    sq_idx to_sq   = is_kside ? (is_w ? 2 : 58)
                              : (is_w ? 4 : 60);

    if (is_w) {
        bbs_.clear_w_king_bit(from_sq);
        bbs_.set_w_king_bit(to_sq);
    } else {
        bbs_.clear_b_king_bit(from_sq);
        bbs_.set_b_king_bit(to_sq);
    }
}


model::Piece::Type MoveMaker::remove_moved_piece_from_board(bool is_w, sq_idx from_sq) 
{
    // Determine the piece type of the piece being moved
    model::Piece::Type  moved_piece_type = piece_map_.get_piece_type_at(from_sq);

    // Update zobrist hash
    z_hasher_.hash_piece_type_at(from_sq, moved_piece_type);

    // Clear the piece from bbs, squarelookup and gamestate bitmasks
    bbs_.clear_piece_type_bit(from_sq, moved_piece_type);
    piece_map_.set_piece_type_at(from_sq, model::Piece::Type::EMPTY);

    is_w ? occupancy_masks_.clear_w_pieces_bit(from_sq) 
         : occupancy_masks_.clear_b_pieces_bit(from_sq);

    return moved_piece_type;
}

void MoveMaker::place_moved_piece_on_board(
    bool is_w, 
    sq_idx to_sq, 
    model::Piece::Type moved_piece_type) 
{
    bbs_.set_piece_type_bit(to_sq, moved_piece_type);
    piece_map_.set_piece_type_at(to_sq, moved_piece_type);

    z_hasher_.hash_piece_type_at(to_sq, moved_piece_type);

    is_w ? occupancy_masks_.set_w_pieces_bit(to_sq) 
         : occupancy_masks_.set_b_pieces_bit(to_sq);
}

void MoveMaker::remove_captured_piece_from_board(bool is_ep, bool is_w, sq_idx capture_sq, model::Piece::Type  captured_piece_type) {
    // Remove captured piece from board models
    bbs_.clear_piece_type_bit(capture_sq, captured_piece_type);

    is_w ? occupancy_masks_.clear_b_pieces_bit(capture_sq) 
         : occupancy_masks_.clear_w_pieces_bit(capture_sq);

    z_hasher_.hash_piece_type_at(capture_sq, captured_piece_type);

    // Only clear from the squares lookup if the move was an ep capture
    // because the capture idx points to the square where the pawn was
    // and is now empty, the square we moved to will have been updated
    if (is_ep) {
        piece_map_.set_piece_type_at(capture_sq, model::Piece::Type::EMPTY);
    }
}

} // namespace logic