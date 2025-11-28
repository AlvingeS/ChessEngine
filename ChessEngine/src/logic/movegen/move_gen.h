#pragma once

#include "logic/movegen/pawn_gen.h"
#include "logic/movegen/bishop_gen.h"
#include "logic/movegen/knight_gen.h"
#include "logic/movegen/castle_gen.h"
#include "logic/movegen/rook_gen.h"
#include "logic/movegen/queen_gen.h"
#include "logic/movegen/king_gen.h"
#include "logic/movegen/check_detection.h"
#include "logic/utils.h"

namespace model {
    class Movelist;
    class Position;
}

namespace logic {

class MoveMaker;
class MoveRetractor;

class MoveGen {

public:
    MoveGen(
        const model::Position& position,
        logic::MoveMaker& move_maker,
        logic::MoveRetractor& move_retractor
    );

    void gen_moves(
        bool is_w,
        model::Movelist& movelist
    );
    
    void gen_rook_moves(bool is_w, model::Movelist& movelist);
    void gen_bishop_moves(bool is_w, model::Movelist& movelist);
    void gen_queen_moves(bool is_w, model::Movelist& movelist);
    void gen_knight_moves(bool is_w, model::Movelist& movelist);
    void gen_king_moves(bool is_w, model::Movelist& movelist);
    void gen_pawn_moves(bool is_w, model::Movelist& movelist);
    void gen_castle_moves(bool is_w, model::Movelist& movelist);
    
    bool in_check(bool is_w);

private:
    const model::Position& pos_;    
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