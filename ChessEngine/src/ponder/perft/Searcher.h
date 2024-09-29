#pragma once

#include "ChessEngine/src/ponder/perft/SearchMemory.h"
#include "ChessEngine/src/ponder/evaluation/Evaluator.h"

#include "ChessEngine/src/game/board/ZHasher.h"
#include "ChessEngine/src/game/board/Bitboards.h"
#include "ChessEngine/src/game/board/GameStateBitmasks.h"
#include "ChessEngine/src/game/board/SquaresLookup.h"
#include "ChessEngine/src/game/movegen/MoveGenerator.h"
#include "ChessEngine/src/game/movegen/Movelist.h"
#include "ChessEngine/src/game/move/MoveMaker.h"
#include "ChessEngine/src/game/move/MoveRetractor.h"
#include "ChessEngine/src/game/move/Move.h"
#include "ChessEngine/src/utils/Fen.h"

namespace ponder {
namespace perft {

class Searcher {

public:
    static constexpr int MAX_LEGAL_MOVES = 218;

    Searcher(
        int maxDepth,
        game::board::Bitboards& bitboards,
        game::board::SquaresLookup& squaresLookup,
        game::board::GameStateBitmasks& gameStateBitmasks,
        SearchMemory& searchMemory,
        game::board::ZHasher& zHasher,
        game::move::MoveMaker& moveMaker,
        game::move::MoveRetractor& moveRetractor,
        game::movegen::MoveGenerator& moveGenerator,
        evaluation::Evaluator& evaluator
    );
    
    void minimax(
        int current_depth,
        bool isMaximizer, 
        int firstMoveIndex,
        bool recPerftStats = true,
        const game::move::Move& lastMove = game::move::Move(),
        bool verbose = true
    );

    void recordPerftStats(
        bool isMaximizer,
        int currentDepth,
        int &firstMoveIndex,
        size_t i,
        const game::move::Move& currentMove,
        bool &retFlag
    );

    void genMoves(
        bool isWhite,
        int currentDepth, 
        unsigned char castlingRights
    );

    void makeMove(game::move::Move move, bool isWhite, int currentDepth);
    void unmakeMove(game::move::Move move, bool isWhite, int currentDepth);
    void undoMove();
    
    int _numMoveGenCalls;
    int _totalNodes;

    long sumNodesToDepth(int depth) const;
    std::vector<long> _nodeCountPerFirstMove;
    std::vector<game::move::Move> _firstMoves;
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
        game::move::Move currentMove, 
        game::move::Move lastMove, 
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

    const game::movegen::MoveGenerator& getMoveGenerator() const
    {
        return _moveGeneratorRef;
    }

    void setBoardFromFen(const std::string& fen)
    {
        utils::setBoardFromFen(fen, _bitboardsRef, _gameStateBitmasksRef, _squaresLookupRef);
    }

    std::string getFenFromBoard() const
    {
        return utils::getFenFromBoard(_squaresLookupRef);
    }

    bool diffBetweenGameStateBitmasks() const
    {
        return (_gameStateBitmasksRef.getBlackPiecesBitmask() | _gameStateBitmasksRef.getWhitePiecesBitmask()) != _gameStateBitmasksRef.getOccupiedPiecesBitmask();
    }

    void resetNodeCounts();
    
private:
    game::board::Bitboards& _bitboardsRef;
    game::board::SquaresLookup& _squaresLookupRef;
    game::board::GameStateBitmasks& _gameStateBitmasksRef;
    SearchMemory& _searchMemoryRef;
    game::board::ZHasher& _zHasherRef;
    game::move::MoveMaker& _moveMakerRef;
    game::move::MoveRetractor& _moveRetractorRef;
    game::movegen::MoveGenerator& _moveGeneratorRef;
    evaluation::Evaluator& _evaluatorRef;
    int _maxDepth;

    int _pseudoLegalMovesCount;
    std::vector<game::movegen::Movelist> _movelists;
    std::vector<game::board::PieceType> _lastCapturedPieces;
    std::vector<int> _noCapturedOrPawnMoveCounts; 

    bool tooManyPiecesOnBoard();
};

} // namespace perft
} // namespace ponder