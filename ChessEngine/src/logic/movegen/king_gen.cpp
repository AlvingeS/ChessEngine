#include "logic/movegen/king_gen.h"

#include "model/position/board.h"
#include "model/move/movelist.h"

#include "logic/movegen/bitmasks/king_bitmasks.h"
#include "logic/movegen/utils/containers.h"
#include "logic/movegen/utils/bit_basics.h"

namespace logic {

KingGenerator::KingGenerator(model::Board& board) 
    : _bitboards(board.bitboards)
    , _stateBitmasks(board.state_bitmasks)
{
    _kingBitmasks = KingBitmasks::getAllKingBitmasks();
}

void KingGenerator::generate(bool isWhite, model::Movelist& moveList) 
{
    std::vector<int>& kingIndices = Containers::getPiecePositionIndices();
    std::vector<int>& freeMovesIndices = Containers::getLeapingPiecefreeMovesIndices();
    std::vector<int>& capturableMovesIndices = Containers::getLeapingPieceCapturableMovesIndices();
    
    BitBasics::getBitIndices(kingIndices, isWhite ? _bitboards.get_w_king_bitboard()
                                              : _bitboards.get_b_king_bitboard());

    // TODO: It makes zero sense to have this in a loop
    for (int currentKingIndex : kingIndices) {
        bitmask kingBitmask = _kingBitmasks[currentKingIndex];

        bitmask freeKingMoves = kingBitmask & _stateBitmasks.get_empty_squares_bitmask();
        
        bitmask enemyPieces = isWhite ? _stateBitmasks.get_b_pieces_bitmask() 
                                      : _stateBitmasks.get_w_pieces_bitmask();

        bitmask capturableKingMoves = kingBitmask & enemyPieces;

        BitBasics::getBitIndices(freeMovesIndices, freeKingMoves);
        BitBasics::getBitIndices(capturableMovesIndices, capturableKingMoves);

        for (int freeKingMoveIndex : freeMovesIndices) {
            moveList.add_move(model::Move(currentKingIndex, freeKingMoveIndex, model::Move::QUITE_FLAG));
        }

        for (int capturableKingMoveIndex : capturableMovesIndices) {
            moveList.add_move(model::Move(currentKingIndex, capturableKingMoveIndex, model::Move::CAPTURE_FLAG));
        }
    }
}

} // namespace logic