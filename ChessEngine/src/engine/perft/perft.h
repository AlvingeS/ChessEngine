#pragma once

#include "engine/perft/perft_data.h"
#include "engine/pickmove/search_memory.h"

#include "io/fen.h"

#include "model/position/board.h"
#include "model/move/movelist.h"
#include "model/move/move.h"
#include "model/position/piece_map.h"

#include "logic/makemove/move_maker.h"
#include "logic/makemove/move_retractor.h"
#include "logic/movegen/move_gen.h"
#include "logic/eval/eval.h"

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
        io::Fen::setBoardFromFen(fen, _bitboards, _stateBitmasks, piece_map_);
    }

    std::string getFenFromBoard() const
    {
        return io::Fen::getFenFromBoard(piece_map_);
    }

    bool diffBetweenStateBitmasks() const
    {
        return (_stateBitmasks.get_b_pieces_bitmask() | _stateBitmasks.get_w_pieces_bitmask()) != _stateBitmasks.get_occupied_pieces_bitmask();
    }

    
private:
    int _maxDepth;
    model::Board _board;
    model::Bitboards& _bitboards;
    model::PieceMap& piece_map_;
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