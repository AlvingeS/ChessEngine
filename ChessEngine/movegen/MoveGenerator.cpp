#include "MoveGenerator.h"

#include <iostream>

#include "ChessEngine/masks/RayBitMasks.h"
#include "ChessEngine/utils/ChessUtils.h"
#include "ChessEngine/masks/KnightBitMasks.h"
#include "ChessEngine/masks/KingBitMasks.h"
#include "ChessEngine/masks/PawnBitMasks.h"
#include "ChessEngine/utils/BitBasics.h"

namespace movegen {
    MoveGenerator::MoveGenerator(game::BitBoards& bitboards, game::GameStateBitMasks& gameStateBitmasks, game::MoveMaker& moveMaker)
        : _bitboards(bitboards),
          _gameStateBitmasks(gameStateBitmasks),
          _moveMaker(moveMaker),
          _searchMemory(moveMaker.getSearchMemory()),
          _commonLogic(),
          _rayLogic(_gameStateBitmasks, _moveIndex, &_commonLogic),
          _checkDetection(_bitboards, &_rayLogic),
          _rookGenerator(_bitboards, &_rayLogic),
          _bishopGenerator(_bitboards, &_rayLogic),
          _knightGenerator(_bitboards, _gameStateBitmasks, _moveIndex, &_commonLogic),
          _queenGenerator(_bitboards, &_rayLogic),
          _kingGenerator(_bitboards, _gameStateBitmasks, _moveIndex, &_commonLogic),
          _pawnGenerator(_bitboards, _gameStateBitmasks, _moveIndex, &_commonLogic),
          _castlingGenerator(_bitboards, _gameStateBitmasks, moveMaker, _moveIndex, &_commonLogic, &_checkDetection) {
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

    void MoveGenerator::genPawnMoves(bool isWhite, std::vector<game::Move>& moveList, int currentDepth, perft::SearchMemory& searchMemory) {
        _pawnGenerator.generate(isWhite, moveList, currentDepth, searchMemory);
    }

    void MoveGenerator::genCastlingMoves(bool isWhite, std::vector<game::Move>& moveList, unsigned char castlingRights) {
        _castlingGenerator.generate(isWhite, moveList, castlingRights);
    }

    bool MoveGenerator::isInCheck(bool isWhite) {
        return _checkDetection.isInCheck(isWhite);
    }
}