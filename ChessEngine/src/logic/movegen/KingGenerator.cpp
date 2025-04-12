#include "ChessEngine/src/logic/movegen/KingGenerator.h"

#include "ChessEngine/src/model/board/Board.h"
#include "ChessEngine/src/model/move/Movelist.h"

#include "ChessEngine/src/logic/movegen/bitmasks/KingBitmasks.h"
#include "ChessEngine/src/logic/movegen/utils/Containers.h"
#include "ChessEngine/src/logic/movegen/utils/BitBasics.h"

namespace logic {

KingGenerator::KingGenerator(model::Board& board) 
    : _bitboards(board.bitboards)
    , _stateBitmasks(board.stateBitmasks)
{
    _kingBitmasks = KingBitmasks::getAllKingBitmasks();
}

void KingGenerator::generate(bool isWhite, model::Movelist& moveList) 
{
    std::vector<int>& kingIndices = Containers::getPiecePositionIndices();
    std::vector<int>& freeMovesIndices = Containers::getLeapingPiecefreeMovesIndices();
    std::vector<int>& capturableMovesIndices = Containers::getLeapingPieceCapturableMovesIndices();
    
    BitBasics::getBitIndices(kingIndices, isWhite ? _bitboards.getWhiteKingBitboard()
                                              : _bitboards.getBlackKingBitboard());

    // TODO: It makes zero sense to have this in a loop
    for (int currentKingIndex : kingIndices) {
        bitmask kingBitmask = _kingBitmasks[currentKingIndex];

        bitmask freeKingMoves = kingBitmask & _stateBitmasks.getEmptySquaresBitmask();
        
        bitmask enemyPieces = isWhite ? _stateBitmasks.getBlackPiecesBitmask() 
                                      : _stateBitmasks.getWhitePiecesBitmask();

        bitmask capturableKingMoves = kingBitmask & enemyPieces;

        BitBasics::getBitIndices(freeMovesIndices, freeKingMoves);
        BitBasics::getBitIndices(capturableMovesIndices, capturableKingMoves);

        for (int freeKingMoveIndex : freeMovesIndices) {
            moveList.addMove(model::Move(currentKingIndex, freeKingMoveIndex, model::Move::QUITE_FLAG));
        }

        for (int capturableKingMoveIndex : capturableMovesIndices) {
            moveList.addMove(model::Move(currentKingIndex, capturableKingMoveIndex, model::Move::CAPTURE_FLAG));
        }
    }
}

} // namespace logic