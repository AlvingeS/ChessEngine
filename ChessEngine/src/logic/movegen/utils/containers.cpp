#include "logic/movegen/utils/containers.h"

namespace logic {

std::vector<int> Containers::sliding_piece_free_moves_indices_;
std::vector<int> Containers::leaping_piece_free_moves_indices_;
std::vector<int> Containers::sliding_piece_capturable_moves_indices_;
std::vector<int> Containers::leaping_piece_capturable_moves_indices_;
std::vector<int> Containers::piece_position_indices_;

void Containers::init() {
    sliding_piece_free_moves_indices_ = std::vector<int>(32, 0);
    leaping_piece_free_moves_indices_ = std::vector<int>(8, 0);

    sliding_piece_capturable_moves_indices_ = std::vector<int>(32, 0);
    leaping_piece_capturable_moves_indices_ = std::vector<int>(8, 0);

    piece_position_indices_ = std::vector<int>(16, 0);
}

std::vector<int>& Containers::get_sliding_piece_free_moves_indices()
{
    return sliding_piece_free_moves_indices_;
}

std::vector<int>& Containers::get_leaping_piece_free_moves_indices()
{
    return leaping_piece_free_moves_indices_;
}

std::vector<int>& Containers::get_sliding_piece_capturable_moves_indices()
{
    return sliding_piece_capturable_moves_indices_;
}

std::vector<int>& Containers::get_leaping_piece_capturable_moves_indices()
{
    return leaping_piece_capturable_moves_indices_;
}

std::vector<int>& Containers::get_piece_position_indices()
{
    return piece_position_indices_;
}

} // namespace logic