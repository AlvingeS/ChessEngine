#include "ChessEngine/src/logic/movegen/MoveGenerator.h"

#include "ChessEngine/src/logic/movegen/bitmasks/RayBitmasks.h"
#include "ChessEngine/src/logic/movegen/bitmasks/KnightBitmasks.h"
#include "ChessEngine/src/logic/movegen/bitmasks/KingBitmasks.h"
#include "ChessEngine/src/logic/movegen/bitmasks/PawnBitmasks.h"

#include "ChessEngine/src/logic/movegen/utils/ChessUtils.h"
#include "ChessEngine/src/logic/movegen/utils/BitBasics.h"

#include <iostream>

namespace logic {

MoveGenerator::MoveGenerator(
    model::Board& board,
    logic::MoveMaker& moveMaker,
    logic::MoveRetractor& moveRetractor)
    : _bitboards(board.bitboards)
    , _stateBitmasks(board.stateBitmasks)
    , _moveMaker(moveMaker)
    , _moveRetractor(moveRetractor)
    , _searchMemory(moveMaker.getSearchMemory())
    , _checkDetection(board)
    , _rookGenerator(board)
    , _bishopGenerator(board)
    , _queenGenerator(board)
    , _knightGenerator(board)
    , _kingGenerator(board)
    , _pawnGenerator(board)
    , _castlingGenerator(board, moveMaker, moveRetractor, &_checkDetection)
{}

void MoveGenerator::genMoves(
    bool isWhite,
    model::Movelist& moveList,
    int currentDepth,
    unsigned char castlingRights)
{
    moveList.reset();
    genRookMoves(isWhite, moveList);
    genBishopMoves(isWhite, moveList);
    genQueenMoves(isWhite, moveList);
    genKnightMoves(isWhite, moveList);
    genKingMoves(isWhite, moveList);
    genPawnMoves(isWhite, moveList, currentDepth, _searchMemory);
    genCastlingMoves(isWhite, moveList, castlingRights);
    moveList.addNullMove(); // Add a null move to the end of the move list
}

void MoveGenerator::genRookMoves(
    bool isWhite,
    model::Movelist& moveList)
{
    _rookGenerator.generate(isWhite, moveList);
}

void MoveGenerator::genBishopMoves(
    bool isWhite,
    model::Movelist& moveList)
{
    _bishopGenerator.generate(isWhite, moveList);
}

void MoveGenerator::genQueenMoves(
    bool isWhite,
    model::Movelist& moveList)
{
    _queenGenerator.generate(isWhite, moveList);;
}

void MoveGenerator::genKnightMoves(
    bool isWhite,
    model::Movelist& moveList)
{
    _knightGenerator.generate(isWhite, moveList);
}

void MoveGenerator::genKingMoves(
    bool isWhite,
    model::Movelist& moveList)
{
    _kingGenerator.generate(isWhite, moveList);
}

void MoveGenerator::genPawnMoves(
    bool isWhite,
    model::Movelist& moveList,
    int currentDepth,
    engine::SearchMemory& searchMemory)
{
    _pawnGenerator.generate(isWhite, moveList, currentDepth, searchMemory);
}

void MoveGenerator::genCastlingMoves(
    bool isWhite,
    model::Movelist& moveList,
    unsigned char castlingRights)
{
    _castlingGenerator.generate(isWhite, moveList, castlingRights);
}

bool MoveGenerator::isInCheck(bool isWhite) {
    return _checkDetection.isInCheck(isWhite);
}

} // namespace logic