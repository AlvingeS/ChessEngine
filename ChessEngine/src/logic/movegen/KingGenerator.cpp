#include "ChessEngine/src/logic/movegen/KingGenerator.h"

#include "ChessEngine/src/representation/board/PieceType.h"

#include "ChessEngine/src/logic/movegen/utils/Containers.h"
#include "ChessEngine/src/logic/movegen/utils/BitBasics.h"

namespace logic {
namespace movegen {

KingGenerator::KingGenerator(
    const representation::board::Bitboards& bitboards,
    const representation::board::GameStateBitmasks& gameStateBitmasks) 
    : _bitboardsRef(bitboards)
    , _gameStateBitmasksRef(gameStateBitmasks)
{
    _kingBitmasks = bitmasks::getAllKingBitmasks();
}

void KingGenerator::generate(bool isWhite, representation::move::Movelist& moveListRef) 
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
            moveListRef.addMove(representation::move::Move(currentKingIndex, freeKingMoveIndex, representation::move::Move::QUITE_FLAG));
        }

        for (int capturableKingMoveIndex : capturableMovesIndices) {
            moveListRef.addMove(representation::move::Move(currentKingIndex, capturableKingMoveIndex, representation::move::Move::CAPTURE_FLAG));
        }
    }
}

} // namespace movegen
} // namespace logic