#pragma once
#include "ChessEngine/movegen/MoveGenerator.h"
#include "ChessEngine/evaluation/Evaluator.h"
#include "ChessEngine/game/ChessBoard.h"
#include "ChessEngine/game/Move.h"

namespace search {

    constexpr unsigned char whiteKingSide = 0b0001;
    constexpr unsigned char whiteQueenSide = 0b0010;
    constexpr unsigned char whiteBoth = 0b0011;
    constexpr unsigned char blackKingSide = 0b0100;
    constexpr unsigned char blackQueenSide = 0b1000;
    constexpr unsigned char blackBoth = 0b1100;

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
            void genMoves(bool isWhite, std::vector<game::Move> &moveList, unsigned char castlingRights);
            void makeMove(game::Move move, bool isWhite);
            void unmakeMove(game::Move move, bool isWhite);
            void undoMove();
            int _numMoveGenCalls;
            int _totalNodes;

            int sumNodesToDepth(int depth);
            std::vector<int> _nodeCountPerFirstMove;
            std::vector<game::Move> _firstMoves;
            std::vector<int> _nodeCount;
            std::vector<int> _captureCount;
            std::vector<int> _epCaptureCount;
            std::vector<int> _castlingCount;
            std::vector<int> _promotionCount;
            std::vector<int> _checkCount;
            std::vector<int> _checkmateCount;
            void debugPrint(bool verbose);
            bool checkCondition(int currentDepth, bool isMaximizer, int firstMoveIndex, game::Move currentMove, game::Move lastMove, bool verbose, size_t i);

            void setMaxDepth(int maxDepth) {
                _maxDepth = maxDepth;
            }

            int getMaxDepth() {
                return _maxDepth;
            }

            game::ChessBoard& getBoard() {
                return _board;
            }

            movegen::MoveGenerator& getMoveGenerator() {
                return _moveGenerator;
            }

            void setBoardFromFen(std::string fen) {
                _board.setBoardFromFen(fen);
            }
            
            void overrideCastlingRights(unsigned char rights);
        private:
            game::ChessBoard _board;
            movegen::MoveGenerator _moveGenerator;
            evaluation::Evaluator _evaluator;
            int _maxDepth;

            int _pseudoLegalMovesCount;
            std::vector<std::vector<game::Move>> _moveLists;
            std::vector<game::PieceType> _lastCapturedPieces;
            std::vector<int> _noCapturedOrPawnMoveCounts; 
            std::vector<unsigned char> _castlingRights;

            void removeCastlingRightsForRemainingDepths(int currentDepth, unsigned char rightsToRemove);
            void restoreCastlingRightsForRemainingDepths(int currentDepth);
            void setCastlingRights(int currentDepth, game::Move move, bool isWhite, game::PieceType movedPieceType);
            void unsetCastlingRights(int currentDepth);
    };
}