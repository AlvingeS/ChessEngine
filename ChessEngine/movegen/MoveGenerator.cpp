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
    move::MoveMaker& moveMaker,
    move::MoveRetractor& moveRetractor)
    : _bitboardsRef(bitboards)
    , _gameStateBitmasksRef(gameStateBitmasks)
    , _moveMakerRef(moveMaker)
    , _moveRetractorRef(moveRetractor)
    , _searchMemoryRef(moveMaker.getSearchMemory())
    , _checkDetection(_bitboardsRef, _gameStateBitmasksRef)
    , _rookGenerator(_bitboardsRef, _gameStateBitmasksRef)
    , _bishopGenerator(_bitboardsRef, _gameStateBitmasksRef)
    , _queenGenerator(_bitboardsRef, _gameStateBitmasksRef)
    , _knightGenerator(_bitboardsRef, _gameStateBitmasksRef)
    , _kingGenerator(_bitboardsRef, _gameStateBitmasksRef)
    , _pawnGenerator(_bitboardsRef, _gameStateBitmasksRef)
    , _castlingGenerator(_bitboardsRef, _gameStateBitmasksRef, moveMaker, moveRetractor, &_checkDetection)
{
}

void MoveGenerator::genMoves(
    bool isWhite,
    Movelist& moveListRef,
    int currentDepth,
    unsigned char castlingRights)
{
    moveListRef.reset();
    genRookMoves(isWhite, moveListRef);
    genBishopMoves(isWhite, moveListRef);
    genQueenMoves(isWhite, moveListRef);
    genKnightMoves(isWhite, moveListRef);
    genKingMoves(isWhite, moveListRef);
    genPawnMoves(isWhite, moveListRef, currentDepth, _searchMemoryRef);
    genCastlingMoves(isWhite, moveListRef, castlingRights);
    moveListRef.addNullMove(); // Add a null move to the end of the move list
}

void MoveGenerator::genRookMoves(
    bool isWhite,
    Movelist& moveListRef)
{
    _rookGenerator.generate(isWhite, moveListRef);
}

void MoveGenerator::genBishopMoves(
    bool isWhite,
    Movelist& moveListRef)
{
    _bishopGenerator.generate(isWhite, moveListRef);
}

void MoveGenerator::genQueenMoves(
    bool isWhite,
    Movelist& moveListRef)
{
    _queenGenerator.generate(isWhite, moveListRef);;
}

void MoveGenerator::genKnightMoves(
    bool isWhite,
    Movelist& moveListRef)
{
    _knightGenerator.generate(isWhite, moveListRef);
}

void MoveGenerator::genKingMoves(
    bool isWhite,
    Movelist& moveListRef)
{
    _kingGenerator.generate(isWhite, moveListRef);
}

void MoveGenerator::genPawnMoves(
    bool isWhite,
    Movelist& moveListRef,
    int currentDepth,
    perft::SearchMemory& searchMemory)
{
    _pawnGenerator.generate(isWhite, moveListRef, currentDepth, searchMemory);
}

void MoveGenerator::genCastlingMoves(
    bool isWhite,
    Movelist& moveListRef,
    unsigned char castlingRights)
{
    _castlingGenerator.generate(isWhite, moveListRef, castlingRights);
}

bool MoveGenerator::isInCheck(bool isWhite) {
    return _checkDetection.isInCheck(isWhite);
}

} // namespace movegen