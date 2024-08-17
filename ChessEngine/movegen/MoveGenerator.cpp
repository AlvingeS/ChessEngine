#include "MoveGenerator.h"

#include <iostream>

#include "ChessEngine/masks/RayBitmasks.h"
#include "ChessEngine/utils/ChessUtils.h"
#include "ChessEngine/masks/KnightBitmasks.h"
#include "ChessEngine/masks/KingBitmasks.h"
#include "ChessEngine/masks/PawnBitmasks.h"
#include "ChessEngine/utils/BitBasics.h"

namespace movegen {

MoveGenerator::MoveGenerator(
    const board::Bitboards& bitboards,
    const board::GameStateBitmasks& gameStateBitmasks,
    move::MoveMaker& moveMaker)
    : _bitboardsRef(bitboards)
    , _gameStateBitmasksRef(gameStateBitmasks)
    , _moveMaker(moveMaker)
    , _searchMemory(moveMaker.getSearchMemory())
    , _commonLogic()
    , _rayLogic(_gameStateBitmasksRef, _moveIndex, &_commonLogic)
    , _checkDetection(_bitboardsRef, &_rayLogic)
    , _rookGenerator(_bitboardsRef, &_rayLogic)
    , _bishopGenerator(_bitboardsRef, &_rayLogic)
    , _knightGenerator(_bitboardsRef, _gameStateBitmasksRef, _moveIndex, &_commonLogic)
    , _queenGenerator(_bitboardsRef, &_rayLogic)
    , _kingGenerator(_bitboardsRef, _gameStateBitmasksRef, _moveIndex, &_commonLogic)
    , _pawnGenerator(_bitboardsRef, _gameStateBitmasksRef, _moveIndex, &_commonLogic)
    , _castlingGenerator(_bitboardsRef, _gameStateBitmasksRef, moveMaker, _moveIndex, &_commonLogic, &_checkDetection)
{
}

void MoveGenerator::genMoves(
    bool isWhite,
    std::vector<move::Move>& moveList,
    int currentDepth,
    unsigned char castlingRights)
{
    genRookMoves(isWhite, moveList);
    genKnightMoves(isWhite, moveList);
    genBishopMoves(isWhite, moveList);
    genQueenMoves(isWhite, moveList);
    genKingMoves(isWhite, moveList);
    genPawnMoves(isWhite, moveList, currentDepth, _searchMemory);
    genCastlingMoves(isWhite, moveList, castlingRights);
    moveList[_moveIndex] = move::Move(); // Add a null move to the end of the move list
}

void MoveGenerator::resetMoves(std::vector<move::Move>& moveList)
{
    moveList = std::vector<move::Move>(MAX_LEGAL_MOVES);
    _moveIndex = 0;
}

void MoveGenerator::resetMoveIndex()
{
    _moveIndex = 0;
}

void MoveGenerator::genRookMoves(
    bool isWhite,
    std::vector<move::Move>& moveList)
{
    _rookGenerator.generate(isWhite, moveList);
}

void MoveGenerator::genBishopMoves(
    bool isWhite,
    std::vector<move::Move>& moveList)
{
    _bishopGenerator.generate(isWhite, moveList);
}

void MoveGenerator::genKnightMoves(
    bool isWhite,
    std::vector<move::Move>& moveList)
{
    _knightGenerator.generate(isWhite, moveList);
}

void MoveGenerator::genQueenMoves(
    bool isWhite,
    std::vector<move::Move>& moveList)
{
    _queenGenerator.generate(isWhite, moveList);
}

void MoveGenerator::genKingMoves(
    bool isWhite,
    std::vector<move::Move>& moveList)
{
    _kingGenerator.generate(isWhite, moveList);
}

void MoveGenerator::genPawnMoves(
    bool isWhite,
    std::vector<move::Move>& moveList,
    int currentDepth,
    perft::SearchMemory& searchMemory)
{
    _pawnGenerator.generate(isWhite, moveList, currentDepth, searchMemory);
}

void MoveGenerator::genCastlingMoves(
    bool isWhite,
    std::vector<move::Move>& moveList,
    unsigned char castlingRights)
{
    _castlingGenerator.generate(isWhite, moveList, castlingRights);
}

bool MoveGenerator::isInCheck(bool isWhite) {
    return _checkDetection.isInCheck(isWhite);
}

} // namespace movegen