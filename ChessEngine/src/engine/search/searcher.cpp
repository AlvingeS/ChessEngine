#include "engine/search/searcher.h"

#include "io/board_printer.h"
#include "model/constants.h"
#include "logic/masks.h"
#include "logic/utils.h"

#include <cstdlib>
namespace engine {

Searcher::Searcher() 
    : pos_(model::Position())
    , z_hasher_(pos_)
    , move_maker_(logic::MoveMaker(pos_, z_hasher_))
    , move_retractor_(logic::MoveRetractor(pos_, z_hasher_))
    , move_generator_(logic::MoveGen(pos_))
    , eval_(logic::Eval(pos_))
    , tt_(engine::TT())
    , game_hist_(GameHistory())
{
    move_lists_.resize(MAX_SAFE_DEPTH + 1);
    undo_stack_.resize(MAX_SAFE_DEPTH + 1);
}


void Searcher::make_move(const model::Move& move) {
    castle_rights prev_c_rights = pos_.c_rights;
    move_maker_.make_move(move);

    bool single_pawn_push = false;
    if (pos_.is_w) {
        single_pawn_push = pos_.bbs.get_piece_type_at(move.to()) == PieceType::W_PAWN && move.is_quiet();
    } else {
        single_pawn_push = pos_.bbs.get_piece_type_at(move.to()) == PieceType::B_PAWN && move.is_quiet();
    }

    if (GameHistory::is_move_irreversible(move, single_pawn_push, pos_.c_rights != prev_c_rights)) {
        game_hist_.push_irreversible(z_hasher_.value());
    } else {
        game_hist_.push(z_hasher_.value());
    }
}

void Searcher::reset_stacks() {
    for (long long i = 0; i < static_cast<long long>(move_lists_.size()); i++) {
        move_lists_[i] = model::Movelist();
    }

    for (long long i = 0; i < static_cast<long long>(undo_stack_.size()); i++) {
        undo_stack_[i] = logic::UndoInfo();
    }
}

model::Movelist Searcher::gen_moves() {
    auto ml = model::Movelist();
    move_generator_.gen_moves(ml, false);
    return ml;
}

void Searcher::set_pos_from_fen(const std::string& fen) {
    clear_game_hist();
    io::fen::set_pos_from_fen(fen, pos_);
}

bool Searcher::has_non_pawn_material() const
{
    if (pos_.is_w) {
        return 
            pos_.bbs.get(PieceType::W_BISHOP) != 0ULL ||
            pos_.bbs.get(PieceType::W_KNIGHT) != 0ULL ||
            pos_.bbs.get(PieceType::W_ROOK)   != 0ULL ||
            pos_.bbs.get(PieceType::W_QUEEN)  != 0ULL;
    } else {
        return 
            pos_.bbs.get(PieceType::B_BISHOP) != 0ULL ||
            pos_.bbs.get(PieceType::B_KNIGHT) != 0ULL ||
            pos_.bbs.get(PieceType::B_ROOK)   != 0ULL ||
            pos_.bbs.get(PieceType::B_QUEEN)  != 0ULL;
    }
}

void Searcher::make_null_move()
{
    pos_.is_w = !pos_.is_w;
    z_hasher_.toggle_side_to_move();

    if (pos_.ep_target_mask != 0ULL)
        z_hasher_.xor_ep_file_at(logic::utils::file_from_sq(logic::utils::lsb_idx(pos_.ep_target_mask)));

    pos_.ep_target_mask = 0ULL;
}

void Searcher::undo_null_move(bitmask ep_target_mask)
{
    pos_.is_w = !pos_.is_w;
    z_hasher_.toggle_side_to_move();

    if (ep_target_mask != 0ULL)
        z_hasher_.xor_ep_file_at(logic::utils::file_from_sq(logic::utils::lsb_idx(ep_target_mask)));
    
    pos_.ep_target_mask = ep_target_mask;
}

} // namespace engine