#pragma once

#include "ChessEngine/src/engine/search/SearchMemory.h"

#include "ChessEngine/src/logic/evaluation/Evaluator.h"

#include "ChessEngine/src/model/board/ZHasher.h"
#include "ChessEngine/src/model/board/Bitboards.h"
#include "ChessEngine/src/model/board/GameStateBitmasks.h"

#include "ChessEngine/src/model/board/SquaresLookup.h"
#include "ChessEngine/src/logic/movegen/MoveGenerator.h"
#include "ChessEngine/src/model/move/Movelist.h"
#include "ChessEngine/src/logic/makemove/MoveMaker.h"
#include "ChessEngine/src/logic/makemove/MoveRetractor.h"
#include "ChessEngine/src/model/move/Move.h"
#include "ChessEngine/src/io/Fen.h"

namespace engine {

class Searcher {

public:
    static constexpr int MAX_LEGAL_MOVES = 218;

    Searcher(int maxDepth);
    
    void minimax(
        int current_depth,
        bool isMaximizer, 
        int firstMoveIndex,
        bool recPerftStats = true,
        const model::Move& lastMove = model::Move(),
        bool verbose = true
    );

    void recordPerftStats(
        bool isMaximizer,
        int currentDepth,
        int &firstMoveIndex,
        size_t i,
        const model::Move& currentMove,
        bool &retFlag
    );

    void genMoves(
        bool isWhite,
        int currentDepth, 
        unsigned char castlingRights
    );

    void makeMove(model::Move move, bool isWhite, int currentDepth);
    void unmakeMove(model::Move move, bool isWhite, int currentDepth);
    void undoMove();
    
    int _numMoveGenCalls;
    int _totalNodes;

    long sumNodesToDepth(int depth) const;
    std::vector<long> _nodeCountPerFirstMove;
    std::vector<model::Move> _firstMoves;
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
        model::Move currentMove, 
        model::Move lastMove, 
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

    const logic::MoveGenerator& getMoveGenerator() const
    {
        return _moveGenerator;
    }

    void setBoardFromFen(const std::string& fen)
    {
        io::setBoardFromFen(fen, _bitboards, _gameStateBitmasks, _squaresLookup);
    }

    std::string getFenFromBoard() const
    {
        return io::getFenFromBoard(_squaresLookup);
    }

    bool diffBetweenGameStateBitmasks() const
    {
        return (_gameStateBitmasks.getBlackPiecesBitmask() | _gameStateBitmasks.getWhitePiecesBitmask()) != _gameStateBitmasks.getOccupiedPiecesBitmask();
    }
    
private:
    model::Bitboards _bitboards;
    model::SquaresLookup _squaresLookup;
    model::GameStateBitmasks _gameStateBitmasks;
    SearchMemory _searchMemory;
    model::ZHasher _zHasher;
    logic::MoveMaker _moveMaker;
    logic::MoveRetractor _moveRetractor;
    logic::MoveGenerator _moveGenerator;
    logic::Evaluator _evaluator;
    int _maxDepth;

    int _pseudoLegalMovesCount;
    std::vector<model::Movelist> _movelists;
    std::vector<model::PieceType> _lastCapturedPieces;
    std::vector<int> _noCapturedOrPawnMoveCounts; 

    bool tooManyPiecesOnBoard();
};

} // namespace engine