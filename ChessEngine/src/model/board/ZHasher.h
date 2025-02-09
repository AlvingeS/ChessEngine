#pragma once
#include "ChessEngine/src/common.h"

#include "ChessEngine/src/model/board/PieceType.h"

namespace model {

class PieceMap;

class ZHasher {

public:
    ZHasher(PieceMap& pieceMap);
    void hashSquarePieceType(int squareIndex, PieceType pieceType);
    void hashCastleRights(int castleRights);
    void hashEnPassantFile(int file);
    void hashIsWhite(bool isWhite);

private:
    void computeInitialHash();

    uint64_t _randBoardPieceTypeNums[64][12];
    
    // In order:
    //      0 - White King side
    //      1 - White Queen side
    //      2 - Black King side	
    //      3 - Black Queen side
    uint64_t _randCastleRightsNums[4];
    
    uint64_t _randEnPassantFileNums[8];
    uint64_t _randIsWhiteNum;
    uint64_t _hash;

    PieceMap& _pieceMap;
};

} // namespace model

#include "ChessEngine/src/model/board/ZHasher.inl"
