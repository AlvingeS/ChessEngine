#include "engine/search/searcher.h"

#include "io/board_printer.h"
#include "model/constants.h"
#include "logic/masks.h"

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
    if (GameHistory::is_move_irreversible(move, pos_.c_rights != prev_c_rights)) {
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

} // namespace engine