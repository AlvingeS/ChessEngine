#include "KnightGenerator.h"
#include "ChessEngine/bits/ChessUtils.h"
#include "ChessEngine/game/PieceType.h"

namespace movegen {
    KnightGenerator::KnightGenerator(game::ChessBoard& board, int& moveIndex, CommonLogic* commonLogic) 
        : _board(board),
          _moveIndex(moveIndex),
          _commonLogic(commonLogic) {
        _freeMovesIndices.reserve(8);
        _capturableMovesIndices.reserve(8);
        _knightIndices.reserve(64);
        _knightBitmasks = bits::getAllKnightBitMasks();
    }

    void KnightGenerator::generate(bool isWhite, std::vector<game::Move>& moveList) {
        bits::getBitIndices(_knightIndices, isWhite ? _board.getWhiteKnightsBitboard() : _board.getBlackKnightsBitboard());

        for (int currentKnightIndex : _knightIndices) {
            bits::U64 knightBitMask = _knightBitmasks[currentKnightIndex];

            bits::U64 freeKnightMoves = knightBitMask & _board.getEmptySquaresBitmask();
            bits::U64 enemyPieces = isWhite ? _board.getBlackPiecesBitmask() : _board.getWhitePiecesBitmask();
            bits::U64 capturableKnightMoves = knightBitMask & enemyPieces;

            bits::getBitIndices(_freeMovesIndices, freeKnightMoves);
            bits::getBitIndices(_capturableMovesIndices, capturableKnightMoves);

            for (int freeKnightMoveIndex : _freeMovesIndices) {
                _commonLogic->addMove(currentKnightIndex, freeKnightMoveIndex, game::Move::QUITE_FLAG, moveList, _moveIndex);
            }

            for (int capturableKnightMoveIndex : _capturableMovesIndices) {
                _commonLogic->addMove(currentKnightIndex, capturableKnightMoveIndex, game::Move::CAPTURE_FLAG, moveList, _moveIndex);
            }
        }
    }
}