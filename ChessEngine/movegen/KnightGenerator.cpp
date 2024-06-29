#include "KnightGenerator.h"

#include "ChessEngine/utils/ChessUtils.h"
#include "ChessEngine/game/PieceType.h"

namespace movegen {
    KnightGenerator::KnightGenerator(game::ChessBoard& board, int& moveIndex, CommonLogic* commonLogic) 
        : _board(board),
          _moveIndex(moveIndex),
          _commonLogic(commonLogic) {
        _freeMovesIndices.reserve(8);
        _capturableMovesIndices.reserve(8);
        _knightIndices.reserve(64);
        _knightBitmasks = masks::getAllKnightBitMasks();
    }

    void KnightGenerator::generate(bool isWhite, std::vector<game::Move>& moveList) {
        utils::getBitIndices(_knightIndices, isWhite ? _board.getWhiteKnightsBitboard() : _board.getBlackKnightsBitboard());

        for (int currentKnightIndex : _knightIndices) {
            U64 knightBitMask = _knightBitmasks[currentKnightIndex];

            U64 freeKnightMoves = knightBitMask & _board.getEmptySquaresBitmask();
            U64 enemyPieces = isWhite ? _board.getBlackPiecesBitmask() : _board.getWhitePiecesBitmask();
            U64 capturableKnightMoves = knightBitMask & enemyPieces;

            utils::getBitIndices(_freeMovesIndices, freeKnightMoves);
            utils::getBitIndices(_capturableMovesIndices, capturableKnightMoves);

            for (int freeKnightMoveIndex : _freeMovesIndices) {
                _commonLogic->addMove(currentKnightIndex, freeKnightMoveIndex, game::Move::QUITE_FLAG, moveList, _moveIndex);
            }

            for (int capturableKnightMoveIndex : _capturableMovesIndices) {
                _commonLogic->addMove(currentKnightIndex, capturableKnightMoveIndex, game::Move::CAPTURE_FLAG, moveList, _moveIndex);
            }
        }
    }
}