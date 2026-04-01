#include "engine/search/transposition_table.h"

namespace engine {

TT::TT()
{
    resize(DEFAULT_SIZE_MB);
}

void TT::resize(size_t mb)
{
    // We want to index keys by and-gating the key with 0x11111...
    // Therefor, we want the size of the array (number of possible entries) to be equal to a power of two
    // and the index_mask a power of 2 - 1 (The power of 2 is 1000..., thus the power of 2 - 1 is 01111...)
    size_t bytes = 1024 * 1024 * mb;
    size_t n_entries = bytes / sizeof(TTEntry);

    // We want to find the largest power of 2 which is still smaller than or equal to n_entries
    // this will be our mask. We left shift 1 until we get a number that is greater or equal to n_entries
    // then we shift it back one step. Every left shift means a greater power of 2
    size_t array_size = 1ULL;
    while (array_size <= n_entries) {
        array_size <<= 1; 
    }
    array_size >>= 1;

    index_mask_ = array_size - 1;

    array_.resize(array_size);
    clear();
}

void TT::clear()
{
    array_.assign(array_.size(), TTEntry{});
}

void TT::store(uint64_t key, eval_t score, uint8_t depth, TTFlag flag, uint16_t best_move_val)
{
    array_[key & index_mask_] = TTEntry{
        key, score, depth, flag, best_move_val
    };
}

const TTEntry* TT::lookup(uint64_t key) const
{
    auto entry = &array_[key & index_mask_];
    
    // If the stored key and the key we called with is the same, it is a transposition
    // Otherwise it is a collision caused by the lower bits of the keys being the same
    if (entry->key == key) {
        return entry;
    } else {
        return nullptr;
    }
}

} // namespace engine