#pragma once

#include "ChessEngine/movegen/MoveGenerator.h"
#include "ChessEngine/evaluation/Evaluator.h"
#include "ChessEngine/board/BitBoards.h"
#include "ChessEngine/board/GameStateBitMasks.h"
#include "ChessEngine/board/SquaresLookup.h"
#include "ChessEngine/move/MoveMaker.h"
#include "ChessEngine/move/Move.h"
#include "ChessEngine/board/ZHasher.h"
#include "ChessEngine/utils/Fen.h"

#include "SearchMemory.h"

namespace perft {

struct MoveScore {
    move::Move move;
    float score;
};

struct MoveList {
    std::vector<move::Move> moves;
    size_t numMoves;
};

class Searcher {

public:
    static constexpr int MAX_LEGAL_MOVES = 218;

    Searcher(int maxDepth);
    
    void minimax(
        int current_depth,
        bool isMaximizer, 
        int firstMoveIndex,
        bool recPerftStats = true,
        move::Move lastMove = move::Move(),
        bool verbose = true
    );

    void recordPerftStats(
        bool isMaximizer,
        int currentDepth,
        int &firstMoveIndex,
        size_t i,
        move::Move &currentMove,
        bool &retFlag
    );

    void genMoves(
        bool isWhite,
        std::vector<move::Move> &moveList,
        int currentDepth, 
        unsigned char castlingRights
    );

    void makeMove(move::Move move, bool isWhite, int currentDepth);
    void unmakeMove(move::Move move, bool isWhite, int currentDepth);
    void undoMove();
    
    int _numMoveGenCalls;
    int _totalNodes;

    long sumNodesToDepth(int depth);
    std::vector<long> _nodeCountPerFirstMove;
    std::vector<move::Move> _firstMoves;
    std::vector<long> _nodeCount;
    std::vector<long> _captureCount;
    std::vector<long> _epCaptureCount;
    std::vector<long> _castlingCount;
    std::vector<long> _promotionCount;
    std::vector<long> _checkCount;
    std::vector<long> _checkmateCount;

    void debugPrint(bool verbose);
    
    bool checkCondition(
        int currentDepth,
        bool isMaximizer, 
        int firstMoveIndex, 
        move::Move currentMove, 
        move::Move lastMove, 
        bool verbose, 
        size_t i
    );

    void setMaxDepth(int maxDepth) 
    {
        _maxDepth = maxDepth;
    }

    int getMaxDepth() {
        return _maxDepth;
    }

    movegen::MoveGenerator& getMoveGenerator() 
    {
        return _moveGenerator;
    }

    void setBoardFromFen(const std::string& fen) 
    {
        utils::setBoardFromFen(fen, _bitboards, _gameStateBitMasks, _squaresLookup);
    }

    std::string& getFenFromBoard() 
    {
        return utils::getFenFromBoard(_squaresLookup);
    }

    bool diffBetweenGameStateBitMasks() 
    {
        return (_gameStateBitMasks.getBlackPiecesBitmask() | _gameStateBitMasks.getWhitePiecesBitmask()) != _gameStateBitMasks.getOccupiedPiecesBitmask();
    }
    
private:
    board::BitBoards _bitboards;
    board::SquaresLookup _squaresLookup;
    board::GameStateBitMasks _gameStateBitMasks;
    SearchMemory _searchMemory;
    board::ZHasher _zHasher;
    move::BitBoardUpdater _bitBoardUpdater;
    move::BitMaskUpdater _bitMaskUpdater;
    move::LookupUpdater _lookupUpdater;
    move::MoveMaker _moveMaker;
    movegen::MoveGenerator _moveGenerator;
    evaluation::Evaluator _evaluator;
    int _maxDepth;

    int _pseudoLegalMovesCount;
    std::vector<std::vector<move::Move>> _moveLists;
    std::vector<board::PieceType> _lastCapturedPieces;
    std::vector<int> _noCapturedOrPawnMoveCounts; 

    bool tooManyPiecesOnBoard();
};

} // namespace perft