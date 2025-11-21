#include "logic/movegen/utils/containers.h"

namespace logic {

std::vector<int> Containers::sliding_piece_quiet_moves_idxs_;
std::vector<int> Containers::leaping_piece_quiet_moves_idxs_;
std::vector<int> Containers::sliding_piece_capturable_moves_idxs_;
std::vector<int> Containers::leaping_piece_capturable_moves_idxs_;
std::vector<int> Containers::piece_position_idxs_;

void Containers::init() {
    sliding_piece_quiet_moves_idxs_ = std::vector<int>(32, 0);
    leaping_piece_quiet_moves_idxs_ = std::vector<int>(8, 0);

    sliding_piece_capturable_moves_idxs_ = std::vector<int>(32, 0);
    leaping_piece_capturable_moves_idxs_ = std::vector<int>(8, 0);

    piece_position_idxs_ = std::vector<int>(16, 0);
}

std::vector<int>& Containers::get_sliding_piece_quiet_moves_idxs()
{
    return sliding_piece_quiet_moves_idxs_;
}

std::vector<int>& Containers::get_leaping_piece_quiet_moves_idxs()
{
    return leaping_piece_quiet_moves_idxs_;
}

std::vector<int>& Containers::get_sliding_piece_capturable_moves_idxs()
{
    return sliding_piece_capturable_moves_idxs_;
}

std::vector<int>& Containers::get_leaping_piece_capturable_moves_idxs()
{
    return leaping_piece_capturable_moves_idxs_;
}

std::vector<int>& Containers::get_piece_position_idxs()
{
    return piece_position_idxs_;
}

} // namespace logic