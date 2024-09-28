#include "ChessEngine/src/game/movegen/KingGenerator.h"

#include "ChessEngine/src/game/board/PieceType.h"

#include "ChessEngine/src/utils/Containers.h"
#include "ChessEngine/src/utils/BitBasics.h"

namespace game {
namespace movegen {

KingGenerator::KingGenerator(
    const board::Bitboards& bitboards,
    const board::GameStateBitmasks& gameStateBitmasks) 
    : _bitboardsRef(bitboards)
    , _gameStateBitmasksRef(gameStateBitmasks)
{
    _kingBitmasks = masks::getAllKingBitmasks();
}

void KingGenerator::generate(bool isWhite, Movelist& moveListRef) 
{
    std::vector<int>& kingIndices = utils::Containers::getPiecePositionIndices();
    std::vector<int>& freeMovesIndices = utils::Containers::getLeapingPiecefreeMovesIndices();
    std::vector<int>& capturableMovesIndices = utils::Containers::getLeapingPieceCapturableMovesIndices();
    
    utils::getBitIndices(kingIndices, isWhite ? _bitboardsRef.getWhiteKingBitboard()
                                              : _bitboardsRef.getBlackKingBitboard());

    // TODO: It makes zero sense to have this in a loop
    for (int currentKingIndex : kingIndices) {
        bitmask kingBitmask = _kingBitmasks[currentKingIndex];

        bitmask freeKingMoves = kingBitmask & _gameStateBitmasksRef.getEmptySquaresBitmask();
        
        bitmask enemyPieces = isWhite ? _gameStateBitmasksRef.getBlackPiecesBitmask() 
                                      : _gameStateBitmasksRef.getWhitePiecesBitmask();

        bitmask capturableKingMoves = kingBitmask & enemyPieces;

        utils::getBitIndices(freeMovesIndices, freeKingMoves);
        utils::getBitIndices(capturableMovesIndices, capturableKingMoves);

        for (int freeKingMoveIndex : freeMovesIndices) {
            moveListRef.addMove(move::Move(currentKingIndex, freeKingMoveIndex, move::Move::QUITE_FLAG));
        }

        for (int capturableKingMoveIndex : capturableMovesIndices) {
            moveListRef.addMove(move::Move(currentKingIndex, capturableKingMoveIndex, move::Move::CAPTURE_FLAG));
        }
    }
}

} // namespace movegen
} // namespace game