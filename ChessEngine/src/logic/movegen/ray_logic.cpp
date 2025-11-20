#include "logic/movegen/ray_logic.h"

#include "model/move/movelist.h"
#include "model/move/move.h"

#include "logic/movegen/utils/containers.h"
#include "logic/movegen/utils/chess_utils.h"
#include "logic/movegen/utils/bit_basics.h"

namespace logic {

void RayLogic::add_moves_from_free_ray(
    bitmask free_ray,
    int bit_index_from,
    model::Movelist& movelist)
{
    std::vector<int>& free_ray_indices = Containers::get_sliding_piece_free_moves_indices();
    BitBasics::get_bit_indices(free_ray_indices, free_ray);

    for (int bit_index : free_ray_indices) {
        movelist.add_move(model::Move(bit_index_from, bit_index, model::Move::QUITE_FLAG));
    }
}

void RayLogic::add_move_if_blocker_is_enemy(
    int blocker_index,
    bool is_w,
    int bit_index_from,
    model::Movelist& movelist,
    bitmask w_pieces_mask)
{
    bool blocker_is_w = BitBasics::get_bit(w_pieces_mask, blocker_index);

    if (blocker_is_w != is_w)
        movelist.add_move(model::Move(bit_index_from, blocker_index, model::Move::CAPTURE_FLAG));
}

void RayLogic::add_moves_between_blocker_and_piece_on_line_ray(
    int blocker_index,
    bool along_file,
    bool start_from_blocker,
    int rook_rank,
    int rook_file,
    int bit_index_from,
    model::Movelist& movelist)
{
    int start = start_from_blocker 
                ? (along_file ? ChessUtils::file_from_bit_index(blocker_index) 
                              : ChessUtils::rank_from_bit_index(blocker_index)) 
                : (along_file ? rook_file 
                              : rook_rank);
                            
    int stop = start_from_blocker 
               ? (along_file ? rook_file 
                             : rook_rank) 
               : (along_file ? ChessUtils::file_from_bit_index(blocker_index) 
                             : ChessUtils::rank_from_bit_index(blocker_index));

    for (int i = start - 1; i > stop; --i) {
        int rank_or_file_index = along_file ? rook_rank * 8 + i 
                                            : i * 8 + rook_file;
        
        movelist.add_move(model::Move(bit_index_from, rank_or_file_index, model::Move::QUITE_FLAG));
    }
}

void RayLogic::add_moves_between_blocker_and_pice_on_diag_ray(
    int blocker_index,
    bool start_from_blocker,
    int bishop_rank,
    int bishop_file,
    int bit_index_from,
    model::Movelist& movelist)
{
    int start_rank = start_from_blocker
                     ? ChessUtils::rank_from_bit_index(blocker_index)
                     : bishop_rank;

    int start_file = start_from_blocker
                     ? ChessUtils::file_from_bit_index(blocker_index)
                     : bishop_file;

    int stop_rank = start_from_blocker
                    ? bishop_rank
                    : ChessUtils::rank_from_bit_index(blocker_index);

    int stop_file = start_from_blocker 
                    ? bishop_file
                    : ChessUtils::file_from_bit_index(blocker_index);

    int rank_diff = start_rank - stop_rank;
    int file_diff = start_file - stop_file;

    int rank_increment = rank_diff > 0 ? -1 : 1;
    int file_increment = file_diff > 0 ? -1 : 1;

    for (int i = start_rank + rank_increment, j = start_file + file_increment;
         i != stop_rank;
         i += rank_increment, j += file_increment) 
    {
        int rank_or_file_index = i * 8 + j;

        movelist.add_move(model::Move(bit_index_from, rank_or_file_index, model::Move::QUITE_FLAG));
    }
}

void RayLogic::add_moves_from_line_ray(
    bitmask ray,
    bool blocker_on_lsb,
    bool along_file,
    bool is_w,
    int piece_index,
    int piece_rank,
    int piece_file,
    model::Movelist& movelist,
    bitmask w_pieces_mask,
    bitmask occupied_sqrs_mask) 
{
    bitmask blocker_mask = ray & occupied_sqrs_mask;          

    if (blocker_mask != 0) {
        int blocker_index = blocker_on_lsb
                            ? BitBasics::lsb_index(blocker_mask)
                            : BitBasics::msb_index(blocker_mask);
                               
        add_move_if_blocker_is_enemy(
            blocker_index,
            is_w,
            piece_index, 
            movelist,
            w_pieces_mask
        );

        add_moves_between_blocker_and_piece_on_line_ray(
            blocker_index,
            along_file, 
            blocker_on_lsb, 
            piece_rank, 
            piece_file, 
            piece_index, 
            movelist
        );

    } else {
        add_moves_from_free_ray(ray, piece_index, movelist);
    }
}

void RayLogic::add_moves_from_diag_ray(
    bitmask ray,
    bool blocker_on_lsb,
    bool is_w,
    int piece_index,
    int piece_rank,
    int piece_file,
    model::Movelist& movelist,
    bitmask w_pieces_mask,
    bitmask occupied_sqrs_mask)
{
    bitmask blocker_mask = ray & occupied_sqrs_mask;

    if (blocker_mask != 0) {
        int blocker_index = blocker_on_lsb
                            ? BitBasics::lsb_index(blocker_mask) 
                            : BitBasics::msb_index(blocker_mask);

        add_move_if_blocker_is_enemy(
            blocker_index, 
            is_w, 
            piece_index, 
            movelist,
            w_pieces_mask
        );

        add_moves_between_blocker_and_pice_on_diag_ray(
            blocker_index, 
            blocker_on_lsb,
            piece_rank, 
            piece_file, 
            piece_index, 
            movelist
        );

    } else {
        add_moves_from_free_ray(ray, piece_index, movelist);
    }
}

bool RayLogic::check_line_ray(
    bitmask line_ray,
    bool first_blocker_on_lsb,
    bitmask opp_rooks_and_queens_mask,
    bitmask occupied_sqrs_mask) 
{
    bitmask rooks_and_queens_blocker_mask = line_ray & opp_rooks_and_queens_mask;
    
    if (rooks_and_queens_blocker_mask == 0ULL)
        return false;

    bitmask blocker_mask = line_ray & occupied_sqrs_mask;

    if (BitBasics::pop_count(blocker_mask) == 1)
        return true;

    int occupied_blocker_index = first_blocker_on_lsb
                                 ? BitBasics::lsb_index(blocker_mask)
                                 : BitBasics::msb_index(blocker_mask);

    int rooks_and_queens_blocker_index = first_blocker_on_lsb
                                         ? BitBasics::lsb_index(rooks_and_queens_blocker_mask)
                                         : BitBasics::msb_index(rooks_and_queens_blocker_mask);

    return occupied_blocker_index == rooks_and_queens_blocker_index;
}

bool RayLogic::check_diag_ray(
    bitmask diag_ray,
    bool first_blocker_on_lsb,
    bitmask opp_bishops_and_queens_mask,
    bitmask occupied_sqrs_mask)
{
    bitmask bishops_and_queens_blocker_mask = diag_ray & opp_bishops_and_queens_mask;

    if (bishops_and_queens_blocker_mask == 0)
        return false;

    bitmask blocker_mask = diag_ray & occupied_sqrs_mask;

    if (BitBasics::pop_count(blocker_mask) == 1)
        return true;

    int occupied_blocker_index = first_blocker_on_lsb
                                 ? BitBasics::lsb_index(blocker_mask)
                                 : BitBasics::msb_index(blocker_mask);

    int bishops_and_queens_blocker_index = first_blocker_on_lsb
                                          ? BitBasics::lsb_index(bishops_and_queens_blocker_mask)
                                          : BitBasics::msb_index(bishops_and_queens_blocker_mask);

    return occupied_blocker_index == bishops_and_queens_blocker_index;
}

} // namespace logic
