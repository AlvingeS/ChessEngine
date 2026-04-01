#pragma once

#include "model/types.h"

namespace model {
    class Movelist;
    class Position;
}

namespace logic {

struct LegalityInfo;

class KnightGen {

public:
    KnightGen(const model::Position& pos);
    void generate(model::Movelist& movelist, const LegalityInfo& legality_info, const bool captures_only);

private:
    const model::Position& pos_;
    attack_table& knight_attack_table_;
};

} // namespace logic