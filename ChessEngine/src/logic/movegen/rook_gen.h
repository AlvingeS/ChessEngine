#pragma once

#include "logic/attack_tables/attack_tables.h"

namespace model {
    class Movelist;
    class Position;
}

namespace logic {

struct LegalityInfo;

class RookGen {

public:
    RookGen(const model::Position& pos);
    
    void generate(model::Movelist& movelist, const LegalityInfo& legality_info);

private:
    const model::Position& pos_;
};

} // namespace logic