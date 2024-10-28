#pragma once

#include "ChessEngine/src/engine/perft/perftData.h"
#include "ChessEngine/src/engine/search/SearchMemory.h"

#include "ChessEngine/src/io/Fen.h"

#include "ChessEngine/src/model/board/ZHasher.h"
#include "ChessEngine/src/model/board/Bitboards.h"
#include "ChessEngine/src/model/board/GameStateBitmasks.h"
#include "ChessEngine/src/model/move/Movelist.h"
#include "ChessEngine/src/model/move/Move.h"
#include "ChessEngine/src/model/board/SquaresLookup.h"

#include "ChessEngine/src/logic/makemove/MoveMaker.h"
#include "ChessEngine/src/logic/makemove/MoveRetractor.h"
#include "ChessEngine/src/logic/movegen/MoveGenerator.h"
#include "ChessEngine/src/logic/evaluation/Evaluator.h"

namespace engine {

class perft {

public:
    perft (
        int maxDepth,
        model::Bitboards& bitboards,
        model::SquaresLookup& squaresLookup,
        model::GameStateBitmasks& gameStateBitmasks,
        model::ZHasher& zHasher,
        logic::MoveMaker& moveMaker,
        logic::MoveRetractor& moveRetractor,
        logic::MoveGenerator& moveGenerator,
        engine::search::SearchMemory& searchMemory,
        logic::Evaluator& evaluator,
        perftData& perftData
    );
    
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
        return _moveGeneratorRef;
    }

    void setBoardFromFen(const std::string& fen)
    {
        io::setBoardFromFen(fen, _bitboardsRef, _gameStateBitmasksRef, _squaresLookupRef);
    }

    std::string getFenFromBoard() const
    {
        return io::getFenFromBoard(_squaresLookupRef);
    }

    bool diffBetweenGameStateBitmasks() const
    {
        return (_gameStateBitmasksRef.getBlackPiecesBitmask() | _gameStateBitmasksRef.getWhitePiecesBitmask()) != _gameStateBitmasksRef.getOccupiedPiecesBitmask();
    }

    
private:
    int _maxDepth;
    model::Bitboards& _bitboardsRef;
    model::SquaresLookup& _squaresLookupRef;
    model::GameStateBitmasks& _gameStateBitmasksRef;
    model::ZHasher& _zHasherRef;
    
    logic::MoveMaker& _moveMakerRef;
    logic::MoveRetractor& _moveRetractorRef;
    logic::MoveGenerator& _moveGeneratorRef;
    logic::Evaluator& _evaluatorRef;
    
    search::SearchMemory& _searchMemoryRef;
    perftData& _perftDataRef;

    int _pseudoLegalMovesCount;
    std::vector<model::Movelist> _movelists;
    std::vector<model::PieceType> _lastCapturedPieces;
    std::vector<int> _noCapturedOrPawnMoveCounts; 

    bool tooManyPiecesOnBoard();
};

} // namespace engine