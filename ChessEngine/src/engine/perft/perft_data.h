#pragma once

#include "model/move/move.h"

namespace engine {

class PerftData {

public:
    PerftData(int max_depth);

    long sum_nodes_to_depth(int depth) const;
    void reset_node_counts();

    size_t get_num_first_moves() const { return first_moves_.size(); }
    int get_node_count_per_first_move_at(int first_move_idx) const { return node_count_per_first_move_[first_move_idx]; }

    int get_node_count_at(int depth) const;
    int get_capture_count_at(int depth) const;
    int get_ep_capture_count_at(int depth) const;
    int get_castle_rights_at(int depth) const;
    int get_promotion_count_at(int depth) const;
    int get_check_count_at(int depth) const;
    int get_checkmate_count_at(int depth) const;

    void increase_node_count_at(int depth);
    void increase_capture_count_at(int depth);
    void increase_ep_capture_count_at(int depth);
    void increase_castle_count_at(int depth);
    void increase_promo_count_at(int depth);
    void increase_check_count_at(int depth);
    void increase_checkmate_count_at(int depth);

    model::Move get_first_move_at(int first_move_idx) const { return first_moves_[first_move_idx]; }
    void set_first_move_at(int first_move_idx, model::Move currentMove);
    void increase_node_count_per_first_move_at(int first_move_idx);

private:
    int max_depth_;

    std::vector<long> node_count_per_first_move_;
    std::vector<model::Move> first_moves_;
    std::vector<long> node_count_;
    std::vector<long> capture_count_;
    std::vector<long> ep_capture_count_;
    std::vector<long> casle_count_;
    std::vector<long> promo_count_;
    std::vector<long> check_count_;
    std::vector<long> checkmate_count_;
};

} // namespace engine