#include "logic/makemove/move_maker.h"

#include "logic/makemove/move_utils.h"

#include "model/position/board.h"
#include "model/move/move.h"

namespace logic {

MoveMaker::MoveMaker(model::Board& board)
    : _bitboards(board.bitboards) 
    , _stateBitmasks(board.state_bitmasks)
    , piece_map_(board.piece_map)
    , _zHasher(board.z_hasher)
{}

MoveResult MoveMaker::makeMove(const model::Move& move, bool isWhite)
{
    auto moveResult = MoveResult();

    // If the move is a castle, update and return
    if (move.is_any_castle()) {
        makeCastleMove(isWhite, move.is_king_castle());
        return moveResult;
    }

    // Get the from and to indices
    int fromIndex = move.get_bit_index_from();
    int toIndex = move.get_bit_index_to();

    // Pick up the piece from the from square and get the moved piece type
    model::Piece::Type movedPieceType = removeMovedPieceFromBoard(isWhite, fromIndex);

    // If the move is a capture, handle memory and remove the captured piece
    if (move.is_any_capture()) {
        // Calculate index of captured piece, might be EP
        int captureIndex = MoveUtils::determineCaptureIndex(move, isWhite, toIndex);
        model::Piece::Type capturedPieceType = piece_map_.get_piece_type_at_index(captureIndex);
        removeCapturedPieceFromBoard(move.is_ep_capture(), isWhite, captureIndex, capturedPieceType);
        moveResult.capturedPieceType = capturedPieceType;
    }

    // Update the moved piece type if the move is a promotion    
    if (move.is_any_promo())
        movedPieceType = MoveUtils::getPromotionPieceType(move.get_flag(), isWhite);

    moveResult.movedPieceType = movedPieceType;

    // Place the moved piece on the to square
    placeMovedPieceOnBoard(isWhite, toIndex, movedPieceType);

    // Update occupied and empty squares bitmasks
    _stateBitmasks.update_occupied_and_empty_squares_bitmasks();

    return moveResult;
}

void MoveMaker::makeCastleMove(bool isWhite, bool isKingSide)
{
    int fromKingInd, toKingInd, fromRookInd, toRookInd;

    if (isWhite) {
        fromKingInd = 3;
        toKingInd = isKingSide ? 1 : 5;
        fromRookInd = isKingSide ? 0 : 7;
        toRookInd = isKingSide ? 2 : 4;

        _bitboards.clear_w_king_bit(fromKingInd);
        _bitboards.set_w_king_bit(toKingInd);
        _bitboards.clear_w_rooks_bit(fromRookInd);
        _bitboards.set_w_rooks_bit(toRookInd);

        _stateBitmasks.clear_w_pieces_bit(fromKingInd);
        _stateBitmasks.set_w_pieces_bit(toKingInd);
        _stateBitmasks.clear_w_pieces_bit(fromRookInd);
        _stateBitmasks.set_w_pieces_bit(toRookInd);

        piece_map_.set_piece_type_at_index(fromKingInd, model::Piece::Type::EMPTY);
        piece_map_.set_piece_type_at_index(toKingInd, model::Piece::Type::W_KING);
        piece_map_.set_piece_type_at_index(fromRookInd, model::Piece::Type::EMPTY);
        piece_map_.set_piece_type_at_index(toRookInd, model::Piece::Type::W_ROOK);
    } else {
        fromKingInd = 59;
        toKingInd = isKingSide ? 57 : 61;
        fromRookInd = isKingSide ? 56 : 63;
        toRookInd = isKingSide ? 58 : 60;

        _bitboards.clear_b_king_bit(fromKingInd);
        _bitboards.set_b_king_bit(toKingInd);
        _bitboards.clear_b_rooks_bit(fromRookInd);
        _bitboards.set_b_rooks_bit(toRookInd);

        _stateBitmasks.clear_b_pieces_bit(fromKingInd);
        _stateBitmasks.set_b_pieces_bit(toKingInd);
        _stateBitmasks.clear_b_pieces_bit(fromRookInd);
        _stateBitmasks.set_b_pieces_bit(toRookInd);

        piece_map_.set_piece_type_at_index(fromKingInd, model::Piece::Type::EMPTY);
        piece_map_.set_piece_type_at_index(toKingInd, model::Piece::Type::B_KING);
        piece_map_.set_piece_type_at_index(fromRookInd, model::Piece::Type::EMPTY);
        piece_map_.set_piece_type_at_index(toRookInd, model::Piece::Type::B_ROOK);
    }

    _stateBitmasks.update_occupied_and_empty_squares_bitmasks();
}

void MoveMaker::makeTemporaryKingMove(bool isWhite, bool isKingSide)
{
    int from = isWhite ? 3 : 59;

    int to = isKingSide ? (isWhite ? 2 : 58)
                        : (isWhite ? 4 : 60);

    if (isWhite) {
        _bitboards.clear_w_king_bit(from);
        _bitboards.set_w_king_bit(to);
    } else {
        _bitboards.clear_b_king_bit(from);
        _bitboards.set_b_king_bit(to);
    }
}


model::Piece::Type MoveMaker::removeMovedPieceFromBoard(bool isWhite, int fromIndex) 
{
    // Determine the piece type of the piece being moved
    model::Piece::Type  movedPieceType = piece_map_.get_piece_type_at_index(fromIndex);

    // Update zobrist hash
    _zHasher.hash_square_piece_type(fromIndex, movedPieceType);

    // Clear the piece from bitboards, squarelookup and gamestate bitmasks
    _bitboards.clear_piece_type_bit(fromIndex, movedPieceType);
    piece_map_.set_piece_type_at_index(fromIndex, model::Piece::Type::EMPTY);

    isWhite ? _stateBitmasks.clear_w_pieces_bit(fromIndex) 
            : _stateBitmasks.clear_b_pieces_bit(fromIndex);

    return movedPieceType;
}

void MoveMaker::placeMovedPieceOnBoard(
    bool isWhite, 
    int toIndex, 
    model::Piece::Type movedPieceType) 
{
    _bitboards.set_piece_type_bit(toIndex, movedPieceType);
    piece_map_.set_piece_type_at_index(toIndex, movedPieceType);

    _zHasher.hash_square_piece_type(toIndex, movedPieceType);

    isWhite ? _stateBitmasks.set_w_pieces_bit(toIndex) 
            : _stateBitmasks.set_b_pieces_bit(toIndex);
}

void MoveMaker::removeCapturedPieceFromBoard(bool isEP, bool isWhite, int captureIndex, model::Piece::Type  capturedPieceType) {
    // Remove captured piece from board models
    _bitboards.clear_piece_type_bit(captureIndex, capturedPieceType);

    isWhite ? _stateBitmasks.clear_b_pieces_bit(captureIndex) 
            : _stateBitmasks.clear_w_pieces_bit(captureIndex);

    _zHasher.hash_square_piece_type(captureIndex, capturedPieceType);

    // Only clear from the squares lookup if the move was an ep capture
    // because the capture index points to the square where the pawn was
    // and is now empty, the square we moved to will have been updated
    if (isEP) {
        piece_map_.set_piece_type_at_index(captureIndex, model::Piece::Type::EMPTY);
    }
}

} // namespace logic