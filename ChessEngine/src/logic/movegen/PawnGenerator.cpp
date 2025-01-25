#include "ChessEngine/src/logic/movegen/PawnGenerator.h"

#include "ChessEngine/src/model/board/Board.h"
#include "ChessEngine/src/model/move/Movelist.h"

#include "ChessEngine/src/logic/movegen/bitmasks/PawnBitmasks.h"
#include "ChessEngine/src/logic/movegen/utils/Containers.h"
#include "ChessEngine/src/logic/movegen/utils/ChessUtils.h"
#include "ChessEngine/src/logic/movegen/utils/BitBasics.h"

namespace logic {

PawnGenerator::PawnGenerator(model::Board& board) 
    : _bitboards(board.bitboards)
    , _stateBitmasks(board.stateBitmasks)
{
    _whitePawnStraightMoveBitmasks = getAllStraightPawnMoveBitmasks(true);
    _whitePawnCaptureMoveBitmasks = getAllCapturePawnMoveBitmasks(true);
    _blackPawnStraightMoveBitmasks = getAllStraightPawnMoveBitmasks(false);
    _blackPawnCaptureMoveBitmasks = getAllCapturePawnMoveBitmasks(false);
}

void PawnGenerator::generate(
    bool isWhite,
    model::Movelist& moveList,
    int currentDepth,
    engine::SearchMemory& searchMemory)
{
    std::vector<int>& pawnIndices = Containers::getPiecePositionIndices();
    std::vector<int>& freeMovesIndices = Containers::getLeapingPiecefreeMovesIndices();
    std::vector<int>& capturableMovesIndices = Containers::getLeapingPieceCapturableMovesIndices();

    getBitIndices(pawnIndices, isWhite ? _bitboards.getWhitePawnsBitboard()
                                              : _bitboards.getBlackPawnsBitboard());

    for (int currentPawnIndex : pawnIndices) {

        bitmask straightPawnMoveBitmask = isWhite ? _whitePawnStraightMoveBitmasks[currentPawnIndex]
                                                  : _blackPawnStraightMoveBitmasks[currentPawnIndex];

        bitmask capturePawnMoveBitmask = isWhite ? _whitePawnCaptureMoveBitmasks[currentPawnIndex]
                                                 : _blackPawnCaptureMoveBitmasks[currentPawnIndex];

        bitmask freePawnMoves = straightPawnMoveBitmask & _stateBitmasks.getEmptySquaresBitmask();
        
        bitmask enemyPieces = isWhite ? _stateBitmasks.getBlackPiecesBitmask()
                                      : _stateBitmasks.getWhitePiecesBitmask();
        
        bitmask enPessantTarget = searchMemory.getEnPessantTargetAtDepth(currentDepth);
        bitmask capturablePawnMoves = capturePawnMoveBitmask & enemyPieces;

        getBitIndices(freeMovesIndices, freePawnMoves);
        getBitIndices(capturableMovesIndices, capturablePawnMoves);

        int offset = isWhite ? 8 : -8;
        bool canPromote = (isWhite && rankFromBitIndex(currentPawnIndex) == 6) || (!isWhite && rankFromBitIndex(currentPawnIndex) == 1);

        if (freeMovesIndices.size() == 2) {
            int singleStepIndex = (isWhite ? 0 : 1);
            int doubleStepIndex = (isWhite ? 1 : 0);
            
            moveList.addMove(model::Move(currentPawnIndex, freeMovesIndices[singleStepIndex], model::Move::QUITE_FLAG));
            moveList.addMove(model::Move(currentPawnIndex, freeMovesIndices[doubleStepIndex], model::Move::DOUBLE_PAWN_PUSH_FLAG));

        } else if (freeMovesIndices.size() == 1 && freeMovesIndices[0] == currentPawnIndex + offset) {
            // Only add them move it is direcly in front of the pawn, to avoid jumping over pieces
            if (canPromote) {
                moveList.addMove(model::Move(currentPawnIndex, freeMovesIndices[0], model::Move::KNIGHT_PROMO_FLAG));
                moveList.addMove(model::Move(currentPawnIndex, freeMovesIndices[0], model::Move::BISHOP_PROMO_FLAG));
                moveList.addMove(model::Move(currentPawnIndex, freeMovesIndices[0], model::Move::ROOK_PROMO_FLAG));
                moveList.addMove(model::Move(currentPawnIndex, freeMovesIndices[0], model::Move::QUEEN_PROMO_FLAG));
            
            } else {
                moveList.addMove(model::Move(currentPawnIndex, freeMovesIndices[0], model::Move::QUITE_FLAG));
            }
        }

        for (int capturablePawnMoveIndex : capturableMovesIndices) {
            if (canPromote) {
                moveList.addMove(model::Move(currentPawnIndex, capturablePawnMoveIndex, model::Move::QUEEN_PROMO_CAPTURE_FLAG));
                moveList.addMove(model::Move(currentPawnIndex, capturablePawnMoveIndex, model::Move::ROOK_PROMO_CAPTURE_FLAG));
                moveList.addMove(model::Move(currentPawnIndex, capturablePawnMoveIndex, model::Move::BISHOP_PROMO_CAPTURE_FLAG));
                moveList.addMove(model::Move(currentPawnIndex, capturablePawnMoveIndex, model::Move::KNIGHT_PROMO_CAPTURE_FLAG));
            } else {
                moveList.addMove(model::Move(currentPawnIndex, capturablePawnMoveIndex, model::Move::CAPTURE_FLAG));
            }
        }

        if ((capturePawnMoveBitmask & enPessantTarget) != 0) {
            moveList.addMove(model::Move(currentPawnIndex, indexOfLSB(capturePawnMoveBitmask & enPessantTarget), model::Move::EP_CAPTURE_FLAG));
        }
    }
}

} // namespace logic