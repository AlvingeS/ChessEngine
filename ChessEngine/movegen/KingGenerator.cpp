#include "KingGenerator.h"

#include "ChessEngine/utils/ChessUtils.h"
#include "ChessEngine/board/PieceType.h"

namespace movegen {

KingGenerator::KingGenerator(
    const board::Bitboards& bitboards,
    const board::GameStateBitmasks& gameStateBitmasks,
    int& moveIndex,
    CommonLogic* commonLogic) 
    : _bitboardsRef(bitboards)
    , _gameStateBitmasksRef(gameStateBitmasks)
    , _moveIndex(moveIndex)
    , _commonLogic(commonLogic) 
{
    _freeMovesIndices.reserve(8);
    _capturableMovesIndices.reserve(8);
    _kingIndices.reserve(64);
    _kingBitmasks = masks::getAllKingBitmasks();
}

void KingGenerator::generate(bool isWhite, std::vector<move::Move>& moveList) 
{
    utils::getBitIndices(_kingIndices, isWhite ? _bitboardsRef.getWhiteKingBitboard()
                                               : _bitboardsRef.getBlackKingBitboard());

    for (int currentKingIndex : _kingIndices) {
        bitmask kingBitmask = _kingBitmasks[currentKingIndex];

        bitmask freeKingMoves = kingBitmask & _gameStateBitmasksRef.getEmptySquaresBitmask();
        
        bitmask enemyPieces = isWhite ? _gameStateBitmasksRef.getBlackPiecesBitmask() 
                                  : _gameStateBitmasksRef.getWhitePiecesBitmask();

        bitmask capturableKingMoves = kingBitmask & enemyPieces;

        utils::getBitIndices(_freeMovesIndices, freeKingMoves);
        utils::getBitIndices(_capturableMovesIndices, capturableKingMoves);

        for (int freeKingMoveIndex : _freeMovesIndices) {
            _commonLogic->addMove(
                currentKingIndex,
                freeKingMoveIndex,
                move::Move::QUITE_FLAG,
                moveList,
                _moveIndex
            );
        }

        for (int capturableKingMoveIndex : _capturableMovesIndices) {
            _commonLogic->addMove(
                currentKingIndex,
                capturableKingMoveIndex,
                move::Move::CAPTURE_FLAG,
                moveList,
                _moveIndex
            );
        }
    }
}

} // namespace movegen