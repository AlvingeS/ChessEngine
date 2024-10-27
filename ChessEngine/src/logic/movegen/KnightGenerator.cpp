#include "ChessEngine/src/logic/movegen/KnightGenerator.h"

#include "ChessEngine/src/representation/board/PieceType.h"

#include "ChessEngine/src/logic/movegen/utils/Containers.h"
#include "ChessEngine/src/logic/movegen/utils/BitBasics.h"

namespace logic {
namespace movegen {

KnightGenerator::KnightGenerator(
    const board::Bitboards& bitboards,
    const board::GameStateBitmasks& gameStateBitmasks) 
    : _bitboardsRef(bitboards)
    , _gameStateBitmasksRef(gameStateBitmasks) {
    _knightBitmasks = masks::getAllKnightBitmasks();
}

void KnightGenerator::generate(
    bool isWhite,
    Movelist& movelist) 
{
    std::vector<int>& knightIndices = utils::Containers::getPiecePositionIndices();
    std::vector<int>& freeMovesIndices = utils::Containers::getLeapingPiecefreeMovesIndices();
    std::vector<int>& capturableMovesIndices = utils::Containers::getLeapingPieceCapturableMovesIndices();

    utils::getBitIndices(knightIndices, isWhite ? _bitboardsRef.getWhiteKnightsBitboard()
                                                    : _bitboardsRef.getBlackKnightsBitboard());

    for (int currentKnightIndex : knightIndices) {
        bitmask knightBitmask = _knightBitmasks[currentKnightIndex];

        bitmask freeKnightMoves = knightBitmask & _gameStateBitmasksRef.getEmptySquaresBitmask();
        
        bitmask enemyPieces = isWhite ? _gameStateBitmasksRef.getBlackPiecesBitmask()
                                    : _gameStateBitmasksRef.getWhitePiecesBitmask();
        
        bitmask capturableKnightMoves = knightBitmask & enemyPieces;

        utils::getBitIndices(freeMovesIndices, freeKnightMoves);
        utils::getBitIndices(capturableMovesIndices, capturableKnightMoves);

        for (int freeKnightMoveIndex : freeMovesIndices) {
            movelist.addMove(move::Move(currentKnightIndex, freeKnightMoveIndex, move::Move::QUITE_FLAG));
        }

        for (int capturableKnightMoveIndex : capturableMovesIndices) {
            movelist.addMove(move::Move(currentKnightIndex, capturableKnightMoveIndex, move::Move::CAPTURE_FLAG));
        }
    }
}

} // namespace movegen
} // namespace logic