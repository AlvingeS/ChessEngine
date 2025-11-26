#include "logic/movegen/rays.h"

#include "model/move/movelist.h"
#include "model/move/move.h"

#include "logic/movegen/containers.h"
#include "logic/utils.h"
#include "logic/utils.h"

namespace logic::rays {

void add_moves_from_free_ray(
    bitmask free_ray,
    sq_idx from_sq,
    model::Movelist& movelist)
{
    std::vector<int>& free_ray_sqs = Containers::get_sliding_piece_quiet_moves_idxs();
    utils::get_bit_idxs(free_ray_sqs, free_ray);

    for (sq_idx to_sq : free_ray_sqs) {
        movelist.add_move(model::Move(from_sq, to_sq, model::Move::QUITE_FLAG));
    }
}

void add_move_if_blocker_is_opp(
    sq_idx blocker_sq,
    bool is_w,
    sq_idx from_sq,
    model::Movelist& movelist,
    bitmask w_pieces_mask)
{
    bool blocker_is_w = utils::get_bit(w_pieces_mask, blocker_sq);

    if (blocker_is_w != is_w)
        movelist.add_move(model::Move(from_sq, blocker_sq, model::Move::CAPTURE_FLAG));
}

void add_moves_between_blocker_and_piece_on_line_ray(
    sq_idx blocker_sq,
    bool along_file,
    bool start_from_blocker,
    int rook_rank,
    int rook_file,
    sq_idx from_sq,
    model::Movelist& movelist)
{
    int start = start_from_blocker 
                ? (along_file ? utils::file_from_sq(blocker_sq) 
                              : utils::rank_from_sq(blocker_sq)) 
                : (along_file ? rook_file 
                              : rook_rank);
                            
    int stop = start_from_blocker 
               ? (along_file ? rook_file 
                             : rook_rank) 
               : (along_file ? utils::file_from_sq(blocker_sq) 
                             : utils::rank_from_sq(blocker_sq));

    for (int i = start - 1; i > stop; --i) {
        sq_idx to_sq = along_file ? rook_rank * 8 + i 
                               : i * 8 + rook_file;
        
        movelist.add_move(model::Move(from_sq, to_sq, model::Move::QUITE_FLAG));
    }
}

void add_moves_between_blocker_and_pice_on_diag_ray(
    sq_idx blocker_sq,
    bool start_from_blocker,
    int bishop_rank,
    int bishop_file,
    sq_idx from_sq,
    model::Movelist& movelist)
{
    int start_rank = start_from_blocker
                     ? utils::rank_from_sq(blocker_sq)
                     : bishop_rank;

    int start_file = start_from_blocker
                     ? utils::file_from_sq(blocker_sq)
                     : bishop_file;

    int stop_rank = start_from_blocker
                    ? bishop_rank
                    : utils::rank_from_sq(blocker_sq);

    int stop_file = start_from_blocker 
                    ? bishop_file
                    : utils::file_from_sq(blocker_sq);

    int rank_diff = start_rank - stop_rank;
    int file_diff = start_file - stop_file;

    int rank_increment = rank_diff > 0 ? -1 : 1;
    int file_increment = file_diff > 0 ? -1 : 1;

    for (int i = start_rank + rank_increment, j = start_file + file_increment;
         i != stop_rank;
         i += rank_increment, j += file_increment) 
    {
        sq_idx to_sq = i * 8 + j;

        movelist.add_move(model::Move(from_sq, to_sq, model::Move::QUITE_FLAG));
    }
}

void add_moves_from_line_ray(
    bitmask ray,
    bool blocker_on_lsb,
    bool along_file,
    bool is_w,
    sq_idx piece_sq,
    int piece_rank,
    int piece_file,
    model::Movelist& movelist,
    bitmask w_pieces_mask,
    bitmask occupied_squares_mask) 
{
    bitmask blockers_mask = ray & occupied_squares_mask;          

    if (blockers_mask != 0) {
        sq_idx blocker_sq = blocker_on_lsb
                            ? utils::lsb_idx(blockers_mask)
                            : utils::msb_idx(blockers_mask);
                               
        add_move_if_blocker_is_opp(
            blocker_sq,
            is_w,
            piece_sq, 
            movelist,
            w_pieces_mask
        );

        add_moves_between_blocker_and_piece_on_line_ray(
            blocker_sq,
            along_file, 
            blocker_on_lsb, 
            piece_rank, 
            piece_file, 
            piece_sq, 
            movelist
        );

    } else {
        add_moves_from_free_ray(ray, piece_sq, movelist);
    }
}

void add_moves_from_diag_ray(
    bitmask ray,
    bool blocker_on_lsb,
    bool is_w,
    sq_idx piece_sq,
    int piece_rank,
    int piece_file,
    model::Movelist& movelist,
    bitmask w_pieces_mask,
    bitmask occupied_squares_mask)
{
    bitmask blockers_mask = ray & occupied_squares_mask;

    if (blockers_mask != 0) {
        sq_idx blocker_sq = blocker_on_lsb
                            ? utils::lsb_idx(blockers_mask) 
                            : utils::msb_idx(blockers_mask);

        add_move_if_blocker_is_opp(
            blocker_sq, 
            is_w, 
            piece_sq, 
            movelist,
            w_pieces_mask
        );

        add_moves_between_blocker_and_pice_on_diag_ray(
            blocker_sq, 
            blocker_on_lsb,
            piece_rank, 
            piece_file, 
            piece_sq, 
            movelist
        );

    } else {
        add_moves_from_free_ray(ray, piece_sq, movelist);
    }
}

bool check_line_ray(
    bitmask line_ray,
    bool is_w,
    bitmask opp_rooks_and_queens_mask,
    bitmask occupied_squares_mask) 
{
    bitmask opp_rooks_and_queens_blocker_mask = line_ray & opp_rooks_and_queens_mask;
    
    // If no blocker is an opponent rook or queen, we cannot be in check from line ray
    if (opp_rooks_and_queens_blocker_mask == 0ULL)
        return false;

    bitmask blockers_mask = line_ray & occupied_squares_mask;

    // We know opp_rooks_and_queens_blocker-mask != 0ULL, so if there is only
    // one blocker, then it must be an opponent rook/queen and we must be in check
    if (utils::pop_count(blockers_mask) == 1)
        return true;

    sq_idx first_blocker_sq = is_w ? utils::lsb_idx(blockers_mask) 
                                   : utils::msb_idx(blockers_mask);

    sq_idx first_opp_rook_or_queen_sq = is_w ? utils::lsb_idx(opp_rooks_and_queens_blocker_mask)
                                             : utils::msb_idx(opp_rooks_and_queens_blocker_mask);

    return first_blocker_sq == first_opp_rook_or_queen_sq;
}

bool check_diag_ray(
    bitmask diag_ray,
    bool is_w,
    bitmask opp_bishops_and_queens_mask,
    bitmask occupied_squares_mask)
{
    bitmask opp_bishops_and_queens_blocker_mask = diag_ray & opp_bishops_and_queens_mask;

    // If no blocker is an opponent bishop or queen, we cannot be in check from diag ray
    if (opp_bishops_and_queens_blocker_mask == 0)
        return false;

    bitmask blockers_mask = diag_ray & occupied_squares_mask;

    // We know opp_rooks_and_queens_blocker-mask != 0ULL, so if there is only
    // one blocker, then it must be an opponent rook/queen and we must be in check
    if (utils::pop_count(blockers_mask) == 1)
        return true;

    sq_idx first_blocker_sq = is_w ? utils::lsb_idx(blockers_mask)
                                   : utils::msb_idx(blockers_mask);

    int first_opp_bishop_or_queen_sq = is_w ? utils::lsb_idx(opp_bishops_and_queens_blocker_mask)
                                            : utils::msb_idx(opp_bishops_and_queens_blocker_mask);

    return first_blocker_sq == first_opp_bishop_or_queen_sq;
}

} // namespace logic
