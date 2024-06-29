#pragma once

#include "ChessEngine/movegen/MoveGenerator.h"
#include "ChessEngine/evaluation/Evaluator.h"
#include "ChessEngine/game/BitBoards.h"
#include "ChessEngine/game/GameStateBitMasks.h"
#include "ChessEngine/game/SquaresLookup.h"
#include "ChessEngine/game/MoveMaker.h"
#include "ChessEngine/game/Move.h"
#include "ChessEngine/game/ZHasher.h"
#include "ChessEngine/utils/Fen.h"

#include "SearchMemory.h"

namespace perft {

    struct MoveScore {
        game::Move move;
        float score;
    };

    struct MoveList {
        std::vector<game::Move> moves;
        size_t numMoves;
    };

    class Searcher {
        public:

            static constexpr int MAX_LEGAL_MOVES = 218;

            Searcher(int maxDepth);
            void minimax(int current_depth, bool isMaximizer, int firstMoveIndex, bool recPerftStats = true, game::Move lastMove = game::Move(), bool verbose = true);
            void recordPerftStats(bool isMaximizer, int currentDepth, int &firstMoveIndex, size_t i, game::Move &currentMove, bool &retFlag);
            void genMoves(bool isWhite, std::vector<game::Move> &moveList, int currentDepth, unsigned char castlingRights);
            void makeMove(game::Move move, bool isWhite, int currentDepth);
            void unmakeMove(game::Move move, bool isWhite, int currentDepth);
            void undoMove();
            int _numMoveGenCalls;
            int _totalNodes;

            long sumNodesToDepth(int depth);
            std::vector<long> _nodeCountPerFirstMove;
            std::vector<game::Move> _firstMoves;
            std::vector<long> _nodeCount;
            std::vector<long> _captureCount;
            std::vector<long> _epCaptureCount;
            std::vector<long> _castlingCount;
            std::vector<long> _promotionCount;
            std::vector<long> _checkCount;
            std::vector<long> _checkmateCount;
            void debugPrint(bool verbose);
            bool checkCondition(int currentDepth, bool isMaximizer, int firstMoveIndex, game::Move currentMove, game::Move lastMove, bool verbose, size_t i);

            void setMaxDepth(int maxDepth) {
                _maxDepth = maxDepth;
            }

            int getMaxDepth() {
                return _maxDepth;
            }

            movegen::MoveGenerator& getMoveGenerator() {
                return _moveGenerator;
            }

            void setBoardFromFen(const std::string& fen) {
                utils::setBoardFromFen(fen, _bitboards, _gameStateBitMasks, _squaresLookup);
            }

            std::string& getFenFromBoard() {
                return utils::getFenFromBoard(_squaresLookup);
            }
            
        private:
            game::BitBoards _bitboards;
            game::SquaresLookup _squaresLookup;
            game::GameStateBitMasks _gameStateBitMasks;
            SearchMemory _searchMemory;
            game::ZHasher _zHasher;
            game::MoveMaker _moveMaker;
            movegen::MoveGenerator _moveGenerator;
            evaluation::Evaluator _evaluator;
            int _maxDepth;
 
            int _pseudoLegalMovesCount;
            std::vector<std::vector<game::Move>> _moveLists;
            std::vector<game::PieceType> _lastCapturedPieces;
            std::vector<int> _noCapturedOrPawnMoveCounts; 

            bool tooManyPiecesOnBoard();
    };
}