#include "KingGenerator.h"

#include "ChessEngine/utils/ChessUtils.h"
#include "ChessEngine/game/PieceType.h"

namespace movegen {
    KingGenerator::KingGenerator(game::BitBoards& bitboards, game::GameStateBitMasks& gameStateBitMasks, int& moveIndex, CommonLogic* commonLogic) 
        : _bitboards(bitboards),
          _gameStateBitmasks(gameStateBitMasks),
          _moveIndex(moveIndex),
          _commonLogic(commonLogic) {
        _freeMovesIndices.reserve(8);
        _capturableMovesIndices.reserve(8);
        _kingIndices.reserve(64);
        _kingBitmasks = masks::getAllKingBitMasks();
    }

    void KingGenerator::generate(bool isWhite, std::vector<game::Move>& moveList) {
        utils::getBitIndices(_kingIndices, isWhite ? _bitboards.getWhiteKingBitboard() : _bitboards.getBlackKingBitboard());

        for (int currentKingIndex : _kingIndices) {
            U64 kingBitMask = _kingBitmasks[currentKingIndex];

            U64 freeKingMoves = kingBitMask & _gameStateBitmasks.getEmptySquaresBitmask();
            U64 enemyPieces = isWhite ? _gameStateBitmasks.getBlackPiecesBitmask() : _gameStateBitmasks.getWhitePiecesBitmask();
            U64 capturableKingMoves = kingBitMask & enemyPieces;

            utils::getBitIndices(_freeMovesIndices, freeKingMoves);
            utils::getBitIndices(_capturableMovesIndices, capturableKingMoves);

            for (int freeKingMoveIndex : _freeMovesIndices) {
                _commonLogic->addMove(currentKingIndex, freeKingMoveIndex, game::Move::QUITE_FLAG, moveList, _moveIndex);
            }

            for (int capturableKingMoveIndex : _capturableMovesIndices) {
                _commonLogic->addMove(currentKingIndex, capturableKingMoveIndex, game::Move::CAPTURE_FLAG, moveList, _moveIndex);
            }
        }
    }
}