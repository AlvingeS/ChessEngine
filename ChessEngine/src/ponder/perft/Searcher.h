#pragma once

#include "ChessEngine/src/ponder/perft/SearchMemory.h"
#include "ChessEngine/src/ponder/evaluation/Evaluator.h"

#include "ChessEngine/src/game/board/ZHasher.h"
#include "ChessEngine/src/game/board/Bitboards.h"
#include "ChessEngine/src/game/board/GameStateBitmasks.h"
#include "ChessEngine/src/game/board/SquaresLookup.h"
#include "ChessEngine/src/game/movegen/MoveGenerator.h"
#include "ChessEngine/src/game/movegen/Movelist.h"
#include "ChessEngine/src/game/move/MoveMaker.h"
#include "ChessEngine/src/game/move/MoveRetractor.h"
#include "ChessEngine/src/game/move/Move.h"
#include "ChessEngine/src/utils/Fen.h"

namespace ponder {
namespace perft {

class Searcher {

public:
    static constexpr int MAX_LEGAL_MOVES = 218;

    Searcher(int maxDepth);
    
    void minimax(
        int current_depth,
        bool isMaximizer, 
        int firstMoveIndex,
        bool recPerftStats = true,
        const game::move::Move& lastMove = game::move::Move(),
        bool verbose = true
    );

    void recordPerftStats(
        bool isMaximizer,
        int currentDepth,
        int &firstMoveIndex,
        size_t i,
        const game::move::Move& currentMove,
        bool &retFlag
    );

    void genMoves(
        bool isWhite,
        int currentDepth, 
        unsigned char castlingRights
    );

    void makeMove(game::move::Move move, bool isWhite, int currentDepth);
    void unmakeMove(game::move::Move move, bool isWhite, int currentDepth);
    void undoMove();
    
    int _numMoveGenCalls;
    int _totalNodes;

    long sumNodesToDepth(int depth) const;
    std::vector<long> _nodeCountPerFirstMove;
    std::vector<game::move::Move> _firstMoves;
    std::vector<long> _nodeCount;
    std::vector<long> _captureCount;
    std::vector<long> _epCaptureCount;
    std::vector<long> _castlingCount;
    std::vector<long> _promotionCount;
    std::vector<long> _checkCount;
    std::vector<long> _checkmateCount;

    void debugPrint(bool verbose) const;
    
    bool checkCondition(
        int currentDepth,
        bool isMaximizer, 
        int firstMoveIndex, 
        game::move::Move currentMove, 
        game::move::Move lastMove, 
        bool verbose, 
        size_t i
    ) const;

    void setMaxDepth(int maxDepth) 
    {
        _maxDepth = maxDepth;
    }

    int getMaxDepth() const {
        return _maxDepth;
    }

    const game::movegen::MoveGenerator& getMoveGenerator() const
    {
        return _moveGenerator;
    }

    void setBoardFromFen(const std::string& fen)
    {
        utils::setBoardFromFen(fen, _bitboards, _gameStateBitmasks, _squaresLookup);
    }

    std::string getFenFromBoard() const
    {
        return utils::getFenFromBoard(_squaresLookup);
    }

    bool diffBetweenGameStateBitmasks() const
    {
        return (_gameStateBitmasks.getBlackPiecesBitmask() | _gameStateBitmasks.getWhitePiecesBitmask()) != _gameStateBitmasks.getOccupiedPiecesBitmask();
    }
    
private:
    game::board::Bitboards _bitboards;
    game::board::SquaresLookup _squaresLookup;
    game::board::GameStateBitmasks _gameStateBitmasks;
    SearchMemory _searchMemory;
    game::board::ZHasher _zHasher;
    game::move::MoveMaker _moveMaker;
    game::move::MoveRetractor _moveRetractor;
    game::movegen::MoveGenerator _moveGenerator;
    evaluation::Evaluator _evaluator;
    int _maxDepth;

    int _pseudoLegalMovesCount;
    std::vector<game::movegen::Movelist> _movelists;
    std::vector<game::board::PieceType> _lastCapturedPieces;
    std::vector<int> _noCapturedOrPawnMoveCounts; 

    bool tooManyPiecesOnBoard();
};

} // namespace perft
} // namespace ponder