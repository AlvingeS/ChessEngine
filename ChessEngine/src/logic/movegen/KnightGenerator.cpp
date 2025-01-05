#include "ChessEngine/src/logic/movegen/KnightGenerator.h"

#include "ChessEngine/src/model/board/PieceType.h"

#include "ChessEngine/src/logic/movegen/utils/Containers.h"
#include "ChessEngine/src/logic/movegen/utils/BitBasics.h"

namespace logic {

KnightGenerator::KnightGenerator(
    const model::Bitboards& bitboards,
    const model::StateBitmasks& stateBitmasks) 
    : _bitboardsRef(bitboards)
    , _stateBitmasksRef(stateBitmasks) {
    _knightBitmasks = getAllKnightBitmasks();
}

void KnightGenerator::generate(
    bool isWhite,
    model::Movelist& movelist) 
{
    std::vector<int>& knightIndices = Containers::getPiecePositionIndices();
    std::vector<int>& freeMovesIndices = Containers::getLeapingPiecefreeMovesIndices();
    std::vector<int>& capturableMovesIndices = Containers::getLeapingPieceCapturableMovesIndices();

    getBitIndices(knightIndices, isWhite ? _bitboardsRef.getWhiteKnightsBitboard()
                                                    : _bitboardsRef.getBlackKnightsBitboard());

    for (int currentKnightIndex : knightIndices) {
        bitmask knightBitmask = _knightBitmasks[currentKnightIndex];

        bitmask freeKnightMoves = knightBitmask & _stateBitmasksRef.getEmptySquaresBitmask();
        
        bitmask enemyPieces = isWhite ? _stateBitmasksRef.getBlackPiecesBitmask()
                                    : _stateBitmasksRef.getWhitePiecesBitmask();
        
        bitmask capturableKnightMoves = knightBitmask & enemyPieces;

        getBitIndices(freeMovesIndices, freeKnightMoves);
        getBitIndices(capturableMovesIndices, capturableKnightMoves);

        for (int freeKnightMoveIndex : freeMovesIndices) {
            movelist.addMove(model::Move(currentKnightIndex, freeKnightMoveIndex, model::Move::QUITE_FLAG));
        }

        for (int capturableKnightMoveIndex : capturableMovesIndices) {
            movelist.addMove(model::Move(currentKnightIndex, capturableKnightMoveIndex, model::Move::CAPTURE_FLAG));
        }
    }
}

} // namespace logic