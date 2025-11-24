#pragma once

#include "logic/movegen/pawn_gen.h"
#include "logic/movegen/bishop_gen.h"
#include "logic/movegen/knight_gen.h"
#include "logic/movegen/castle_gen.h"
#include "logic/movegen/rook_gen.h"
#include "logic/movegen/queen_gen.h"
#include "logic/movegen/king_gen.h"
#include "logic/movegen/check_detection.h"
#include "logic/movegen/utils/bit_basics.h"

namespace model {
    class Board;
    class Bitboards;
    class OccupancyMasks;
    class Movelist;
}

namespace engine {
    class SearchMemory;
}

namespace logic {

class MoveMaker;
class MoveRetractor;

class MoveGen {

public:
    MoveGen(
        model::Board& board,
        logic::MoveMaker& move_maker,
        logic::MoveRetractor& move_retractor
    );

    void gen_moves(
        bool is_w,
        model::Movelist& movelist,
        bitmask ep_target_mask,
        unsigned char castle_rights
    );
    
    void gen_rook_moves(bool is_w, model::Movelist& movelist);
    void gen_bishop_moves(bool is_w, model::Movelist& movelist);
    void gen_queen_moves(bool is_w, model::Movelist& movelist);
    void gen_knight_moves(bool is_w, model::Movelist& movelist);
    void gen_king_moves(bool is_w, model::Movelist& movelist);
    
    void gen_pawn_moves(
        bool is_w,
        model::Movelist& movelist,
        bitmask ep_target_mask
    );

    void gen_castle_moves(
        bool is_w,
        model::Movelist& movelist,
        unsigned char castle_rights
    );
    
    bool in_check(bool is_w);

private:
    const model::Bitboards& bbs_;
    const model::OccupancyMasks& occupancy_masks_;
    
    logic::MoveMaker& move_maker_;
    logic::MoveRetractor& move_retractor_;
    
    CheckDetection check_detection_;
    RookGen rook_gen_;
    BishopGen bishop_gen_;
    QueenGen queen_gen_;
    KnightGen knight_gen_;
    KingGen king_gen_;
    PawnGen pawn_gen_;
    CastleGen castle_gen_;
    
};
} // namespace logic