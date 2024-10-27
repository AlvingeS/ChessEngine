#pragma once

#include "ChessEngine/src/engine/search/SearchMemory.h"

#include "ChessEngine/src/logic/evaluation/Evaluator.h"

#include "ChessEngine/src/representation/board/ZHasher.h"
#include "ChessEngine/src/representation/board/Bitboards.h"
#include "ChessEngine/src/representation/board/GameStateBitmasks.h"

#include "ChessEngine/src/representation/board/SquaresLookup.h"
#include "ChessEngine/src/logic/movegen/MoveGenerator.h"
#include "ChessEngine/src/representation/move/Movelist.h"
#include "ChessEngine/src/logic/makemove/MoveMaker.h"
#include "ChessEngine/src/logic/makemove/MoveRetractor.h"
#include "ChessEngine/src/representation/move/Move.h"
#include "ChessEngine/src/io/Fen.h"

namespace engine {
namespace search {

class Searcher {

public:
    static constexpr int MAX_LEGAL_MOVES = 218;

    Searcher(int maxDepth);
    
    void minimax(
        int current_depth,
        bool isMaximizer, 
        int firstMoveIndex,
        bool recPerftStats = true,
        const representation::move::Move& lastMove = representation::move::Move(),
        bool verbose = true
    );

    void recordPerftStats(
        bool isMaximizer,
        int currentDepth,
        int &firstMoveIndex,
        size_t i,
        const representation::move::Move& currentMove,
        bool &retFlag
    );

    void genMoves(
        bool isWhite,
        int currentDepth, 
        unsigned char castlingRights
    );

    void makeMove(representation::move::Move move, bool isWhite, int currentDepth);
    void unmakeMove(representation::move::Move move, bool isWhite, int currentDepth);
    void undoMove();
    
    int _numMoveGenCalls;
    int _totalNodes;

    long sumNodesToDepth(int depth) const;
    std::vector<long> _nodeCountPerFirstMove;
    std::vector<representation::move::Move> _firstMoves;
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
        representation::move::Move currentMove, 
        representation::move::Move lastMove, 
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

    const logic::movegen::MoveGenerator& getMoveGenerator() const
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
    representation::board::Bitboards _bitboards;
    representation::board::SquaresLookup _squaresLookup;
    representation::board::GameStateBitmasks _gameStateBitmasks;
    SearchMemory _searchMemory;
    representation::board::ZHasher _zHasher;
    logic::makemove::MoverMaker _moveMaker;
    logic::makemove::MoveMaker _moveRetractor;
    logic::movegen::MoveGenerator _moveGenerator;
    logic::evaluation::Evaluator _evaluator;
    int _maxDepth;

    int _pseudoLegalMovesCount;
    std::vector<representation::move::Movelist> _movelists;
    std::vector<representation::board::PieceType> _lastCapturedPieces;
    std::vector<int> _noCapturedOrPawnMoveCounts; 

    bool tooManyPiecesOnBoard();
};

} // namespace search
} // namespace engine