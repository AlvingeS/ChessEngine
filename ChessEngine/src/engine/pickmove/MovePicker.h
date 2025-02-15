#pragma once

#include "ChessEngine/src/engine/pickmove/SearchMemory.h"

#include "ChessEngine/src/logic/eval/Evaluator.h"

#include "ChessEngine/src/model/board/Board.h"

#include "ChessEngine/src/model/board/PieceMap.h"
#include "ChessEngine/src/logic/movegen/MoveGenerator.h"
#include "ChessEngine/src/model/move/Movelist.h"
#include "ChessEngine/src/logic/makemove/MoveMaker.h"
#include "ChessEngine/src/logic/makemove/MoveRetractor.h"
#include "ChessEngine/src/logic/makemove/MoveUtils.h"
#include "ChessEngine/src/model/move/Move.h"
#include "ChessEngine/src/io/Fen.h"

namespace engine {

class MovePicker {

public:
    MovePicker(int maxDepth);
    
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
        bitmask enpessantTarget, 
        unsigned char castlingRights
    );

    logic::MoveResult makeMove(model::Move move, bool isWhite);
    void unmakeMove(model::Move move, bool isWhite, logic::MoveResult prevousMoveResult);
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
        io::setBoardFromFen(fen, _bitboards, _stateBitmasks, _pieceMap);
    }

    std::string getFenFromBoard() const
    {
        return io::getFenFromBoard(_pieceMap);
    }

    bool diffBetweenStateBitmasks() const
    {
        return (_stateBitmasks.getBlackPiecesBitmask() | _stateBitmasks.getWhitePiecesBitmask()) != _stateBitmasks.getOccupiedPiecesBitmask();
    }
    
private:
    model::Board _board;
    model::Bitboards& _bitboards;
    model::PieceMap& _pieceMap;
    model::StateBitmasks& _stateBitmasks;
    model::ZHasher& _zHasher;
    
    SearchMemory _searchMemory;
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