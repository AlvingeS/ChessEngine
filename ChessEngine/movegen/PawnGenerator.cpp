#include "PawnGenerator.h"

#include "ChessEngine/utils/ChessUtils.h"
#include "ChessEngine/game/PieceType.h"

namespace movegen {
    PawnGenerator::PawnGenerator(game::ChessBoard& board, int& moveIndex, CommonLogic* commonLogic) 
        : _board(board),
          _moveIndex(moveIndex),
          _commonLogic(commonLogic) {
        _freeMovesIndices.reserve(8);
        _capturableMovesIndices.reserve(8);
        _pawnIndices.reserve(64);
        _whitePawnStraightMoveBitmasks = bits::getAllStraightPawnMoveBitmasks(true);
        _whitePawnCaptureMoveBitmasks = bits::getAllCapturePawnMoveBitmasks(true);
        _blackPawnStraightMoveBitmasks = bits::getAllStraightPawnMoveBitmasks(false);
        _blackPawnCaptureMoveBitmasks = bits::getAllCapturePawnMoveBitmasks(false);
    }

    void PawnGenerator::generate(bool isWhite, std::vector<game::Move>& moveList, int currentDepth, search::SearchMemory& searchMemory) {
        bits::getBitIndices(_pawnIndices, isWhite ? _board.getWhitePawnsBitboard()
                                                  : _board.getBlackPawnsBitboard());

        for (int currentPawnIndex : _pawnIndices) {
            U64 straightPawnMoveBitmask = isWhite ? _whitePawnStraightMoveBitmasks[currentPawnIndex]
                                                        : _blackPawnStraightMoveBitmasks[currentPawnIndex];

            U64 capturePawnMoveBitmask = isWhite ? _whitePawnCaptureMoveBitmasks[currentPawnIndex]
                                                       : _blackPawnCaptureMoveBitmasks[currentPawnIndex];


            U64 freePawnMoves = straightPawnMoveBitmask & _board.getEmptySquaresBitmask();
            U64 enemyPieces = isWhite ? _board.getBlackPiecesBitmask() : _board.getWhitePiecesBitmask();
            U64 enPessantTarget = searchMemory.getEnPessantTargetAtDepth(currentDepth);
            U64 capturablePawnMoves = capturePawnMoveBitmask & enemyPieces;

            bits::getBitIndices(_freeMovesIndices, freePawnMoves);
            bits::getBitIndices(_capturableMovesIndices, capturablePawnMoves);
            int offset = isWhite ? 8 : -8;
            bool canPromote = (isWhite && bits::rankFromBitIndex(currentPawnIndex) == 6) || (!isWhite && bits::rankFromBitIndex(currentPawnIndex) == 1);

            if (_freeMovesIndices.size() == 2) {
                int singleStepIndex = (isWhite ? 0 : 1);
                int doubleStepIndex = (isWhite ? 1 : 0);

                _commonLogic->addMove(currentPawnIndex, _freeMovesIndices[singleStepIndex], (game::Move::QUITE_FLAG), moveList, _moveIndex);
                _commonLogic->addMove(currentPawnIndex, _freeMovesIndices[doubleStepIndex], (game::Move::DOUBLE_PAWN_PUSH_FLAG), moveList, _moveIndex);
            } else if (_freeMovesIndices.size() == 1 && _freeMovesIndices[0] == currentPawnIndex + offset) {
                // Only add them move it is direcly in front of the pawn, to avoid jumping over pieces
                if (canPromote) {
                    _commonLogic->addMove(currentPawnIndex, _freeMovesIndices[0], game::Move::KNIGHT_PROMO_FLAG, moveList, _moveIndex);
                    _commonLogic->addMove(currentPawnIndex, _freeMovesIndices[0], game::Move::BISHOP_PROMO_FLAG, moveList, _moveIndex);
                    _commonLogic->addMove(currentPawnIndex, _freeMovesIndices[0], game::Move::ROOK_PROMO_FLAG, moveList, _moveIndex);
                    _commonLogic->addMove(currentPawnIndex, _freeMovesIndices[0], game::Move::QUEEN_PROMO_FLAG, moveList, _moveIndex);
                } else {
                    _commonLogic->addMove(currentPawnIndex, _freeMovesIndices[0], game::Move::QUITE_FLAG, moveList, _moveIndex);
                }
            }

            for (int capturablePawnMoveIndex : _capturableMovesIndices) {
                if (canPromote) {
                    _commonLogic->addMove(currentPawnIndex, capturablePawnMoveIndex, game::Move::KNIGHT_PROMO_CAPTURE_FLAG, moveList, _moveIndex);
                    _commonLogic->addMove(currentPawnIndex, capturablePawnMoveIndex, game::Move::BISHOP_PROMO_CAPTURE_FLAG, moveList, _moveIndex);
                    _commonLogic->addMove(currentPawnIndex, capturablePawnMoveIndex, game::Move::ROOK_PROMO_CAPTURE_FLAG, moveList, _moveIndex);
                    _commonLogic->addMove(currentPawnIndex, capturablePawnMoveIndex, game::Move::QUEEN_PROMO_CAPTURE_FLAG, moveList, _moveIndex);
                } else {
                    _commonLogic->addMove(currentPawnIndex, capturablePawnMoveIndex, game::Move::CAPTURE_FLAG, moveList, _moveIndex);
                }
            }

            if ((capturePawnMoveBitmask & enPessantTarget) != 0) {
                _commonLogic->addMove(currentPawnIndex, bits::indexOfLSB(capturePawnMoveBitmask & enPessantTarget), game::Move::EP_CAPTURE_FLAG, moveList, _moveIndex);
            }
        }
    }
}