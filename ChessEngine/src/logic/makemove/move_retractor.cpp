#include "logic/makemove/move_retractor.h"

#include "model/position/board.h"
#include "model/move/move.h"

namespace logic {

MoveRetractor::MoveRetractor(
    model::Board& board
) : bitboards_(board.bitboards), 
    state_bitmasks_(board.state_bitmasks), 
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
        
        bitboards_.clear_w_king_bit(toKingInd);
        bitboards_.set_w_king_bit(fromKingInd);
        bitboards_.clear_w_rooks_bit(toRookInd);
        bitboards_.set_w_rooks_bit(fromRookInd);

        state_bitmasks_.set_w_pieces_bit(fromKingInd);
        state_bitmasks_.clear_w_pieces_bit(toKingInd);
        state_bitmasks_.set_w_pieces_bit(fromRookInd);
        state_bitmasks_.clear_w_pieces_bit(toRookInd);

        piece_map_.set_piece_type_at_index(fromKingInd, model::Piece::Type::W_KING);
        piece_map_.set_piece_type_at_index(toKingInd, model::Piece::Type::EMPTY);
        piece_map_.set_piece_type_at_index(fromRookInd, model::Piece::Type::W_ROOK);
        piece_map_.set_piece_type_at_index(toRookInd, model::Piece::Type::EMPTY);
    } else {
        fromKingInd = 59;
        toKingInd = wasKingSide ? 57 : 61;
        fromRookInd = wasKingSide ? 56 : 63;
        toRookInd = wasKingSide ? 58 : 60;

        bitboards_.set_b_king_bit(fromKingInd);
        bitboards_.clear_b_king_bit(toKingInd);
        bitboards_.set_b_rooks_bit(fromRookInd);
        bitboards_.clear_b_rooks_bit(toRookInd);

        state_bitmasks_.set_b_pieces_bit(fromKingInd);
        state_bitmasks_.clear_b_pieces_bit(toKingInd);
        state_bitmasks_.set_b_pieces_bit(fromRookInd);
        state_bitmasks_.clear_b_pieces_bit(toRookInd);

        piece_map_.set_piece_type_at_index(toKingInd, model::Piece::Type::EMPTY);
        piece_map_.set_piece_type_at_index(fromKingInd, model::Piece::Type::B_KING);
        piece_map_.set_piece_type_at_index(toRookInd, model::Piece::Type::EMPTY);
        piece_map_.set_piece_type_at_index(fromRookInd, model::Piece::Type::B_ROOK);
    }

    state_bitmasks_.update_occupied_and_empty_squares_bitmasks();
}

void MoveRetractor::revert_temporary_king_move(bool wasWhite, bool is_kside) 
{
    int from = is_kside ? (wasWhite ? 2 : 58) 
                          : (wasWhite ? 4 : 60);

    int to = wasWhite ? 3 : 59;

    if (wasWhite) {
        bitboards_.clear_w_king_bit(from);
        bitboards_.set_w_king_bit(to);
    } else {
        bitboards_.clear_b_king_bit(from);
        bitboards_.set_b_king_bit(to);
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
    if (not move.is_any_promo()) {
        bitboards_.clear_piece_type_bit(toIndex, previouslyMovedPieceType);
    } else {
        model::Piece::Type promotionPieceType = MoveUtils::getPromotionPieceType(move.get_flag(), wasWhite);
        bitboards_.clear_piece_type_bit(toIndex, promotionPieceType);
    }

    wasWhite ? state_bitmasks_.clear_w_pieces_bit(toIndex) 
             : state_bitmasks_.clear_b_pieces_bit(toIndex);
}


void MoveRetractor::placeBackCapturedPieceOnBoard(
    bool isEP,
    int captureIndex,
    int toIndex,
    bool wasWhite,
    model::Piece::Type previouslyCapturedPieceType) 
{
    bitboards_.set_piece_type_bit(captureIndex, previouslyCapturedPieceType);
    piece_map_.set_piece_type_at_index(captureIndex, previouslyCapturedPieceType);

    // If the move was an ep capture, the to square will be empty
    if (isEP) {
        piece_map_.set_piece_type_at_index(toIndex, model::Piece::Type::EMPTY);
    }

    wasWhite ? state_bitmasks_.set_b_pieces_bit(captureIndex) 
             : state_bitmasks_.set_w_pieces_bit(captureIndex);
}

void MoveRetractor::placeBackMovedPieceOnBoard(
    bool wasWhite, 
    int fromIndex, 
    model::Piece::Type  movedPieceType)
{
    bitboards_.set_piece_type_bit(fromIndex, movedPieceType);
    piece_map_.set_piece_type_at_index(fromIndex, movedPieceType);

    wasWhite ? state_bitmasks_.set_w_pieces_bit(fromIndex) 
             : state_bitmasks_.set_b_pieces_bit(fromIndex);
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
    previouslyMovedPieceType = move.is_any_promo() ? (wasWhite ? model::Piece::Type::W_PAWN
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
    if (previousMove.is_any_castle()) {
        unmakeCastleMove(wasWhite, previousMove.is_king_castle());
        return;
    }

    // Get the from and to indices
    // Things get a bit tricky here because the move is being unmade, and so
    // we are "moving to" the from square and "moving from" the to square
    int fromIndex = previousMove.get_bit_index_from();
    int toIndex = previousMove.get_bit_index_to();

    // Determine the piece type of the piece that was previously moved,
    // takes into consideration if the move was a promotion
    model::Piece::Type  previouslyMovedPieceType = determineMovedPieceType(previousMove, wasWhite, toIndex);

    // We do the move in reverse, so now we pick up the previously moved piece
    removePreviouslyMovedPieceFromBoard(previousMove, toIndex, previouslyMovedPieceType, wasWhite);

    // We place back the captured piece if there was one
    if (previousMove.is_any_capture()) {
       // Calculate the index of the previously captured piece, might be EP
        int captureIndex = MoveUtils::determineCaptureIndex(previousMove, wasWhite, toIndex);

        placeBackCapturedPieceOnBoard(previousMove.is_ep_capture(), captureIndex, toIndex, wasWhite, previousMoveResults.capturedPieceType);
    } else {
        // If there was no capture, we place back an empty square on the to square
        piece_map_.set_piece_type_at_index(toIndex, model::Piece::Type::EMPTY);
    }

    // Place the moved piece back on the from square
    placeBackMovedPieceOnBoard(wasWhite, fromIndex, previouslyMovedPieceType);

    state_bitmasks_.update_occupied_and_empty_squares_bitmasks();
}

} // namespace logic