#include "KingGenerator.h"
#include "ChessEngine/bits/ChessUtils.h"
#include "ChessEngine/game/PieceType.h"

namespace movegen {
    KingGenerator::KingGenerator(game::ChessBoard& board, int& moveIndex, CommonLogic* commonLogic) 
        : _board(board),
          _moveIndex(moveIndex),
          _commonLogic(commonLogic) {
        _freeMovesIndices.reserve(8);
        _capturableMovesIndices.reserve(8);
        _kingIndices.reserve(64);
        _kingBitmasks = bits::getAllKingBitMasks();
    }

    void KingGenerator::generate(bool isWhite, std::vector<game::Move>& moveList) {
        bits::getBitIndices(_kingIndices, isWhite ? _board.getWhiteKingBitboard() : _board.getBlackKingBitboard());

        for (int currentKingIndex : _kingIndices) {
            bits::U64 kingBitMask = _kingBitmasks[currentKingIndex];

            bits::U64 freeKingMoves = kingBitMask & _board.getEmptySquaresBitmask();
            bits::U64 enemyPieces = isWhite ? _board.getBlackPiecesBitmask() : _board.getWhitePiecesBitmask();
            bits::U64 capturableKingMoves = kingBitMask & enemyPieces;

            bits::getBitIndices(_freeMovesIndices, freeKingMoves);
            bits::getBitIndices(_capturableMovesIndices, capturableKingMoves);

            for (int freeKingMoveIndex : _freeMovesIndices) {
                _commonLogic->addMove(currentKingIndex, freeKingMoveIndex, game::Move::QUITE_FLAG, moveList, _moveIndex);
            }

            for (int capturableKingMoveIndex : _capturableMovesIndices) {
                _commonLogic->addMove(currentKingIndex, capturableKingMoveIndex, game::Move::CAPTURE_FLAG, moveList, _moveIndex);
            }
        }
    }
}