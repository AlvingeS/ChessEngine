#pragma once

#include "model/types.h"

namespace model {
    class Move;
    class Movelist;
    class Board;
    class Bitboards;
    class OccupancyMasks;
}

namespace logic {

class MoveMaker;
class MoveRetractor;
class CheckDetection;

class CastleGen {
    
public:
    CastleGen(
        model::Board& board,
        logic::MoveMaker& move_maker,
        logic::MoveRetractor& move_retractor,
        CheckDetection* check_detection
    );
    
    void generate(
        bool is_w, 
        model::Movelist& movelist, 
        unsigned char castle_rights
    );

private:
    const model::Bitboards& bitboards_;
    const model::OccupancyMasks& occupancy_masks_;
    
    logic::MoveMaker& move_maker_;
    logic::MoveRetractor& move_retractor_;
    
    CheckDetection* check_detection_;

    bitmask w_kside_castle_mask_;
    bitmask w_qside_castle_mask_;
    bitmask b_kside_castle_mask_;
    bitmask b_qside_castle_mask_;

    void gen_single_castle_move(
        bool is_w,
        bool is_kside,
        model::Movelist& movelist
    );

    bool king_and_rook_on_castle_squares(bool is_w, bool is_kside) const;
    void make_temporary_king_move(bool is_w, bool is_kside);
    void revert_temporary_king_move(bool is_w, bool is_kside);
};

} // namespace logic