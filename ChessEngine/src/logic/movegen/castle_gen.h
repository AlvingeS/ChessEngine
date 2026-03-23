#pragma once

#include "model/types.h"
namespace model {
    class Move;
    class Movelist;
    class Position;
}

namespace logic {

class MoveMaker;
class MoveRetractor;
class CheckDetection;
struct LegalityInfo;

class CastleGen {
    
public:
    CastleGen(const model::Position& position); 
    void generate(model::Movelist& movelist, const LegalityInfo& legality_info);

private:
    void gen_single_castle_move(
        bool is_kside,
        model::Movelist& movelist,
        const LegalityInfo& legality_info
    );

    bool king_and_rook_on_castle_squares(bool is_kside) const;

    const model::Position& pos_;
};

} // namespace logic