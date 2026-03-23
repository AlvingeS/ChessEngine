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
struct LegalityInfo;

class MoveGen {

public:
    MoveGen(const model::Position& position);

    LegalityInfo gen_moves(model::Movelist& movelist);
    
    void gen_rook_moves(model::Movelist& movelist, const LegalityInfo& legality_info);
    void gen_bishop_moves(model::Movelist& movelist, const LegalityInfo& legality_info);
    void gen_queen_moves(model::Movelist& movelist, const LegalityInfo& legality_info);
    void gen_knight_moves(model::Movelist& movelist, const LegalityInfo& legality_info);
    void gen_king_moves(model::Movelist& movelist, const LegalityInfo& legality_info);
    void gen_pawn_moves(model::Movelist& movelist, const LegalityInfo& legality_info);
    void gen_castle_moves(model::Movelist& movelist, const LegalityInfo& legality_info);
    bool in_check();

private:
    const model::Position& pos_;    
    
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