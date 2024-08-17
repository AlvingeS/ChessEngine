#include "KnightGenerator.h"

#include "ChessEngine/utils/ChessUtils.h"
#include "ChessEngine/board/PieceType.h"

namespace movegen {

KnightGenerator::KnightGenerator(board::Bitboards& bitboards, board::GameStateBitmasks& gameStateBitmasks, int& moveIndex, CommonLogic* commonLogic) 
    : _bitboards(bitboards),
        _gameStateBitmasks(gameStateBitmasks),
        _moveIndex(moveIndex),
        _commonLogic(commonLogic) {
    _freeMovesIndices.reserve(8);
    _capturableMovesIndices.reserve(8);
    _knightIndices.reserve(64);
    _knightBitmasks = masks::getAllKnightBitmasks();
}

void KnightGenerator::generate(
    bool isWhite,
    std::vector<move::Move>& moveList) 
{
    utils::getBitIndices(_knightIndices, isWhite ? _bitboards.getWhiteKnightsBitboard()
                                                    : _bitboards.getBlackKnightsBitboard());

    for (int currentKnightIndex : _knightIndices) {
        bitmask knightBitmask = _knightBitmasks[currentKnightIndex];

        bitmask freeKnightMoves = knightBitmask & _gameStateBitmasks.getEmptySquaresBitmask();
        
        bitmask enemyPieces = isWhite ? _gameStateBitmasks.getBlackPiecesBitmask()
                                    : _gameStateBitmasks.getWhitePiecesBitmask();
        
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