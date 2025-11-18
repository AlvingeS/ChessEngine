#pragma once

#include "model/position/game_state.h"
#include "model/position/board.h"

namespace model
{

struct Position {
    GameState gameState;
    Board board;
};

}