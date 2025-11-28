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

class CastleGen {
    
public:
    CastleGen(
        const model::Position& position,
        logic::MoveMaker& move_maker,
        logic::MoveRetractor& move_retractor,
        CheckDetection* check_detection
    );
    
    void generate(bool is_w, model::Movelist& movelist);

private:
    void gen_single_castle_move(
        bool is_w,
        bool is_kside,
        model::Movelist& movelist
    );
    bool king_and_rook_on_castle_squares(bool is_w, bool is_kside) const;
    void make_temporary_king_move(bool is_w, bool is_kside);
    void revert_temporary_king_move(bool is_w, bool is_kside);

    const model::Position& pos_;
    logic::MoveMaker& move_maker_;
    logic::MoveRetractor& move_retractor_;
    CheckDetection* check_detection_;
};

} // namespace logic