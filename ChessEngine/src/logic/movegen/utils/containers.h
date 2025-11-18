#pragma once

#include <vector>

namespace logic {

class Containers {

public:
    static void init();

    static std::vector<int>& get_sliding_piece_free_moves_indices();
    static std::vector<int>& get_leaping_piece_free_moves_indices();
    static std::vector<int>& get_sliding_piece_capturable_moves_indices();
    static std::vector<int>& get_leaping_piece_capturable_moves_indices();
    static std::vector<int>& get_piece_position_indices();

private:
    static std::vector<int> sliding_piece_free_moves_indices_;
    static std::vector<int> leaping_piece_free_moves_indices_;
    
    static std::vector<int> sliding_piece_capturable_moves_indices_;
    static std::vector<int> leaping_piece_capturable_moves_indices_;

    static std::vector<int> piece_position_indices_;
};

} // namespace logic