#pragma once

#include "GameState.h"
#include "Board.h"

namespace model
{

struct Position {
    GameState gameState;
    Board board;
};

}