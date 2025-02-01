#include "ChessEngine/src/model/board/Board.h"

namespace model {

Bitboards::Bitboards() 
{
    _bitboards.reserve(12);

    _whitePawnsBitboard = 0x000000000000FF00ULL;
    _whiteKnightsBitboard = 0x0000000000000042ULL;
    _whiteBishopsBitboard = 0x0000000000000024ULL;
    _whiteRooksBitboard = 0x0000000000000081ULL;
    _whiteQueensBitboard = 0x0000000000000010ULL;
    _whiteKingBitboard = 0x0000000000000008ULL;

    _blackPawnsBitboard = 0x00FF000000000000ULL;
    _blackKnightsBitboard = 0x4200000000000000ULL;
    _blackBishopsBitboard = 0x2400000000000000ULL;
    _blackRooksBitboard = 0x8100000000000000ULL;
    _blackQueensBitboard = 0x1000000000000000ULL;
    _blackKingBitboard = 0x0800000000000000ULL;

    _bitboards.push_back(&_whitePawnsBitboard);
    _bitboards.push_back(&_whiteKnightsBitboard);
    _bitboards.push_back(&_whiteBishopsBitboard);
    _bitboards.push_back(&_whiteRooksBitboard);
    _bitboards.push_back(&_whiteQueensBitboard);
    _bitboards.push_back(&_whiteKingBitboard);

    _bitboards.push_back(&_blackPawnsBitboard);
    _bitboards.push_back(&_blackKnightsBitboard);
    _bitboards.push_back(&_blackBishopsBitboard);
    _bitboards.push_back(&_blackRooksBitboard);
    _bitboards.push_back(&_blackQueensBitboard);
    _bitboards.push_back(&_blackKingBitboard);
}

void Bitboards::resetBitboards() 
{
    for (int i = 0; i < 12; i++)
        *(_bitboards[i]) = 0;
}

} // namespace model