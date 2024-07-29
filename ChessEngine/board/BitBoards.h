#pragma once

#include "ChessEngine/common.h"

#include "PieceType.h"

namespace board {

class BitBoards {

public:
    // ** Constructor **
    BitBoards();

    // ** Getters & Setters **
    inline U64& getBitboardFromPieceType(const PieceType pieceType) const 
    {
        return *(_bitboards[pieceTypeToInt(pieceType)]);
    }

    inline U64& getBitboardFromIndex(const int index) const 
    {
        return *(_bitboards[index]);
    }

    // TODO: This is a bad getter, it should be removed
    inline const std::vector<U64*>& getBitboards() const 
    {
        return _bitboards;
    }

    inline U64& getWhitePawnsBitboard() 
    {
        return _whitePawnsBitboard;
    }

    inline U64& getWhiteKnightsBitboard() 
    {
        return _whiteKnightsBitboard;
    }

    inline U64& getWhiteBishopsBitboard() 
    {
        return _whiteBishopsBitboard;
    }

    inline U64& getWhiteRooksBitboard() 
    {
        return _whiteRooksBitboard;
    }

    inline U64& getWhiteQueensBitboard() 
    {
        return _whiteQueensBitboard;
    }

    inline U64& getWhiteKingBitboard() 
    {
        return _whiteKingBitboard;
    }

    inline U64& getBlackPawnsBitboard() 
    {
        return _blackPawnsBitboard;
    }

    inline U64& getBlackKnightsBitboard() 
    {
        return _blackKnightsBitboard;
    }

    inline U64& getBlackBishopsBitboard() 
    {
        return _blackBishopsBitboard;
    }

    inline U64& getBlackRooksBitboard() 
    {
        return _blackRooksBitboard;
    }

    inline U64& getBlackQueensBitboard() 
    {
        return _blackQueensBitboard;
    }

    inline U64& getBlackKingBitboard() 
    {
        return _blackKingBitboard;
    }

    // ** Functions **
    void resetBitboards() {
        for (int i = 0; i < 12; i++) {
            *(_bitboards[i]) = 0;
        }
    }

private:
// ** Data **
std::vector<U64*> _bitboards;

// White bitboards
U64 _whitePawnsBitboard;
U64 _whiteKnightsBitboard;
U64 _whiteBishopsBitboard;
U64 _whiteRooksBitboard;
U64 _whiteQueensBitboard;
U64 _whiteKingBitboard;

// Black bitboards
U64 _blackPawnsBitboard;
U64 _blackKnightsBitboard;
U64 _blackBishopsBitboard;
U64 _blackRooksBitboard;
U64 _blackQueensBitboard;
U64 _blackKingBitboard;

};

} // namespace board