#include "ChessEngine/src/logic/movegen/PawnGenerator.h"

#include "ChessEngine/src/representation/board/PieceType.h"

#include "ChessEngine/src/logic/movegen/utils/Containers.h"
#include "ChessEngine/src/logic/movegen/utils/ChessUtils.h"
#include "ChessEngine/src/logic/movegen/utils/BitBasics.h"

namespace logic {
namespace movegen {

PawnGenerator::PawnGenerator(
    const representation::board::Bitboards& bitboards,
    const representation::board::GameStateBitmasks& gameStateBitmasks) 
    : _bitboardsRef(bitboards)
    , _gameStateBitmasksRef(gameStateBitmasks)
{
    _whitePawnStraightMoveBitmasks = bitmasks::getAllStraightPawnMoveBitmasks(true);
    _whitePawnCaptureMoveBitmasks = bitmasks::getAllCapturePawnMoveBitmasks(true);
    _blackPawnStraightMoveBitmasks = bitmasks::getAllStraightPawnMoveBitmasks(false);
    _blackPawnCaptureMoveBitmasks = bitmasks::getAllCapturePawnMoveBitmasks(false);
}

void PawnGenerator::generate(
    bool isWhite,
    representation::move::Movelist& moveListRef,
    int currentDepth,
    engine::search::SearchMemory& searchMemory)
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

            moveListRef.addMove(representation::move::Move(currentPawnIndex, freeMovesIndices[singleStepIndex], representation::move::Move::QUITE_FLAG));
            moveListRef.addMove(representation::move::Move(currentPawnIndex, freeMovesIndices[doubleStepIndex], representation::move::Move::DOUBLE_PAWN_PUSH_FLAG));

        } else if (freeMovesIndices.size() == 1 && freeMovesIndices[0] == currentPawnIndex + offset) {
            // Only add them move it is direcly in front of the pawn, to avoid jumping over pieces
            if (canPromote) {
                moveListRef.addMove(representation::move::Move(currentPawnIndex, freeMovesIndices[0], representation::move::Move::KNIGHT_PROMO_FLAG));
                moveListRef.addMove(representation::move::Move(currentPawnIndex, freeMovesIndices[0], representation::move::Move::BISHOP_PROMO_FLAG));
                moveListRef.addMove(representation::move::Move(currentPawnIndex, freeMovesIndices[0], representation::move::Move::ROOK_PROMO_FLAG));
                moveListRef.addMove(representation::move::Move(currentPawnIndex, freeMovesIndices[0], representation::move::Move::QUEEN_PROMO_FLAG));
            
            } else {
                moveListRef.addMove(representation::move::Move(currentPawnIndex, freeMovesIndices[0], representation::move::Move::QUITE_FLAG));
            }
        }

        for (int capturablePawnMoveIndex : capturableMovesIndices) {
            if (canPromote) {
                moveListRef.addMove(representation::move::Move(currentPawnIndex, capturablePawnMoveIndex, representation::move::Move::QUEEN_PROMO_CAPTURE_FLAG));
                moveListRef.addMove(representation::move::Move(currentPawnIndex, capturablePawnMoveIndex, representation::move::Move::ROOK_PROMO_CAPTURE_FLAG));
                moveListRef.addMove(representation::move::Move(currentPawnIndex, capturablePawnMoveIndex, representation::move::Move::BISHOP_PROMO_CAPTURE_FLAG));
                moveListRef.addMove(representation::move::Move(currentPawnIndex, capturablePawnMoveIndex, representation::move::Move::KNIGHT_PROMO_CAPTURE_FLAG));
            } else {
                moveListRef.addMove(representation::move::Move(currentPawnIndex, capturablePawnMoveIndex, representation::move::Move::CAPTURE_FLAG));
            }
        }

        if ((capturePawnMoveBitmask & enPessantTarget) != 0) {
            moveListRef.addMove(representation::move::Move(currentPawnIndex, utils::indexOfLSB(capturePawnMoveBitmask & enPessantTarget), representation::move::Move::EP_CAPTURE_FLAG));
        }
    }
}

} // namespace movegen
} // namespace logic