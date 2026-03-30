#pragma once

#include "model/types.h"
#include "model/move/move.h"

namespace engine {

enum class TTFlag : std::uint8_t 
{
    NONE,
    EXACT,
    LOWER_BOUND,
    UPPER_BOUND
};

struct TTEntry
{
    uint64_t key;
    eval_t score;
    uint8_t depth;
    TTFlag flag;
    uint16_t best_move_val;
};

class TT
{

public:
    TT();
    const TTEntry* lookup(uint64_t key) const;
    void store(uint64_t key, eval_t score, uint8_t depth, TTFlag flag, uint16_t best_move_val);
    
    void resize(size_t mb);
    void clear();

private:
    std::vector<TTEntry> array_;
    uint64_t index_mask_;

    static constexpr size_t DEFAULT_SIZE_MB = 16;
};

} // namespace engine