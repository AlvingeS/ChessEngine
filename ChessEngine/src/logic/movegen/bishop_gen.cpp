#include "logic/movegen/bishop_gen.h"

#include "logic/movegen/ray_logic.h"

#include "logic/movegen/utils/containers.h"
#include "logic/movegen/utils/bit_basics.h"
#include "logic/movegen/utils/chess_utils.h"

#include "model/position/piece_type.h"
#include "model/position/board.h"
#include "model/move/move.h"
#include "model/move/movelist.h"

namespace logic {

BishopGen::BishopGen(model::Board& board) 
    : bitboards_(board.bitboards)
    , state_bitmasks_(board.state_bitmasks)
{
    diag_ray_masks = RayBitmasks::get_all_diagonal_ray_bitmasks();
}

void BishopGen::generate(
    bool is_w,
    model::Movelist& movelist)
{
    std::vector<int>& bishop_indices = Containers::get_piece_position_indices();

    BitBasics::get_bit_indices(bishop_indices, is_w ? bitboards_.get_w_bishops_bitboard()
                                         : bitboards_.get_b_bishops_bitboard());

    for (int current_bishop_index : bishop_indices) {
        RayBitmasks::DiagonalRays rays = diag_ray_masks[current_bishop_index];
        int bishopRank = ChessUtils::rank_from_bit_index(current_bishop_index);
        int bishopFile = ChessUtils::file_from_bit_index(current_bishop_index);

        RayLogic::add_moves_from_diag_ray(rays.ne, true, is_w, current_bishop_index, bishopRank, bishopFile, movelist, state_bitmasks_.get_w_pieces_bitmask(), state_bitmasks_.get_occupied_pieces_bitmask());
        RayLogic::add_moves_from_diag_ray(rays.se, false, is_w, current_bishop_index, bishopRank, bishopFile, movelist, state_bitmasks_.get_w_pieces_bitmask(), state_bitmasks_.get_occupied_pieces_bitmask());
        RayLogic::add_moves_from_diag_ray(rays.sw, false, is_w, current_bishop_index, bishopRank, bishopFile, movelist, state_bitmasks_.get_w_pieces_bitmask(), state_bitmasks_.get_occupied_pieces_bitmask());
        RayLogic::add_moves_from_diag_ray(rays.nw, true, is_w, current_bishop_index, bishopRank, bishopFile, movelist, state_bitmasks_.get_w_pieces_bitmask(), state_bitmasks_.get_occupied_pieces_bitmask());
    }
}

} // namespace logic