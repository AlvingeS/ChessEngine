#include "logic/movegen/queen_gen.h"

#include "model/position/board.h"

#include "logic/movegen/ray_logic.h"
#include "logic/movegen/utils/containers.h"
#include "logic/movegen/utils/bit_basics.h"
#include "logic/movegen/utils/chess_utils.h"

namespace logic {

QueenGenerator::QueenGenerator(model::Board& board) 
    : _bitboards(board.bitboards)
    , _stateBitmasks(board.state_bitmasks)
{
    _straightRayBitmasks = RayBitmasks::getAllStraightRayBitmasks();
    _diagonalRayBitmasks = RayBitmasks::getAllDiagonalRayBitmasks();
}

void QueenGenerator::generate(
    bool isWhite,
    model::Movelist& moveList)
{
    std::vector<int>& queenIndices = Containers::getPiecePositionIndices();

    BitBasics::getBitIndices(queenIndices, isWhite ? _bitboards.get_w_queens_bitboard()
                                               : _bitboards.get_b_queens_bitboard());

    for (int currentQueenIndex : queenIndices) {
        RayBitmasks::StraightRays straightRays = _straightRayBitmasks[currentQueenIndex];
        RayBitmasks::DiagonalRays diagonalRays = _diagonalRayBitmasks[currentQueenIndex];
        
        int queenRank = ChessUtils::rankFromBitIndex(currentQueenIndex);
        int queenFile = ChessUtils::fileFromBitIndex(currentQueenIndex);

        RayLogic::addMovesFromStraightRay(
            straightRays.north,
            true,
            false,
            isWhite,
            currentQueenIndex,
            queenRank,
            queenFile,
            moveList,
            _stateBitmasks.get_w_pieces_bitmask(),
            _stateBitmasks.get_occupied_pieces_bitmask()
        );

        RayLogic::addMovesFromStraightRay(
            straightRays.east,
            false,
            true,
            isWhite,
            currentQueenIndex,
            queenRank,
            queenFile,
            moveList,
            _stateBitmasks.get_w_pieces_bitmask(),
            _stateBitmasks.get_occupied_pieces_bitmask()
        );

        RayLogic::addMovesFromStraightRay(
            straightRays.south,
            false,
            false,
            isWhite,
            currentQueenIndex,
            queenRank,
            queenFile,
            moveList,
            _stateBitmasks.get_w_pieces_bitmask(),
            _stateBitmasks.get_occupied_pieces_bitmask()
        );

        RayLogic::addMovesFromStraightRay(
            straightRays.west,
            true,
            true,
            isWhite,
            currentQueenIndex,
            queenRank,
            queenFile,
            moveList,
            _stateBitmasks.get_w_pieces_bitmask(),
            _stateBitmasks.get_occupied_pieces_bitmask()
        );

        RayLogic::addMovesFromDiagonalRay(
            diagonalRays.northEast,
            true,
            isWhite,
            currentQueenIndex,
            queenRank,
            queenFile,
            moveList,
            _stateBitmasks.get_w_pieces_bitmask(),
            _stateBitmasks.get_occupied_pieces_bitmask()
        );

        RayLogic::addMovesFromDiagonalRay(
            diagonalRays.southEast,
            false,
            isWhite,
            currentQueenIndex,
            queenRank,
            queenFile,
            moveList,
            _stateBitmasks.get_w_pieces_bitmask(),
            _stateBitmasks.get_occupied_pieces_bitmask()
        );

        RayLogic::addMovesFromDiagonalRay(
            diagonalRays.southWest,
            false,
            isWhite,
            currentQueenIndex,
            queenRank,
            queenFile,
            moveList,
            _stateBitmasks.get_w_pieces_bitmask(),
            _stateBitmasks.get_occupied_pieces_bitmask()
        );
        
        RayLogic::addMovesFromDiagonalRay(
            diagonalRays.northWest,
            true,
            isWhite,
            currentQueenIndex,
            queenRank,
            queenFile,
            moveList,
            _stateBitmasks.get_w_pieces_bitmask(),
            _stateBitmasks.get_occupied_pieces_bitmask()
        );
    }
}

} // namespace logic