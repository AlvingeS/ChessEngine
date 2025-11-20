#include "logic/movegen/utils/chess_utils.h"

namespace logic {

int ChessUtils::manhattan_distance(int i, int j) 
{
    int rank_diff = rank_from_bit_idx(i) - rank_from_bit_idx(j);
    int file_diff = file_from_bit_idx(i) - file_from_bit_idx(j);
    return abs(rank_diff) + abs(file_diff);
}

}