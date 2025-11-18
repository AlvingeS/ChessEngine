#include "logic/movegen/rook_gen.h"

#include "model/position/board.h"

#include "logic/movegen/ray_logic.h"
#include "logic/movegen/utils/containers.h"
#include "logic/movegen/utils/chess_utils.h"
#include "logic/movegen/utils/bit_basics.h"

namespace logic {

RookGenerator::RookGenerator(model::Board& board)
    : _bitboards(board.bitboards)
    , _stateBitmasks(board.state_bitmasks)
{
    _straightRayBitmasks = RayBitmasks::getAllStraightRayBitmasks();
}

void RookGenerator::generate(
    bool isWhite,
    model::Movelist& moveList)
{
    std::vector<int>& rookIndices = Containers::getPiecePositionIndices();

    RayBitmasks::StraightRays rays;

    BitBasics::getBitIndices(rookIndices, isWhite ? _bitboards.get_w_rooks_bitboard()
                                       : _bitboards.get_b_rooks_bitboard());

    // Loop through all rooks and isolate them
    for (int currentRookIndex : rookIndices) {
        rays = _straightRayBitmasks[currentRookIndex];
        int rookRank = ChessUtils::rankFromBitIndex(currentRookIndex);
        int rookFile = ChessUtils::fileFromBitIndex(currentRookIndex);

        RayLogic::addMovesFromStraightRay(
            rays.north,
            true, 
            false, 
            isWhite, 
            currentRookIndex, 
            rookRank, 
            rookFile, 
            moveList,
            _stateBitmasks.get_w_pieces_bitmask(),
            _stateBitmasks.get_occupied_pieces_bitmask()
        );

        RayLogic::addMovesFromStraightRay(
            rays.east,
            false, 
            true, 
            isWhite, 
            currentRookIndex, 
            rookRank, 
            rookFile, 
            moveList,
            _stateBitmasks.get_w_pieces_bitmask(),
            _stateBitmasks.get_occupied_pieces_bitmask()
        );

        RayLogic::addMovesFromStraightRay(
            rays.south,
            false, 
            false, 
            isWhite, 
            currentRookIndex, 
            rookRank, 
            rookFile, 
            moveList,
            _stateBitmasks.get_w_pieces_bitmask(),
            _stateBitmasks.get_occupied_pieces_bitmask()
        );

        RayLogic::addMovesFromStraightRay(
            rays.west,
            true, 
            true, 
            isWhite, 
            currentRookIndex, 
            rookRank, 
            rookFile, 
            moveList,
            _stateBitmasks.get_w_pieces_bitmask(),
            _stateBitmasks.get_occupied_pieces_bitmask()
        );
    }
}

} // namespace logic