#pragma once

#include "engine/pickmove/search_memory.h"

#include "logic/eval/eval.h"

#include "model/position/board.h"

#include "model/position/piece_map.h"
#include "logic/movegen/move_gen.h"
#include "model/move/movelist.h"
#include "logic/makemove/move_maker.h"
#include "logic/makemove/move_retractor.h"
#include "logic/makemove/move_utils.h"
#include "model/move/move.h"
#include "io/fen.h"

#include <string>

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

    void gen_moves(
        bool is_w,
        int currentDepth,
        bitmask ep_target_mask, 
        unsigned char castle_rights
    );

    logic::MoveResult makeMove(model::Move move, bool is_w);
    void unmakeMove(model::Move move, bool is_w, logic::MoveResult prevousMoveResult);
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
        io::Fen::setBoardFromFen(fen, bitboards_, state_bitmasks_, piece_map_);
    }

    std::string getFenFromBoard() const
    {
        return io::Fen::getFenFromBoard(piece_map_);
    }

    bool diffBetweenStateBitmasks() const
    {
        return (state_bitmasks_.get_b_pieces_bitmask() | state_bitmasks_.get_w_pieces_bitmask()) != state_bitmasks_.get_occupied_pieces_bitmask();
    }
    
private:
    model::Board _board;
    model::Bitboards& bitboards_;
    model::PieceMap& piece_map_;
    model::StateBitmasks& state_bitmasks_;
    model::ZHasher& _zHasher;
    
    SearchMemory _searchMemory;
    logic::MoveMaker move_maker_;
    logic::MoveRetractor move_retractor_;
    logic::MoveGenerator _moveGenerator;
    logic::Evaluator _evaluator;
    int _maxDepth;

    int _pseudoLegalMovesCount;
    std::vector<model::Movelist> _movelists;
    std::vector<model::Piece::Type> _lastCapturedPieces;
    std::vector<int> _noCapturedOrPawnMoveCounts; 

    bool tooManyPiecesOnBoard();
};

} // namespace engine