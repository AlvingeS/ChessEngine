#include "ChessEngine/src/logic/movegen/KingGenerator.h"

#include "ChessEngine/src/model/board/PieceType.h"

#include "ChessEngine/src/logic/movegen/utils/Containers.h"
#include "ChessEngine/src/logic/movegen/utils/BitBasics.h"

namespace logic {

KingGenerator::KingGenerator(
    const model::Bitboards& bitboards,
    const model::GameStateBitmasks& gameStateBitmasks) 
    : _bitboardsRef(bitboards)
    , _gameStateBitmasksRef(gameStateBitmasks)
{
    _kingBitmasks = bitmasks::getAllKingBitmasks();
}

void KingGenerator::generate(bool isWhite, model::Movelist& moveListRef) 
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
            moveListRef.addMove(model::Move(currentKingIndex, freeKingMoveIndex, model::Move::QUITE_FLAG));
        }

        for (int capturableKingMoveIndex : capturableMovesIndices) {
            moveListRef.addMove(model::Move(currentKingIndex, capturableKingMoveIndex, model::Move::CAPTURE_FLAG));
        }
    }
}

} // namespace logic