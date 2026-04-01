#pragma once

#include "model/types.h"

namespace model {
    class Position;
    class Movelist;
}

namespace logic {

struct LegalityInfo;

class KingGen {

public:
    KingGen(const model::Position& pos);

    void generate(model::Movelist& movelist, const LegalityInfo& legality_info, const bool captures_only);

private:
    const model::Position& pos_;    
    attack_table& king_attack_table_;
};

} // namespace logic