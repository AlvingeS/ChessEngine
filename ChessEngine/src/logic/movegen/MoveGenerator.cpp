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
    const model::Bitboards& bitboards,
    const model::StateBitmasks& stateBitmasks,
    logic::MoveMaker& moveMaker,
    logic::MoveRetractor& moveRetractor)
    : _bitboardsRef(bitboards)
    , _stateBitmasksRef(stateBitmasks)
    , _moveMakerRef(moveMaker)
    , _moveRetractorRef(moveRetractor)
    , _searchMemoryRef(moveMaker.getSearchMemory())
    , _checkDetection(_bitboardsRef, _stateBitmasksRef)
    , _rookGenerator(_bitboardsRef, _stateBitmasksRef)
    , _bishopGenerator(_bitboardsRef, _stateBitmasksRef)
    , _queenGenerator(_bitboardsRef, _stateBitmasksRef)
    , _knightGenerator(_bitboardsRef, _stateBitmasksRef)
    , _kingGenerator(_bitboardsRef, _stateBitmasksRef)
    , _pawnGenerator(_bitboardsRef, _stateBitmasksRef)
    , _castlingGenerator(_bitboardsRef, _stateBitmasksRef, moveMaker, moveRetractor, &_checkDetection)
{
}

void MoveGenerator::genMoves(
    bool isWhite,
    model::Movelist& moveListRef,
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
    model::Movelist& moveListRef)
{
    _rookGenerator.generate(isWhite, moveListRef);
}

void MoveGenerator::genBishopMoves(
    bool isWhite,
    model::Movelist& moveListRef)
{
    _bishopGenerator.generate(isWhite, moveListRef);
}

void MoveGenerator::genQueenMoves(
    bool isWhite,
    model::Movelist& moveListRef)
{
    _queenGenerator.generate(isWhite, moveListRef);;
}

void MoveGenerator::genKnightMoves(
    bool isWhite,
    model::Movelist& moveListRef)
{
    _knightGenerator.generate(isWhite, moveListRef);
}

void MoveGenerator::genKingMoves(
    bool isWhite,
    model::Movelist& moveListRef)
{
    _kingGenerator.generate(isWhite, moveListRef);
}

void MoveGenerator::genPawnMoves(
    bool isWhite,
    model::Movelist& moveListRef,
    int currentDepth,
    engine::SearchMemory& searchMemory)
{
    _pawnGenerator.generate(isWhite, moveListRef, currentDepth, searchMemory);
}

void MoveGenerator::genCastlingMoves(
    bool isWhite,
    model::Movelist& moveListRef,
    unsigned char castlingRights)
{
    _castlingGenerator.generate(isWhite, moveListRef, castlingRights);
}

bool MoveGenerator::isInCheck(bool isWhite) {
    return _checkDetection.isInCheck(isWhite);
}

} // namespace logic