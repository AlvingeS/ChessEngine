#include "KnightGenerator.h"

#include "ChessEngine/utils/ChessUtils.h"
#include "ChessEngine/board/PieceType.h"

namespace movegen {

KnightGenerator::KnightGenerator(
    const board::Bitboards& bitboards,
    const board::GameStateBitmasks& gameStateBitmasks,
    int& moveIndex,
    CommonLogic* commonLogic) 
    : _bitboardsRef(bitboards)
    , _gameStateBitmasksRef(gameStateBitmasks)
    , _moveIndex(moveIndex)
    , _commonLogic(commonLogic) {
    _freeMovesIndices.reserve(8);
    _capturableMovesIndices.reserve(8);
    _knightIndices.reserve(64);
    _knightBitmasks = masks::getAllKnightBitmasks();
}

void KnightGenerator::generate(
    bool isWhite,
    std::vector<move::Move>& moveList) 
{
    utils::getBitIndices(_knightIndices, isWhite ? _bitboardsRef.getWhiteKnightsBitboard()
                                                    : _bitboardsRef.getBlackKnightsBitboard());

    for (int currentKnightIndex : _knightIndices) {
        bitmask knightBitmask = _knightBitmasks[currentKnightIndex];

        bitmask freeKnightMoves = knightBitmask & _gameStateBitmasksRef.getEmptySquaresBitmask();
        
        bitmask enemyPieces = isWhite ? _gameStateBitmasksRef.getBlackPiecesBitmask()
                                    : _gameStateBitmasksRef.getWhitePiecesBitmask();
        
        bitmask capturableKnightMoves = knightBitmask & enemyPieces;

        utils::getBitIndices(_freeMovesIndices, freeKnightMoves);
        utils::getBitIndices(_capturableMovesIndices, capturableKnightMoves);

        for (int freeKnightMoveIndex : _freeMovesIndices) {
            _commonLogic->addMove(currentKnightIndex, freeKnightMoveIndex, move::Move::QUITE_FLAG, moveList, _moveIndex);
        }

        for (int capturableKnightMoveIndex : _capturableMovesIndices) {
            _commonLogic->addMove(currentKnightIndex, capturableKnightMoveIndex, move::Move::CAPTURE_FLAG, moveList, _moveIndex);
        }
    }
}

} // namespace movegen