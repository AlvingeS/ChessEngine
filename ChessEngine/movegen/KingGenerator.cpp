#include "KingGenerator.h"

#include "ChessEngine/utils/ChessUtils.h"
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
            U64 kingBitMask = _kingBitmasks[currentKingIndex];

            U64 freeKingMoves = kingBitMask & _board.getEmptySquaresBitmask();
            U64 enemyPieces = isWhite ? _board.getBlackPiecesBitmask() : _board.getWhitePiecesBitmask();
            U64 capturableKingMoves = kingBitMask & enemyPieces;

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