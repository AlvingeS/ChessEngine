#pragma once

#include "model/types.h"

namespace model {
    class Movelist;
    class Position;
}

namespace logic {

struct LegalityInfo;

class PawnGen {

public:
    PawnGen(const model::Position& position);
    
    void generate(model::Movelist& movelist, const LegalityInfo& legality_info);

private:
    const model::Position& pos_;
};

} // namespace logic