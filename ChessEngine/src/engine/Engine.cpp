#include "ChessEngine/src/engine/Engine.h"

namespace engine {

Engine::Engine(int maxDepth) 
    : _bitboards(game::board::Bitboards())
    , _squaresLookup(game::board::SquaresLookup(_bitboards))
    , _gameStateBitmasks(game::board::GameStateBitmasks(_bitboards))
    , _searchMemory(ponder::perft::SearchMemory(maxDepth))
    , _zHasher(game::board::ZHasher(_squaresLookup))
    , _moveMaker(game::move::MoveMaker(_bitboards, _gameStateBitmasks, _squaresLookup, _searchMemory, _zHasher))
    , _moveRetractor(game::move::MoveRetractor(_bitboards, _gameStateBitmasks, _squaresLookup, _searchMemory, _zHasher))
    , _moveGenerator(game::movegen::MoveGenerator(_bitboards, _gameStateBitmasks, _moveMaker, _moveRetractor))
    , _evaluator(ponder::evaluation::Evaluator(_bitboards))
    , _searcher(ponder::perft::Searcher(maxDepth, _bitboards, _squaresLookup, _gameStateBitmasks, _searchMemory, _zHasher, _moveMaker, _moveRetractor, _moveGenerator, _evaluator))
{}

Engine::Engine(int maxDepth, const std::string& fen) 
    : _bitboards(game::board::Bitboards())
    , _squaresLookup(game::board::SquaresLookup(_bitboards))
    , _gameStateBitmasks(game::board::GameStateBitmasks(_bitboards))
    , _searchMemory(ponder::perft::SearchMemory(maxDepth))
    , _zHasher(game::board::ZHasher(_squaresLookup))
    , _moveMaker(game::move::MoveMaker(_bitboards, _gameStateBitmasks, _squaresLookup, _searchMemory, _zHasher))
    , _moveRetractor(game::move::MoveRetractor(_bitboards, _gameStateBitmasks, _squaresLookup, _searchMemory, _zHasher))
    , _moveGenerator(game::movegen::MoveGenerator(_bitboards, _gameStateBitmasks, _moveMaker, _moveRetractor))
    , _evaluator(ponder::evaluation::Evaluator(_bitboards))
    , _searcher(ponder::perft::Searcher(maxDepth, _bitboards, _squaresLookup, _gameStateBitmasks, _searchMemory, _zHasher, _moveMaker, _moveRetractor, _moveGenerator, _evaluator))
{}

void Engine::resetToFEN(const std::string& fen)
{
    utils::setBoardFromFen(fen, _bitboards, _gameStateBitmasks, _squaresLookup);
    _zHasher.computeHash();
    _searcher.resetNodeCounts();
}

void Engine::runPerft(int currentDepth, bool isMaximizer, int firstMoveIndex, bool recPerftStats)
{
    _searcher.minimax(currentDepth, isMaximizer, firstMoveIndex, recPerftStats);    
}

int Engine::getSumNodesToDepth(int depth) const
{
    return _searcher.sumNodesToDepth(depth);
}

std::vector<long> Engine::getNodeCountPerFirstMove() const
{
    return _searcher._nodeCountPerFirstMove;
}

std::vector<long> Engine::getNodeCount() const
{
    return _searcher._nodeCount;
}

std::vector<long> Engine::getCaptureCount() const
{
    return _searcher._captureCount;
}

std::vector<long> Engine::getEpCaptureCount() const
{
    return _searcher._epCaptureCount;
}

std::vector<long> Engine::getCastlingCount() const
{
    return _searcher._castlingCount;
}

std::vector<long> Engine::getPromotionCount() const
{
    return _searcher._promotionCount;
}

std::vector<long> Engine::getCheckCount() const
{
    return _searcher._checkCount;
}

std::vector<long> Engine::getCheckmateCount() const
{
    return _searcher._checkmateCount;
}

std::vector<game::move::Move> Engine::getFirstMoves() const
{
    return _searcher._firstMoves;
}

void Engine::setMaxSearchDepth(int depth)
{
    _searcher.setMaxDepth(depth);
}

int Engine::getMaxSearchDepth() const
{
    return _searcher.getMaxDepth();
}

std::string Engine::getFenFromBoard() const
{
    return utils::getFenFromBoard(_squaresLookup);
}



} // namespace engine