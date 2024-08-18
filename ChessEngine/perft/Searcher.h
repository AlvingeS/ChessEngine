#pragma once

#include "ChessEngine/movegen/MoveGenerator.h"
#include "ChessEngine/evaluation/Evaluator.h"
#include "ChessEngine/board/Bitboards.h"
#include "ChessEngine/board/GameStateBitmasks.h"
#include "ChessEngine/board/SquaresLookup.h"
#include "ChessEngine/move/MoveMaker.h"
#include "ChessEngine/move/Move.h"
#include "ChessEngine/board/ZHasher.h"
#include "ChessEngine/utils/Fen.h"
#include "ChessEngine/movegen/Movelist.h"

#include "SearchMemory.h"

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
        const move::Move& lastMove = move::Move(),
        bool verbose = true
    );

    void recordPerftStats(
        bool isMaximizer,
        int currentDepth,
        int &firstMoveIndex,
        size_t i,
        const move::Move& currentMove,
        bool &retFlag
    );

    void genMoves(
        bool isWhite,
        movegen::Movelist& movelist,
        int currentDepth, 
        unsigned char castlingRights
    );

    void makeMove(move::Move move, bool isWhite, int currentDepth);
    void unmakeMove(move::Move move, bool isWhite, int currentDepth);
    void undoMove();
    
    int _numMoveGenCalls;
    int _totalNodes;

    long sumNodesToDepth(int depth) const;
    std::vector<long> _nodeCountPerFirstMove;
    std::vector<move::Move> _firstMoves;
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
        move::Move currentMove, 
        move::Move lastMove, 
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

    const movegen::MoveGenerator& getMoveGenerator() const
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
    board::Bitboards _bitboards;
    board::SquaresLookup _squaresLookup;
    board::GameStateBitmasks _gameStateBitmasks;
    SearchMemory _searchMemory;
    board::ZHasher _zHasher;
    move::MoveMaker _moveMaker;
    movegen::MoveGenerator _moveGenerator;
    evaluation::Evaluator _evaluator;
    int _maxDepth;

    int _pseudoLegalMovesCount;
    std::vector<movegen::Movelist> _movelists;
    std::vector<board::PieceType> _lastCapturedPieces;
    std::vector<int> _noCapturedOrPawnMoveCounts; 

    bool tooManyPiecesOnBoard();
};

} // namespace perft