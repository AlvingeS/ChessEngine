#pragma once

#include "model/types.h"

#include <array>

namespace engine {

static constexpr int PIECE_VALUES[6] = {
    100, 300, 300, 500, 900, 5000
};

static constexpr int mvv_lva(int victim, int attacker)
{
    return PIECE_VALUES[victim] - PIECE_VALUES[attacker];
}

static constexpr auto build_mvv_lva_table()
{
    std::array<std::array<int, 6>, 6> table{};
    for (int vic_idx = 0; vic_idx < 6; vic_idx++) {
        for (int att_idx = 0; att_idx < 6; att_idx++) {
            table[vic_idx][att_idx] = mvv_lva(vic_idx, att_idx);
        }
    }
    return table;
}

static constexpr auto MVV_LVA_TABLE = build_mvv_lva_table();

} // namespace engine