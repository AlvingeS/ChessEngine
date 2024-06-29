#pragma once

#include "ChessEngine/common.h"

#include "ChessEngine/game/BitBoards.h"
#include "ChessEngine/game/GameStateBitMasks.h"
#include "ChessEngine/game/SquaresLookup.h"


namespace utils {
    void setBoardFromFen(const std::string& fen, game::BitBoards& bitboards, game::GameStateBitMasks& gameStateBitmasks, game::SquaresLookup& squaresLookup);
    std::string& getFenFromBoard(game::SquaresLookup& squaresLookup);
}