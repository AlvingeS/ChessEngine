#include "KnightGenerator.h"

#include "ChessEngine/utils/ChessUtils.h"
#include "ChessEngine/board/PieceType.h"

namespace movegen {

KnightGenerator::KnightGenerator(board::BitBoards& bitboards, board::GameStateBitMasks& gameStateBitMasks, int& moveIndex, CommonLogic* commonLogic) 
    : _bitboards(bitboards),
        _gameStateBitmasks(gameStateBitMasks),
        _moveIndex(moveIndex),
        _commonLogic(commonLogic) {
    _freeMovesIndices.reserve(8);
    _capturableMovesIndices.reserve(8);
    _knightIndices.reserve(64);
    _knightBitmasks = masks::getAllKnightBitMasks();
}

void KnightGenerator::generate(
    bool isWhite,
    std::vector<move::Move>& moveList) 
{
    utils::getBitIndices(_knightIndices, isWhite ? _bitboards.getWhiteKnightsBitboard()
                                                    : _bitboards.getBlackKnightsBitboard());

    for (int currentKnightIndex : _knightIndices) {
        U64 knightBitMask = _knightBitmasks[currentKnightIndex];

        U64 freeKnightMoves = knightBitMask & _gameStateBitmasks.getEmptySquaresBitmask();
        
        U64 enemyPieces = isWhite ? _gameStateBitmasks.getBlackPiecesBitmask()
                                    : _gameStateBitmasks.getWhitePiecesBitmask();
        
        U64 capturableKnightMoves = knightBitMask & enemyPieces;

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