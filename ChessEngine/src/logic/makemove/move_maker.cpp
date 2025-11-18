#include "logic/makemove/move_maker.h"

#include "logic/makemove/move_utils.h"

#include "model/position/board.h"
#include "model/move/move.h"

namespace logic {

MoveMaker::MoveMaker(model::Board& board)
    : bitboards_(board.bitboards) 
    , state_bitmasks_(board.state_bitmasks)
    , piece_map_(board.piece_map)
    , _zHasher(board.z_hasher)
{}

MoveResult MoveMaker::makeMove(const model::Move& move, bool is_w)
{
    auto moveResult = MoveResult();

    // If the move is a castle, update and return
    if (move.is_any_castle()) {
        makeCastleMove(is_w, move.is_king_castle());
        return moveResult;
    }

    // Get the from and to indices
    int fromIndex = move.get_bit_index_from();
    int toIndex = move.get_bit_index_to();

    // Pick up the piece from the from square and get the moved piece type
    model::Piece::Type movedPieceType = removeMovedPieceFromBoard(is_w, fromIndex);

    // If the move is a capture, handle memory and remove the captured piece
    if (move.is_any_capture()) {
        // Calculate index of captured piece, might be EP
        int captureIndex = MoveUtils::determineCaptureIndex(move, is_w, toIndex);
        model::Piece::Type capturedPieceType = piece_map_.get_piece_type_at_index(captureIndex);
        removeCapturedPieceFromBoard(move.is_ep_capture(), is_w, captureIndex, capturedPieceType);
        moveResult.capturedPieceType = capturedPieceType;
    }

    // Update the moved piece type if the move is a promotion    
    if (move.is_any_promo())
        movedPieceType = MoveUtils::getPromotionPieceType(move.get_flag(), is_w);

    moveResult.movedPieceType = movedPieceType;

    // Place the moved piece on the to square
    placeMovedPieceOnBoard(is_w, toIndex, movedPieceType);

    // Update occupied and empty squares bitmasks
    state_bitmasks_.update_occupied_and_empty_squares_bitmasks();

    return moveResult;
}

void MoveMaker::makeCastleMove(bool is_w, bool is_kside)
{
    int fromKingInd, toKingInd, fromRookInd, toRookInd;

    if (is_w) {
        fromKingInd = 3;
        toKingInd = is_kside ? 1 : 5;
        fromRookInd = is_kside ? 0 : 7;
        toRookInd = is_kside ? 2 : 4;

        bitboards_.clear_w_king_bit(fromKingInd);
        bitboards_.set_w_king_bit(toKingInd);
        bitboards_.clear_w_rooks_bit(fromRookInd);
        bitboards_.set_w_rooks_bit(toRookInd);

        state_bitmasks_.clear_w_pieces_bit(fromKingInd);
        state_bitmasks_.set_w_pieces_bit(toKingInd);
        state_bitmasks_.clear_w_pieces_bit(fromRookInd);
        state_bitmasks_.set_w_pieces_bit(toRookInd);

        piece_map_.set_piece_type_at_index(fromKingInd, model::Piece::Type::EMPTY);
        piece_map_.set_piece_type_at_index(toKingInd, model::Piece::Type::W_KING);
        piece_map_.set_piece_type_at_index(fromRookInd, model::Piece::Type::EMPTY);
        piece_map_.set_piece_type_at_index(toRookInd, model::Piece::Type::W_ROOK);
    } else {
        fromKingInd = 59;
        toKingInd = is_kside ? 57 : 61;
        fromRookInd = is_kside ? 56 : 63;
        toRookInd = is_kside ? 58 : 60;

        bitboards_.clear_b_king_bit(fromKingInd);
        bitboards_.set_b_king_bit(toKingInd);
        bitboards_.clear_b_rooks_bit(fromRookInd);
        bitboards_.set_b_rooks_bit(toRookInd);

        state_bitmasks_.clear_b_pieces_bit(fromKingInd);
        state_bitmasks_.set_b_pieces_bit(toKingInd);
        state_bitmasks_.clear_b_pieces_bit(fromRookInd);
        state_bitmasks_.set_b_pieces_bit(toRookInd);

        piece_map_.set_piece_type_at_index(fromKingInd, model::Piece::Type::EMPTY);
        piece_map_.set_piece_type_at_index(toKingInd, model::Piece::Type::B_KING);
        piece_map_.set_piece_type_at_index(fromRookInd, model::Piece::Type::EMPTY);
        piece_map_.set_piece_type_at_index(toRookInd, model::Piece::Type::B_ROOK);
    }

    state_bitmasks_.update_occupied_and_empty_squares_bitmasks();
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


model::Piece::Type MoveMaker::removeMovedPieceFromBoard(bool is_w, int fromIndex) 
{
    // Determine the piece type of the piece being moved
    model::Piece::Type  movedPieceType = piece_map_.get_piece_type_at_index(fromIndex);

    // Update zobrist hash
    _zHasher.hash_square_piece_type(fromIndex, movedPieceType);

    // Clear the piece from bitboards, squarelookup and gamestate bitmasks
    bitboards_.clear_piece_type_bit(fromIndex, movedPieceType);
    piece_map_.set_piece_type_at_index(fromIndex, model::Piece::Type::EMPTY);

    is_w ? state_bitmasks_.clear_w_pieces_bit(fromIndex) 
            : state_bitmasks_.clear_b_pieces_bit(fromIndex);

    return movedPieceType;
}

void MoveMaker::placeMovedPieceOnBoard(
    bool is_w, 
    int toIndex, 
    model::Piece::Type movedPieceType) 
{
    bitboards_.set_piece_type_bit(toIndex, movedPieceType);
    piece_map_.set_piece_type_at_index(toIndex, movedPieceType);

    _zHasher.hash_square_piece_type(toIndex, movedPieceType);

    is_w ? state_bitmasks_.set_w_pieces_bit(toIndex) 
            : state_bitmasks_.set_b_pieces_bit(toIndex);
}

void MoveMaker::removeCapturedPieceFromBoard(bool isEP, bool is_w, int captureIndex, model::Piece::Type  capturedPieceType) {
    // Remove captured piece from board models
    bitboards_.clear_piece_type_bit(captureIndex, capturedPieceType);

    is_w ? state_bitmasks_.clear_b_pieces_bit(captureIndex) 
            : state_bitmasks_.clear_w_pieces_bit(captureIndex);

    _zHasher.hash_square_piece_type(captureIndex, capturedPieceType);

    // Only clear from the squares lookup if the move was an ep capture
    // because the capture index points to the square where the pawn was
    // and is now empty, the square we moved to will have been updated
    if (isEP) {
        piece_map_.set_piece_type_at_index(captureIndex, model::Piece::Type::EMPTY);
    }
}

} // namespace logic