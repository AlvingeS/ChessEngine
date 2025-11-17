#pragma once

#include "engine/perft/perftData.h"
#include "engine/pickmove/SearchMemory.h"

#include "io/Fen.h"

#include "model/position/Board.h"
#include "model/move/Movelist.h"
#include "model/move/Move.h"
#include "model/position/PieceMap.h"

#include "logic/makemove/MoveMaker.h"
#include "logic/makemove/MoveRetractor.h"
#include "logic/movegen/MoveGenerator.h"
#include "logic/eval/Evaluator.h"

namespace engine {

class perft {

public:
    perft(int maxDepth);
    
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
    void unmakeMove(model::Move move, bool isWhite, logic::MoveResult previousMoveResult);
    void undoMove();
    
    int _numMoveGenCalls;
    int _totalNodes;

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
        io::Fen::setBoardFromFen(fen, _bitboards, _stateBitmasks, _pieceMap);
    }

    std::string getFenFromBoard() const
    {
        return io::Fen::getFenFromBoard(_pieceMap);
    }

    bool diffBetweenStateBitmasks() const
    {
        return (_stateBitmasks.getBlackPiecesBitmask() | _stateBitmasks.getWhitePiecesBitmask()) != _stateBitmasks.getOccupiedPiecesBitmask();
    }

    
private:
    int _maxDepth;
    model::Board _board;
    model::Bitboards& _bitboards;
    model::PieceMap& _pieceMap;
    model::StateBitmasks& _stateBitmasks;
    model::ZHasher& _zHasher;
    
    logic::MoveMaker _moveMaker;
    logic::MoveRetractor _moveRetractor;
    logic::MoveGenerator _moveGenerator;
    logic::Evaluator _evaluator;
    
    SearchMemory _searchMemory;
    perftData _perftData;

    int _pseudoLegalMovesCount;
    std::vector<model::Movelist> _movelists;
    std::vector<model::Piece::Type> _lastCapturedPieces;
    std::vector<int> _noCapturedOrPawnMoveCounts; 

    bool tooManyPiecesOnBoard();
};

} // namespace engine