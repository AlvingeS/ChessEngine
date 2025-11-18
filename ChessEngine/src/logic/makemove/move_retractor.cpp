#include "logic/makemove/move_retractor.h"

#include "model/position/board.h"
#include "model/move/move.h"

namespace logic {

MoveRetractor::MoveRetractor(
    model::Board& board
) : _bitboards(board.bitboards), 
    _stateBitmasks(board.state_bitmasks), 
    piece_map_(board.piece_map), 
    _zHasher(board.z_hasher)
{}

void MoveRetractor::unmakeCastleMove(bool wasWhite, bool wasKingSide)
{
    int fromKingInd, toKingInd, fromRookInd, toRookInd;

    if (wasWhite) {
        fromKingInd = 3;
        toKingInd = wasKingSide ? 1 : 5;
        fromRookInd = wasKingSide ? 0 : 7;
        toRookInd = wasKingSide ? 2 : 4;
        
        _bitboards.clear_w_king_bit(toKingInd);
        _bitboards.set_w_king_bit(fromKingInd);
        _bitboards.clear_w_rooks_bit(toRookInd);
        _bitboards.set_w_rooks_bit(fromRookInd);

        _stateBitmasks.set_w_pieces_bit(fromKingInd);
        _stateBitmasks.clear_w_pieces_bit(toKingInd);
        _stateBitmasks.set_w_pieces_bit(fromRookInd);
        _stateBitmasks.clear_w_pieces_bit(toRookInd);

        piece_map_.set_piece_type_at_index(fromKingInd, model::Piece::Type::W_KING);
        piece_map_.set_piece_type_at_index(toKingInd, model::Piece::Type::EMPTY);
        piece_map_.set_piece_type_at_index(fromRookInd, model::Piece::Type::W_ROOK);
        piece_map_.set_piece_type_at_index(toRookInd, model::Piece::Type::EMPTY);
    } else {
        fromKingInd = 59;
        toKingInd = wasKingSide ? 57 : 61;
        fromRookInd = wasKingSide ? 56 : 63;
        toRookInd = wasKingSide ? 58 : 60;

        _bitboards.set_b_king_bit(fromKingInd);
        _bitboards.clear_b_king_bit(toKingInd);
        _bitboards.set_b_rooks_bit(fromRookInd);
        _bitboards.clear_b_rooks_bit(toRookInd);

        _stateBitmasks.set_b_pieces_bit(fromKingInd);
        _stateBitmasks.clear_b_pieces_bit(toKingInd);
        _stateBitmasks.set_b_pieces_bit(fromRookInd);
        _stateBitmasks.clear_b_pieces_bit(toRookInd);

        piece_map_.set_piece_type_at_index(toKingInd, model::Piece::Type::EMPTY);
        piece_map_.set_piece_type_at_index(fromKingInd, model::Piece::Type::B_KING);
        piece_map_.set_piece_type_at_index(toRookInd, model::Piece::Type::EMPTY);
        piece_map_.set_piece_type_at_index(fromRookInd, model::Piece::Type::B_ROOK);
    }

    _stateBitmasks.update_occupied_and_empty_squares_bitmasks();
}

void MoveRetractor::unmakeTemporaryKingMove(bool wasWhite, bool isKingSide) 
{
    int from = isKingSide ? (wasWhite ? 2 : 58) 
                          : (wasWhite ? 4 : 60);

    int to = wasWhite ? 3 : 59;

    if (wasWhite) {
        _bitboards.clear_w_king_bit(from);
        _bitboards.set_w_king_bit(to);
    } else {
        _bitboards.clear_b_king_bit(from);
        _bitboards.set_b_king_bit(to);
    }
}

void MoveRetractor::removePreviouslyMovedPieceFromBoard(
    const model::Move& move,
    int toIndex,
    model::Piece::Type previouslyMovedPieceType,
    bool wasWhite) 
{
    // Square lookup is dependent on if there was a capture or promotion,
    // handled by the placeBackCapturedPieceOnBoard method 

    // If the move was not a promotion, remove the piece in the bitboard
    // Else, remove the bit for the promoted piece
    if (not move.isAnyPromo()) {
        _bitboards.clear_piece_type_bit(toIndex, previouslyMovedPieceType);
    } else {
        model::Piece::Type promotionPieceType = MoveUtils::getPromotionPieceType(move.getFlag(), wasWhite);
        _bitboards.clear_piece_type_bit(toIndex, promotionPieceType);
    }

    wasWhite ? _stateBitmasks.clear_w_pieces_bit(toIndex) 
             : _stateBitmasks.clear_b_pieces_bit(toIndex);
}


void MoveRetractor::placeBackCapturedPieceOnBoard(
    bool isEP,
    int captureIndex,
    int toIndex,
    bool wasWhite,
    model::Piece::Type previouslyCapturedPieceType) 
{
    _bitboards.set_piece_type_bit(captureIndex, previouslyCapturedPieceType);
    piece_map_.set_piece_type_at_index(captureIndex, previouslyCapturedPieceType);

    // If the move was an ep capture, the to square will be empty
    if (isEP) {
        piece_map_.set_piece_type_at_index(toIndex, model::Piece::Type::EMPTY);
    }

    wasWhite ? _stateBitmasks.set_b_pieces_bit(captureIndex) 
             : _stateBitmasks.set_w_pieces_bit(captureIndex);
}

void MoveRetractor::placeBackMovedPieceOnBoard(
    bool wasWhite, 
    int fromIndex, 
    model::Piece::Type  movedPieceType)
{
    _bitboards.set_piece_type_bit(fromIndex, movedPieceType);
    piece_map_.set_piece_type_at_index(fromIndex, movedPieceType);

    wasWhite ? _stateBitmasks.set_w_pieces_bit(fromIndex) 
             : _stateBitmasks.set_b_pieces_bit(fromIndex);
}

model::Piece::Type MoveRetractor::determineMovedPieceType(
    const model::Move& move, 
    bool wasWhite,
    int toIndex) const
{
    // Piece type of piece being moved
    model::Piece::Type  previouslyMovedPieceType;

    // If the move was a promotion, set the moved piece to a pawn of the same color
    // Else, set the moved piece to the piece occupying the to square
    previouslyMovedPieceType = move.isAnyPromo() ? (wasWhite ? model::Piece::Type::W_PAWN
                                                             : model::Piece::Type::B_PAWN)
                                                 : piece_map_.get_piece_type_at_index(toIndex);

    return previouslyMovedPieceType;
}

void MoveRetractor::unmakeMove(
    const model::Move& previousMove, 
    bool wasWhite, 
    MoveResult previousMoveResults) 
{
    // If the move is a castle, update the bitboards and return
    if (previousMove.isAnyCastle()) {
        unmakeCastleMove(wasWhite, previousMove.isKingCastle());
        return;
    }

    // Get the from and to indices
    // Things get a bit tricky here because the move is being unmade, and so
    // we are "moving to" the from square and "moving from" the to square
    int fromIndex = previousMove.getBitIndexFrom();
    int toIndex = previousMove.getBitIndexTo();

    // Determine the piece type of the piece that was previously moved,
    // takes into consideration if the move was a promotion
    model::Piece::Type  previouslyMovedPieceType = determineMovedPieceType(previousMove, wasWhite, toIndex);

    // We do the move in reverse, so now we pick up the previously moved piece
    removePreviouslyMovedPieceFromBoard(previousMove, toIndex, previouslyMovedPieceType, wasWhite);

    // We place back the captured piece if there was one
    if (previousMove.isAnyCapture()) {
       // Calculate the index of the previously captured piece, might be EP
        int captureIndex = MoveUtils::determineCaptureIndex(previousMove, wasWhite, toIndex);

        placeBackCapturedPieceOnBoard(previousMove.isEpCapture(), captureIndex, toIndex, wasWhite, previousMoveResults.capturedPieceType);
    } else {
        // If there was no capture, we place back an empty square on the to square
        piece_map_.set_piece_type_at_index(toIndex, model::Piece::Type::EMPTY);
    }

    // Place the moved piece back on the from square
    placeBackMovedPieceOnBoard(wasWhite, fromIndex, previouslyMovedPieceType);

    _stateBitmasks.update_occupied_and_empty_squares_bitmasks();
}

} // namespace logic