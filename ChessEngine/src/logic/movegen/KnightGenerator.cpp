#include "logic/movegen/KnightGenerator.h"

#include "model/position/Board.h"
#include "model/move/Movelist.h"

#include "logic/movegen/bitmasks/KnightBitmasks.h"
#include "logic/movegen/utils/Containers.h"
#include "logic/movegen/utils/BitBasics.h"

namespace logic {

KnightGenerator::KnightGenerator(model::Board& board) 
    : _bitboards(board.bitboards)
    , _stateBitmasks(board.stateBitmasks) {
    _knightBitmasks = KnightBitmasks::getAllKnightBitmasks();
}

void KnightGenerator::generate(
    bool isWhite,
    model::Movelist& movelist) 
{
    std::vector<int>& knightIndices = Containers::getPiecePositionIndices();
    std::vector<int>& freeMovesIndices = Containers::getLeapingPiecefreeMovesIndices();
    std::vector<int>& capturableMovesIndices = Containers::getLeapingPieceCapturableMovesIndices();

    BitBasics::getBitIndices(knightIndices, isWhite ? _bitboards.getWhiteKnightsBitboard()
                                                    : _bitboards.getBlackKnightsBitboard());

    for (int currentKnightIndex : knightIndices) {
        bitmask knightBitmask = _knightBitmasks[currentKnightIndex];

        bitmask freeKnightMoves = knightBitmask & _stateBitmasks.getEmptySquaresBitmask();
        
        bitmask enemyPieces = isWhite ? _stateBitmasks.getBlackPiecesBitmask()
                                    : _stateBitmasks.getWhitePiecesBitmask();
        
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