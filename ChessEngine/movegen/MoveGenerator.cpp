#include "MoveGenerator.h"

#include <iostream>

#include "ChessEngine/bits/RayBitMasks.h"
#include "ChessEngine/utils/ChessUtils.h"
#include "ChessEngine/bits/KnightBitMasks.h"
#include "ChessEngine/bits/KingBitMasks.h"
#include "ChessEngine/bits/PawnBitMasks.h"
#include "ChessEngine/utils/BitBasics.h"

namespace movegen {
    MoveGenerator::MoveGenerator(game::ChessBoard& board, game::MoveMaker& moveMaker)
        : _board(board),
          _moveMaker(moveMaker),
          _searchMemory(moveMaker.getSearchMemory()),
          _commonLogic(),
          _rayLogic(_board, _moveIndex, &_commonLogic),
          _checkDetection(_board, &_rayLogic),
          _rookGenerator(_board, &_rayLogic),
          _bishopGenerator(_board, &_rayLogic),
          _knightGenerator(_board, _moveIndex, &_commonLogic),
          _queenGenerator(_board, &_rayLogic),
          _kingGenerator(_board, _moveIndex, &_commonLogic),
          _pawnGenerator(_board, _moveIndex, &_commonLogic),
           _castlingGenerator(_board, moveMaker, _moveIndex, &_commonLogic, &_checkDetection) {
    }

    void MoveGenerator::setBoardFromFen(std::string fen) {
        _board.setBoardFromFen(fen);
        _board.fillBlackPiecesBitmask();
        _board.fillWhitePiecesBitmask();
        _board.fillOccupiedPiecesBitmask();
        _board.fillEmptySquaresBitmask();
    }

    void MoveGenerator::genMoves(bool isWhite, std::vector<game::Move>& moveList, int currentDepth, unsigned char castlingRights) {
        genRookMoves(isWhite, moveList);
        genKnightMoves(isWhite, moveList);
        genBishopMoves(isWhite, moveList);
        genQueenMoves(isWhite, moveList);
        genKingMoves(isWhite, moveList);
        genPawnMoves(isWhite, moveList, currentDepth, _searchMemory);
        genCastlingMoves(isWhite, moveList, castlingRights);
        moveList[_moveIndex] = game::Move(); // Add a null move to the end of the move list
    }

    void MoveGenerator::resetMoves(std::vector<game::Move>& moveList) {
        moveList = std::vector<game::Move>(MAX_LEGAL_MOVES);
        _moveIndex = 0;
    }

    void MoveGenerator::resetMoveIndex() {
        _moveIndex = 0;
    }

    void MoveGenerator::genRookMoves(bool isWhite, std::vector<game::Move>& moveList) {
        _rookGenerator.generate(isWhite, moveList);
    }

    void MoveGenerator::genBishopMoves(bool isWhite, std::vector<game::Move>& moveList) {
        _bishopGenerator.generate(isWhite, moveList);
    }

    void MoveGenerator::genKnightMoves(bool isWhite, std::vector<game::Move>& moveList) {
        _knightGenerator.generate(isWhite, moveList);
    }

    void MoveGenerator::genQueenMoves(bool isWhite, std::vector<game::Move>& moveList) {
        _queenGenerator.generate(isWhite, moveList);
    }

    void MoveGenerator::genKingMoves(bool isWhite, std::vector<game::Move>& moveList) {
        _kingGenerator.generate(isWhite, moveList);
    }

    void MoveGenerator::genPawnMoves(bool isWhite, std::vector<game::Move>& moveList, int currentDepth, search::SearchMemory& searchMemory) {
        _pawnGenerator.generate(isWhite, moveList, currentDepth, searchMemory);
    }

    void MoveGenerator::genCastlingMoves(bool isWhite, std::vector<game::Move>& moveList, unsigned char castlingRights) {
        _castlingGenerator.generate(isWhite, moveList, castlingRights);
    }

    bool MoveGenerator::isInCheck(bool isWhite) {
        return _checkDetection.isInCheck(isWhite);
    }
}