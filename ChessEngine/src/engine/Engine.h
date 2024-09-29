#pragma once

#include "ChessEngine/src/game/board/ZHasher.h"
#include "ChessEngine/src/game/board/Bitboards.h"
#include "ChessEngine/src/game/board/GameStateBitmasks.h"
#include "ChessEngine/src/game/board/SquaresLookup.h"
#include "ChessEngine/src/game/movegen/MoveGenerator.h"
#include "ChessEngine/src/game/movegen/Movelist.h"
#include "ChessEngine/src/game/move/MoveMaker.h"
#include "ChessEngine/src/game/move/MoveRetractor.h"
#include "ChessEngine/src/game/move/Move.h"

#include "ChessEngine/src/ponder/perft/SearchMemory.h"
#include "ChessEngine/src/ponder/evaluation/Evaluator.h"
#include "ChessEngine/src/ponder/perft/Searcher.h"

#include "ChessEngine/src/utils/Fen.h"

namespace engine {

class Engine 
{
public:
    // Default constructor
    Engine(int maxDepth);

    // Constructor with starting FEN
    Engine(int maxDepth, const std::string& fen);

    void resetToFEN(const std::string& fen);

    // Bitboard methods

    // Squares lookup methods

    // GameStateBitmasks methods

    // MoveMaker methods

    // MoveRetractor methods

    // MoveGenerator methods

    // ZHasher methods

    // SearchMemory methods

    // Evaluator methods

    // Searcher methods
    void runPerft(int currentDepth, bool isMaximizer, int firstMoveIndex, bool recPerftStats);
    int getSumNodesToDepth(int depth) const;
    std::vector<long> getNodeCountPerFirstMove() const;
    
    // FIXME: Create a struct that holds all info instead
    std::vector<long> getNodeCount() const;
    std::vector<long> getCaptureCount() const;
    std::vector<long> getEpCaptureCount() const;
    std::vector<long> getCastlingCount() const;
    std::vector<long> getPromotionCount() const;
    std::vector<long> getCheckCount() const;
    std::vector<long> getCheckmateCount() const;
    std::vector<game::move::Move> getFirstMoves() const;
    void setMaxSearchDepth(int depth);
    int getMaxSearchDepth() const;
    
    // Utils
    std::string getFenFromBoard() const;

private:
    // game
    game::board::Bitboards _bitboards;
    game::board::SquaresLookup _squaresLookup;
    game::board::GameStateBitmasks _gameStateBitmasks;
    game::move::MoveMaker _moveMaker;
    game::move::MoveRetractor _moveRetractor;
    game::movegen::MoveGenerator _moveGenerator;
    game::board::ZHasher _zHasher;
    
    // ponder
    ponder::perft::SearchMemory _searchMemory;
    ponder::evaluation::Evaluator _evaluator;
    ponder::perft::Searcher _searcher;
};

}