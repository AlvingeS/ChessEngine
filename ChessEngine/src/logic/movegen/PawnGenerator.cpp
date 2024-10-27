#include "ChessEngine/src/logic/movegen/PawnGenerator.h"

#include "ChessEngine/src/representation/board/PieceType.h"

#include "ChessEngine/src/logic/movegen/utils/Containers.h"
#include "ChessEngine/src/logic/movegen/utils/ChessUtils.h"

namespace logic {
namespace movegen {

PawnGenerator::PawnGenerator(
    const board::Bitboards& bitboards,
    const board::GameStateBitmasks& gameStateBitmasks) 
    : _bitboardsRef(bitboards)
    , _gameStateBitmasksRef(gameStateBitmasks)
{
    _whitePawnStraightMoveBitmasks = masks::getAllStraightPawnMoveBitmasks(true);
    _whitePawnCaptureMoveBitmasks = masks::getAllCapturePawnMoveBitmasks(true);
    _blackPawnStraightMoveBitmasks = masks::getAllStraightPawnMoveBitmasks(false);
    _blackPawnCaptureMoveBitmasks = masks::getAllCapturePawnMoveBitmasks(false);
}

void PawnGenerator::generate(
    bool isWhite,
    Movelist& moveListRef,
    int currentDepth,
    ponder::perft::SearchMemory& searchMemory)
{
    std::vector<int>& pawnIndices = utils::Containers::getPiecePositionIndices();
    std::vector<int>& freeMovesIndices = utils::Containers::getLeapingPiecefreeMovesIndices();
    std::vector<int>& capturableMovesIndices = utils::Containers::getLeapingPieceCapturableMovesIndices();

    utils::getBitIndices(pawnIndices, isWhite ? _bitboardsRef.getWhitePawnsBitboard()
                                              : _bitboardsRef.getBlackPawnsBitboard());

    for (int currentPawnIndex : pawnIndices) {

        bitmask straightPawnMoveBitmask = isWhite ? _whitePawnStraightMoveBitmasks[currentPawnIndex]
                                                  : _blackPawnStraightMoveBitmasks[currentPawnIndex];

        bitmask capturePawnMoveBitmask = isWhite ? _whitePawnCaptureMoveBitmasks[currentPawnIndex]
                                                 : _blackPawnCaptureMoveBitmasks[currentPawnIndex];

        bitmask freePawnMoves = straightPawnMoveBitmask & _gameStateBitmasksRef.getEmptySquaresBitmask();
        
        bitmask enemyPieces = isWhite ? _gameStateBitmasksRef.getBlackPiecesBitmask()
                                      : _gameStateBitmasksRef.getWhitePiecesBitmask();
        
        bitmask enPessantTarget = searchMemory.getEnPessantTargetAtDepth(currentDepth);
        bitmask capturablePawnMoves = capturePawnMoveBitmask & enemyPieces;

        utils::getBitIndices(freeMovesIndices, freePawnMoves);
        utils::getBitIndices(capturableMovesIndices, capturablePawnMoves);

        int offset = isWhite ? 8 : -8;
        bool canPromote = (isWhite && utils::rankFromBitIndex(currentPawnIndex) == 6) || (!isWhite && utils::rankFromBitIndex(currentPawnIndex) == 1);

        if (freeMovesIndices.size() == 2) {
            int singleStepIndex = (isWhite ? 0 : 1);
            int doubleStepIndex = (isWhite ? 1 : 0);

            moveListRef.addMove(move::Move(currentPawnIndex, freeMovesIndices[singleStepIndex], move::Move::QUITE_FLAG));
            moveListRef.addMove(move::Move(currentPawnIndex, freeMovesIndices[doubleStepIndex], move::Move::DOUBLE_PAWN_PUSH_FLAG));

        } else if (freeMovesIndices.size() == 1 && freeMovesIndices[0] == currentPawnIndex + offset) {
            // Only add them move it is direcly in front of the pawn, to avoid jumping over pieces
            if (canPromote) {
                moveListRef.addMove(move::Move(currentPawnIndex, freeMovesIndices[0], move::Move::KNIGHT_PROMO_FLAG));
                moveListRef.addMove(move::Move(currentPawnIndex, freeMovesIndices[0], move::Move::BISHOP_PROMO_FLAG));
                moveListRef.addMove(move::Move(currentPawnIndex, freeMovesIndices[0], move::Move::ROOK_PROMO_FLAG));
                moveListRef.addMove(move::Move(currentPawnIndex, freeMovesIndices[0], move::Move::QUEEN_PROMO_FLAG));
            
            } else {
                moveListRef.addMove(move::Move(currentPawnIndex, freeMovesIndices[0], move::Move::QUITE_FLAG));
            }
        }

        for (int capturablePawnMoveIndex : capturableMovesIndices) {
            if (canPromote) {
                moveListRef.addMove(move::Move(currentPawnIndex, capturablePawnMoveIndex, move::Move::QUEEN_PROMO_CAPTURE_FLAG));
                moveListRef.addMove(move::Move(currentPawnIndex, capturablePawnMoveIndex, move::Move::ROOK_PROMO_CAPTURE_FLAG));
                moveListRef.addMove(move::Move(currentPawnIndex, capturablePawnMoveIndex, move::Move::BISHOP_PROMO_CAPTURE_FLAG));
                moveListRef.addMove(move::Move(currentPawnIndex, capturablePawnMoveIndex, move::Move::KNIGHT_PROMO_CAPTURE_FLAG));
            } else {
                moveListRef.addMove(move::Move(currentPawnIndex, capturablePawnMoveIndex, move::Move::CAPTURE_FLAG));
            }
        }

        if ((capturePawnMoveBitmask & enPessantTarget) != 0) {
            moveListRef.addMove(move::Move(currentPawnIndex, utils::indexOfLSB(capturePawnMoveBitmask & enPessantTarget), move::Move::EP_CAPTURE_FLAG));
        }
    }
}

} // namespace movegen
} // namespace logic