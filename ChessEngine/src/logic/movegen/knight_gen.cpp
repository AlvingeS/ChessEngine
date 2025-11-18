#include "logic/movegen/knight_gen.h"

#include "model/position/board.h"
#include "model/move/movelist.h"

#include "logic/movegen/bitmasks/knight_bitmasks.h"
#include "logic/movegen/utils/containers.h"
#include "logic/movegen/utils/bit_basics.h"

namespace logic {

KnightGenerator::KnightGenerator(model::Board& board) 
    : _bitboards(board.bitboards)
    , _stateBitmasks(board.state_bitmasks) {
    _knightBitmasks = KnightBitmasks::getAllKnightBitmasks();
}

void KnightGenerator::generate(
    bool isWhite,
    model::Movelist& movelist) 
{
    std::vector<int>& knightIndices = Containers::getPiecePositionIndices();
    std::vector<int>& freeMovesIndices = Containers::getLeapingPiecefreeMovesIndices();
    std::vector<int>& capturableMovesIndices = Containers::getLeapingPieceCapturableMovesIndices();

    BitBasics::getBitIndices(knightIndices, isWhite ? _bitboards.get_w_knights_bitboard()
                                                    : _bitboards.get_b_knights_bitboard());

    for (int currentKnightIndex : knightIndices) {
        bitmask knightBitmask = _knightBitmasks[currentKnightIndex];

        bitmask freeKnightMoves = knightBitmask & _stateBitmasks.get_empty_squares_bitmask();
        
        bitmask enemyPieces = isWhite ? _stateBitmasks.get_b_pieces_bitmask()
                                    : _stateBitmasks.get_w_pieces_bitmask();
        
        bitmask capturableKnightMoves = knightBitmask & enemyPieces;

        BitBasics::getBitIndices(freeMovesIndices, freeKnightMoves);
        BitBasics::getBitIndices(capturableMovesIndices, capturableKnightMoves);

        for (int freeKnightMoveIndex : freeMovesIndices) {
            movelist.addMove(model::Move(currentKnightIndex, freeKnightMoveIndex, model::Move::QUITE_FLAG));
        }

        for (int capturableKnightMoveIndex : capturableMovesIndices) {
            movelist.addMove(model::Move(currentKnightIndex, capturableKnightMoveIndex, model::Move::CAPTURE_FLAG));
        }
    }
}

} // namespace logic