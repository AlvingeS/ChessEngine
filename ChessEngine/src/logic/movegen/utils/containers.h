#pragma once

#include <vector>

namespace logic {

class Containers {

public:
    static void init();

    static std::vector<int>& get_sliding_piece_quiet_moves_idxs();
    static std::vector<int>& get_leaping_piece_quiet_moves_idxs();
    static std::vector<int>& get_sliding_piece_capturable_moves_idxs();
    static std::vector<int>& get_leaping_piece_capturable_moves_idxs();
    static std::vector<int>& get_piece_position_idxs();

private:
    static std::vector<int> sliding_piece_quiet_moves_idxs_;
    static std::vector<int> leaping_piece_quiet_moves_idxs_;
    
    static std::vector<int> sliding_piece_capturable_moves_idxs_;
    static std::vector<int> leaping_piece_capturable_moves_idxs_;

    static std::vector<int> piece_position_idxs_;
};

} // namespace logic