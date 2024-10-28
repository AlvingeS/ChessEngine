#pragma once

#include "ChessEngine/src/engine/perft/perftData.h"
#include "ChessEngine/src/engine/search/SearchMemory.h"

#include "ChessEngine/src/io/Fen.h"

#include "ChessEngine/src/representation/board/ZHasher.h"
#include "ChessEngine/src/representation/board/Bitboards.h"
#include "ChessEngine/src/representation/board/GameStateBitmasks.h"
#include "ChessEngine/src/representation/move/Movelist.h"
#include "ChessEngine/src/representation/move/Move.h"
#include "ChessEngine/src/representation/board/SquaresLookup.h"

#include "ChessEngine/src/logic/makemove/MoveMaker.h"
#include "ChessEngine/src/logic/makemove/MoveRetractor.h"
#include "ChessEngine/src/logic/movegen/MoveGenerator.h"
#include "ChessEngine/src/logic/evaluation/Evaluator.h"

namespace engine {
namespace perft {

class perft {

public:
    perft (
        int maxDepth,
        representation::board::Bitboards& bitboards,
        representation::board::SquaresLookup& squaresLookup,
        representation::board::GameStateBitmasks& gameStateBitmasks,
        representation::board::ZHasher& zHasher,
        logic::makemove::MoveMaker& moveMaker,
        logic::makemove::MoveRetractor& moveRetractor,
        logic::movegen::MoveGenerator& moveGenerator,
        engine::search::SearchMemory& searchMemory,
        logic::evaluation::Evaluator& evaluator,
        perftData& perftData
    );
    
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
    representation::board::Bitboards& _bitboardsRef;
    representation::board::SquaresLookup& _squaresLookupRef;
    representation::board::GameStateBitmasks& _gameStateBitmasksRef;
    representation::board::ZHasher& _zHasherRef;
    
    logic::makemove::MoveMaker& _moveMakerRef;
    logic::makemove::MoveRetractor& _moveRetractorRef;
    logic::movegen::MoveGenerator& _moveGeneratorRef;
    logic::evaluation::Evaluator& _evaluatorRef;
    
    search::SearchMemory& _searchMemoryRef;
    perftData& _perftDataRef;

    int _pseudoLegalMovesCount;
    std::vector<representation::move::Movelist> _movelists;
    std::vector<representation::board::PieceType> _lastCapturedPieces;
    std::vector<int> _noCapturedOrPawnMoveCounts; 

    bool tooManyPiecesOnBoard();
};

} // namespace perft
} // namespace engine