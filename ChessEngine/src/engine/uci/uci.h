#pragma once

#include "engine/search/searcher.h"

#include "model/move/move.h"
#include "model/move/movelist.h"
#include "model/types.h"

#include <string>

namespace engine::uci {

std::string move_to_uci(const model::Move& move);

model::Move parse_uci_move(
    const std::string& str, 
    model::Movelist& legal_moves);

void uci_loop();

} // namespace engine::uci