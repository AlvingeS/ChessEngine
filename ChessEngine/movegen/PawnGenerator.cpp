#include "PawnGenerator.h"

#include "ChessEngine/utils/ChessUtils.h"
#include "ChessEngine/board/PieceType.h"

namespace movegen {
PawnGenerator::PawnGenerator(
    board::BitBoards& bitboards,
    board::GameStateBitMasks& gameStateBitmasks,
    int& moveIndex,
    CommonLogic* commonLogic) 
    : _bitboards(bitboards)
    ,  _gameStateBitmasks(gameStateBitmasks)
    ,  _moveIndex(moveIndex)
    ,  _commonLogic(commonLogic)
{
    _freeMovesIndices.reserve(8);
    _capturableMovesIndices.reserve(8);
    _pawnIndices.reserve(64);
    _whitePawnStraightMoveBitmasks = masks::getAllStraightPawnMoveBitmasks(true);
    _whitePawnCaptureMoveBitmasks = masks::getAllCapturePawnMoveBitmasks(true);
    _blackPawnStraightMoveBitmasks = masks::getAllStraightPawnMoveBitmasks(false);
    _blackPawnCaptureMoveBitmasks = masks::getAllCapturePawnMoveBitmasks(false);
}

void PawnGenerator::generate(
    bool isWhite,
    std::vector<move::Move>& moveList,
    int currentDepth,
    perft::SearchMemory& searchMemory)
{
    utils::getBitIndices(_pawnIndices, isWhite ? _bitboards.getWhitePawnsBitboard()
                                               : _bitboards.getBlackPawnsBitboard());

    for (int currentPawnIndex : _pawnIndices) {

        bitmask straightPawnMoveBitmask = isWhite ? _whitePawnStraightMoveBitmasks[currentPawnIndex]
                                              : _blackPawnStraightMoveBitmasks[currentPawnIndex];

        bitmask capturePawnMoveBitmask = isWhite ? _whitePawnCaptureMoveBitmasks[currentPawnIndex]
                                             : _blackPawnCaptureMoveBitmasks[currentPawnIndex];

        bitmask freePawnMoves = straightPawnMoveBitmask & _gameStateBitmasks.getEmptySquaresBitmask();
        
        bitmask enemyPieces = isWhite ? _gameStateBitmasks.getBlackPiecesBitmask()
                                  : _gameStateBitmasks.getWhitePiecesBitmask();
        
        bitmask enPessantTarget = searchMemory.getEnPessantTargetAtDepth(currentDepth);
        bitmask capturablePawnMoves = capturePawnMoveBitmask & enemyPieces;

        utils::getBitIndices(_freeMovesIndices, freePawnMoves);
        utils::getBitIndices(_capturableMovesIndices, capturablePawnMoves);

        int offset = isWhite ? 8 : -8;
        bool canPromote = (isWhite && utils::rankFromBitIndex(currentPawnIndex) == 6) || (!isWhite && utils::rankFromBitIndex(currentPawnIndex) == 1);

        if (_freeMovesIndices.size() == 2) {
            int singleStepIndex = (isWhite ? 0 : 1);
            int doubleStepIndex = (isWhite ? 1 : 0);

            _commonLogic->addMove(
                currentPawnIndex,
                _freeMovesIndices[singleStepIndex],
                (move::Move::QUITE_FLAG), moveList,
                _moveIndex
            );

            _commonLogic->addMove(
                currentPawnIndex,
                _freeMovesIndices[doubleStepIndex],
                (move::Move::DOUBLE_PAWN_PUSH_FLAG),
                moveList,
                _moveIndex
            );

        } else if (_freeMovesIndices.size() == 1 && _freeMovesIndices[0] == currentPawnIndex + offset) {
            // Only add them move it is direcly in front of the pawn, to avoid jumping over pieces
            if (canPromote) {
                _commonLogic->addMove(
                    currentPawnIndex,
                    _freeMovesIndices[0],
                    move::Move::KNIGHT_PROMO_FLAG,
                    moveList,
                    _moveIndex
                );

                _commonLogic->addMove(
                    currentPawnIndex,
                    _freeMovesIndices[0],
                    move::Move::BISHOP_PROMO_FLAG,
                    moveList,
                    _moveIndex
                );

                _commonLogic->addMove(
                    currentPawnIndex,
                    _freeMovesIndices[0],
                    move::Move::ROOK_PROMO_FLAG, 
                    moveList,
                    _moveIndex);
                
 
                _commonLogic->addMove(
                    currentPawnIndex,
                    _freeMovesIndices[0],
                    move::Move::QUEEN_PROMO_FLAG, 
                    moveList, 
                    _moveIndex)
                ;

            } else {
                _commonLogic->addMove(currentPawnIndex, _freeMovesIndices[0], move::Move::QUITE_FLAG, moveList, _moveIndex);
            }
        }

        for (int capturablePawnMoveIndex : _capturableMovesIndices) {
            if (canPromote) {
                _commonLogic->addMove(currentPawnIndex, capturablePawnMoveIndex, move::Move::KNIGHT_PROMO_CAPTURE_FLAG, moveList, _moveIndex);
                _commonLogic->addMove(currentPawnIndex, capturablePawnMoveIndex, move::Move::BISHOP_PROMO_CAPTURE_FLAG, moveList, _moveIndex);
                _commonLogic->addMove(currentPawnIndex, capturablePawnMoveIndex, move::Move::ROOK_PROMO_CAPTURE_FLAG, moveList, _moveIndex);
                _commonLogic->addMove(currentPawnIndex, capturablePawnMoveIndex, move::Move::QUEEN_PROMO_CAPTURE_FLAG, moveList, _moveIndex);
            } else {
                _commonLogic->addMove(currentPawnIndex, capturablePawnMoveIndex, move::Move::CAPTURE_FLAG, moveList, _moveIndex);
            }
        }

        if ((capturePawnMoveBitmask & enPessantTarget) != 0) {
            _commonLogic->addMove(currentPawnIndex, utils::indexOfLSB(capturePawnMoveBitmask & enPessantTarget), move::Move::EP_CAPTURE_FLAG, moveList, _moveIndex);
        }
    }
}

} // namespace movegen