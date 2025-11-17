REPO STRUCTURE
./
  CMakeLists.txt
  src/
    CMakeLists.txt
    common.h
    io/
      BoardPrinter.cpp
      BoardPrinter.h
      CMakeLists.txt
      Fen.cpp
      Fen.h
      StockfishPerftRetriever.cpp
      StockfishPerftRetriever.h
    logic/
      CMakeLists.txt
      movegen/
        BishopGenerator.cpp
        BishopGenerator.h
        CMakeLists.txt
        CastlingGenerator.cpp
        CastlingGenerator.h
        CheckDetection.cpp
        CheckDetection.h
        KingGenerator.cpp
        KingGenerator.h
        KnightGenerator.cpp
        KnightGenerator.h
        MoveGenerator.cpp
        MoveGenerator.h
        PawnGenerator.cpp
        PawnGenerator.h
        QueenGenerator.cpp
        QueenGenerator.h
        RayLogic.cpp
        RayLogic.h
        RookGenerator.cpp
        RookGenerator.h
        utils/
          BitBasics.cpp
          BitBasics.h
          BitBasics.inl
          CMakeLists.txt
          ChessUtils.cpp
          ChessUtils.h
          ChessUtils.inl
          Containers.cpp
          Containers.h
          Containers.inl
        bitmasks/
          BitmasksGenerator.cpp
          BitmasksGenerator.h
          CMakeLists.txt
          KingBitmasks.cpp
          KingBitmasks.h
          KnightBitmasks.cpp
          KnightBitmasks.h
          PawnBitmasks.cpp
          PawnBitmasks.h
          RayBitmasks.cpp
          RayBitmasks.h
      eval/
        CMakeLists.txt
        Evaluator.cpp
        Evaluator.h
      makemove/
        CMakeLists.txt
        MoveMaker.cpp
        MoveMaker.h
        MoveRetractor.cpp
        MoveRetractor.h
        MoveUtils.cpp
        MoveUtils.h
    engine/
      CMakeLists.txt
      pickmove/
        CMakeLists.txt
        MovePicker.cpp
        MovePicker.h
        SearchMemory.cpp
        SearchMemory.h
      perft/
        CMakeLists.txt
        perft.cpp
        perft.h
        perftData.cpp
        perftData.h
        perftData.inl
    model/
      CMakeLists.txt
      move/
        CMakeLists.txt
        Move.h
        Move.inl
        Movelist.cpp
        Movelist.h
        Movelist.inl
      board/
        Bitboards.cpp
        Bitboards.h
        Bitboards.inl
        Board.h
        CMakeLists.txt
        PieceMap.cpp
        PieceMap.h
        PieceMap.inl
        PieceType.cpp
        PieceType.h
        PieceType.inl
        StateBitmasks.cpp
        StateBitmasks.h
        StateBitmasks.inl
        ZHasher.cpp
        ZHasher.h
        ZHasher.inl
  tests/
    CMakeLists.txt
    logic/
      CMakeLists.txt
      movegen/
        BaseGenerator_test.h
        BishopGenerator_test.cpp
        CMakeLists.txt
        CastlingGenerator_test.cpp
        CheckDetection_test.cpp
        KingGenerator_test.cpp
        KnightGenerator_test.cpp
        PawnGenerator_test.cpp
        QueenGenerator_test.cpp
        RookGenerator_test.cpp
        utils/
          BitBasics_test.cpp
          CMakeLists.txt
          ChessUtils_test.cpp
        bitmasks/
          CMakeLists.txt
          KingBitmasks_test.cpp
          KnightBitmasks_test.cpp
          PawnBitmasks_test.cpp
          RayBitmasks_test.cpp
      makemove/
        CMakeLists.txt
        MakeUnmakeMove_test.cpp
    engine/
      CMakeLists.txt
      perft/
        CMakeLists.txt
        perftBase_test.h
        perftPosFive_test.cpp
        perftPosThree_test.cpp
        perftPosTwo_test.cpp
        perftStartPos_test.cpp
    model/
      CMakeLists.txt
      board/
        Bitboards_test.cpp
        CMakeLists.txt


--------------------

# FILE: CMakeLists.txt

```
add_subdirectory(src)
add_subdirectory(tests)
```


--------------------

# FILE: src/CMakeLists.txt

```
add_subdirectory(io)
add_subdirectory(engine)
add_subdirectory(logic)
add_subdirectory(model)
```


--------------------

# FILE: src/common.h

```cpp
#pragma once

#include <stdint.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <cassert>

typedef uint64_t bitboard;
typedef uint64_t bitmask;

static constexpr int MAX_LEGAL_MOVES = 218;
```


--------------------

# FILE: src/io/BoardPrinter.cpp

```cpp
#include "io/BoardPrinter.h"

#include "logic/movegen/utils/BitBasics.h"

#include <iostream>

namespace io {

BoardPrinter::BoardPrinter(const model::Bitboards& bitboards) 
{
    fillBoard(bitboards);
}

bool BoardPrinter::isValidPiece(model::Piece::Type piece_type) 
{
    switch(piece_type) {
        case model::Piece::Type::W_PAWN:
        case model::Piece::Type::W_KNIGHT:
        case model::Piece::Type::W_BISHOP:
        case model::Piece::Type::W_ROOK:
        case model::Piece::Type::W_QUEEN:
        case model::Piece::Type::W_KING:
        case model::Piece::Type::B_PAWN:
        case model::Piece::Type::B_KNIGHT:
        case model::Piece::Type::B_BISHOP:
        case model::Piece::Type::B_ROOK:
        case model::Piece::Type::B_QUEEN:
        case model::Piece::Type::B_KING:
            return true;
        default:
            return false;
    }
}

void BoardPrinter::fillBoard(const model::Bitboards& bitboards) 
{
    _board = std::vector<std::vector<char>>(8, std::vector<char>(8, ' '));
    
    for (int i = 0; i < 12; i++) {
        model::Piece::Type pieceType = model::Piece::getTypeFromInt(i);
        bitmask bitboard = bitboards.getBitboardFromIndex(i);
        char pieceChar = model::Piece::getCharFromType(pieceType);

        for (int i = 0; i < 64; i++) {
            if ((bitboard >> i) & 1) {
                int row = i / 8;
                int col = i % 8;
                _board[row][col] = pieceChar;
            }
        }
    }
}

void BoardPrinter::printBoard() const {
    for (int row = 7; row >= 0; row--) {  // Start from the bottom row (row 7) and go upwards.
        std::cout << "---------------------------------   ---------------------------------" << std::endl;
        for (int col = 7; col >= 0; col--) {  // Iterate through columns from left to right.
            char piece = _board[row][col];
            std::cout << "| " << piece << " ";  // Print the piece character.
        }

        std::cout << "|   ";  // Separate the two boards.

        for (int col = 7; col >= 0; col--) {  // Iterate again for bit indices.
            int bitIndex = row * 8 + col;
            std::cout << "| " << bitIndex << (bitIndex < 10 ? " " : "");  // Print the bit index, add extra space for single digit numbers.
        }

        std::cout << "|" << std::endl;  // End of the row.
    }
    std::cout << "----------------------------------  ----------------------------------" << std::endl;
}

} // namespace io
```


--------------------

# FILE: src/io/BoardPrinter.h

```cpp
#pragma once

#include "model/board/Board.h"

namespace io {

class BoardPrinter {

public:
    BoardPrinter(const model::Bitboards& bitboards);
    void fillBoard(const model::Bitboards& bitboards);
    void printBoard() const;

private:
    bool isValidPiece(model::Piece::Type piece_type);

    std::vector<std::vector<char>> _board;
};

} // namespace io
```


--------------------

# FILE: src/io/CMakeLists.txt

```
add_library(io
    BoardPrinter.cpp
    Fen.cpp
    StockfishPerftRetriever.cpp
)

target_link_libraries(io PUBLIC movegen board cxx_setup)
```


--------------------

# FILE: src/io/Fen.cpp

```cpp
#include "io/Fen.h"

#include "model/board/PieceType.h"

namespace io {

void Fen::setBoardFromFen(
    const std::string& fen, 
    model::Bitboards& bitboards, 
    model::StateBitmasks& stateBitmasks, 
    model::PieceMap& pieceMap) 
{
    bitboards.resetBitboards();

    int rank = 7;
    int file = 0;
    for (char c : fen) {
        if (c == ' ') {
            break;
        } else if (c == '/') {
            rank--;
            file = 0;
        } else if (std::isdigit(c)) {
            file += c - '0';
        } else {
            model::Piece::Type type = model::Piece::getTypeFromChar(c);
            bitboards.setPieceTypeBit(rank * 8 + 7 - file, type);
            file++;
        }
    }

    pieceMap.fillPieceMapFromBitboards(bitboards);
    stateBitmasks.fillBlackPiecesBitmaskFromBitboards(bitboards);
    stateBitmasks.fillWhitePiecesBitmaskFromBitboards(bitboards);
    stateBitmasks.updOccupiedAndEmptySquaresBitmasks();
}

std::string Fen::getFenFromBoard(const model::PieceMap& pieceMap)
{
    std::string fen = "";
    int emptyCount = 0;

    for (int i = 63; i >= 0; i--) {
        if ((i + 1) % 8 == 0 && i != 63) {
            if (emptyCount != 0) {
                fen += std::to_string(emptyCount);
                emptyCount = 0;
            }
            fen += "/";
        }

        model::Piece::Type type = pieceMap.getPieceTypeAtIndex(i);
        if (type == model::Piece::Type::EMPTY) {
            emptyCount++;
        } else {
            if (emptyCount != 0) {
                fen += std::to_string(emptyCount);
                emptyCount = 0;
            }
            fen += model::Piece::getCharFromType(type);
        }
    }

    return fen;
}

} // namespace io
```


--------------------

# FILE: src/io/Fen.h

```cpp
#pragma once

#include "model/board/Board.h"

namespace io {

struct Fen {
    
    static void setBoardFromFen(const std::string& fen, model::Bitboards& bitboards, model::StateBitmasks& stateBitmasks, model::PieceMap& pieceMap);
    static std::string getFenFromBoard(const model::PieceMap& pieceMap);

};

} // namespace io
```


--------------------

# FILE: src/io/StockfishPerftRetriever.cpp

```cpp
#include "io/StockfishPerftRetriever.h"

#include <sstream>
#include <regex>
#include <iostream>

namespace io::stockfish
{

namespace
{

std::string buildShellCommand(const std::string& FEN, int depth) 
{
    std::ostringstream oss;
    
    oss << "echo \"position fen " << FEN
        << "\\ngo perft " << depth
        << "\\nquit\" | stockfish";

    return oss.str();
} 

std::string getStockfishOutput(const std::string& FEN, int depth)
{
    auto shellCommand = buildShellCommand(FEN, depth);

    FILE* pipe = popen(shellCommand.c_str(), "r");

    if (!pipe) {
        std::cerr << "Couldn't open pipe to stockfish" << std::endl;
        return{}; // Return empty buffer
    }

    char buffer[256];
    std::string output = "";

    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        output += buffer;
    }

    pclose(pipe);

    return output;
}

std::unordered_map<std::string, uint64_t> parseOutput(std::string stockfishOutput)
{
    std::unordered_map<std::string, uint64_t> results{};
    std::regex pattern(R"((\w+): (\d+))");              // Finds <alphanumeric>: <digits> pattern
    std::smatch match;

    while (std::regex_search(stockfishOutput, match, pattern)) {
        if (match.size() == 3) {                        // Full match + 2 subgroups
            std::string move = match[1].str();          // <alphanumeric>
            long count = std::stol(match[2].str());     // <digits>
            results[move] = count;
        }
        stockfishOutput = match.suffix().str();         // Replace output with everything
                                                        // after the matched row
    }

    return results; 
}

} // namespace


std::unordered_map<std::string, uint64_t> getPerftResults(const std::string& FEN, int depth) 
{
    const auto stockfishOutput = getStockfishOutput(FEN, depth);
    return parseOutput(stockfishOutput);
}

} // namespace io::stockfish
```


--------------------

# FILE: src/io/StockfishPerftRetriever.h

```cpp
#pragma once

#include "common.h"
#include <unordered_map>

namespace io::stockfish
{

std::unordered_map<std::string, uint64_t> getPerftResults(const std::string& FEN, int depth);


} // namespace io::stockfish
```


--------------------

# FILE: src/logic/CMakeLists.txt

```
add_subdirectory(eval)
add_subdirectory(makemove)
add_subdirectory(movegen)
```


--------------------

# FILE: src/logic/movegen/BishopGenerator.cpp

```cpp
#include "logic/movegen/BishopGenerator.h"

#include "logic/movegen/RayLogic.h"

#include "logic/movegen/utils/Containers.h"
#include "logic/movegen/utils/BitBasics.h"
#include "logic/movegen/utils/ChessUtils.h"

#include "model/board/PieceType.h"
#include "model/board/Board.h"
#include "model/move/Move.h"
#include "model/move/Movelist.h"

namespace logic {

BishopGenerator::BishopGenerator(model::Board& board) 
    : _bitboards(board.bitboards)
    , _stateBitmasks(board.stateBitmasks)
{
    _diagonalRayBitmasks = RayBitmasks::getAllDiagonalRayBitmasks();
}

void BishopGenerator::generate(
    bool isWhite,
    model::Movelist& moveList)
{
    std::vector<int>& bishopIndices = Containers::getPiecePositionIndices();

    BitBasics::getBitIndices(bishopIndices, isWhite ? _bitboards.getWhiteBishopsBitboard()
                                         : _bitboards.getBlackBishopsBitboard());

    for (int currentBishopIndex : bishopIndices) {
        RayBitmasks::DiagonalRays rays = _diagonalRayBitmasks[currentBishopIndex];
        int bishopRank = ChessUtils::rankFromBitIndex(currentBishopIndex);
        int bishopFile = ChessUtils::fileFromBitIndex(currentBishopIndex);

        RayLogic::addMovesFromDiagonalRay(rays.northEast, true, isWhite, currentBishopIndex, bishopRank, bishopFile, moveList, _stateBitmasks.getWhitePiecesBitmask(), _stateBitmasks.getOccupiedPiecesBitmask());
        RayLogic::addMovesFromDiagonalRay(rays.southEast, false, isWhite, currentBishopIndex, bishopRank, bishopFile, moveList, _stateBitmasks.getWhitePiecesBitmask(), _stateBitmasks.getOccupiedPiecesBitmask());
        RayLogic::addMovesFromDiagonalRay(rays.southWest, false, isWhite, currentBishopIndex, bishopRank, bishopFile, moveList, _stateBitmasks.getWhitePiecesBitmask(), _stateBitmasks.getOccupiedPiecesBitmask());
        RayLogic::addMovesFromDiagonalRay(rays.northWest, true, isWhite, currentBishopIndex, bishopRank, bishopFile, moveList, _stateBitmasks.getWhitePiecesBitmask(), _stateBitmasks.getOccupiedPiecesBitmask());
    }
}

} // namespace logic
```


--------------------

# FILE: src/logic/movegen/BishopGenerator.h

```cpp
#pragma once
#include "common.h"
#include "logic/movegen/bitmasks/RayBitmasks.h"

namespace model {
    class Movelist;
    class Board;
    class Bitboards;
    class StateBitmasks;
}

namespace logic {

class BishopGenerator {

public:
    BishopGenerator(model::Board& board);
    
    void generate(
        bool isWhite, 
        model::Movelist& movelist
    );

private:
    const model::Bitboards& _bitboards;
    const model::StateBitmasks& _stateBitmasks;
    std::vector<RayBitmasks::DiagonalRays> _diagonalRayBitmasks;
};

} // namespace logic
```


--------------------

# FILE: src/logic/movegen/CMakeLists.txt

```
add_library(movegen)

add_subdirectory(utils)
add_subdirectory(bitmasks)

target_sources(movegen PRIVATE
    MoveGenerator.cpp
    RookGenerator.cpp
    BishopGenerator.cpp
    QueenGenerator.cpp
    KnightGenerator.cpp
    KingGenerator.cpp
    PawnGenerator.cpp
    CastlingGenerator.cpp
    RayLogic.cpp
    CheckDetection.cpp
)

target_link_libraries(movegen PUBLIC pickmove move board cxx_setup)
```


--------------------

# FILE: src/logic/movegen/CastlingGenerator.cpp

```cpp
#include "logic/movegen/CastlingGenerator.h"

#include "model/board/Board.h"

#include "logic/movegen/utils/ChessUtils.h"
#include "logic/movegen/CheckDetection.h"

#include "logic/makemove/MoveMaker.h"
#include "logic/makemove/MoveRetractor.h"

#include "model/move/Movelist.h"
#include "model/move/Move.h"

namespace logic {

CastlingGenerator::CastlingGenerator(
    model::Board& board,
    logic::MoveMaker& moveMaker, 
    logic::MoveRetractor& moveRetractor, 
    CheckDetection* checkDetection)
    : _bitboards(board.bitboards)
    , _stateBitmasks(board.stateBitmasks)
    , _moveMaker(moveMaker)
    , _moveRetractor(moveRetractor)
    , _checkDetection(checkDetection) 
{
    _whiteKingSideCastleBitmask = mWhiteKingSideCastleMask;
    _whiteQueenSideCastleBitmask = mWhiteQueenSideCastleMask;
    _blackKingSideCastleBitmask = mBlackKingSideCastleMask;
    _blackQueenSideCastleBitmask = mBlackQueenSideCastleMask;
}

void CastlingGenerator::generate(
    bool isWhite,
    model::Movelist& movelist,
    unsigned char castlingRights) 
{
    if (castlingRights == 0) {
        return;
    }
    
    if (isWhite) {
        if (castlingRights & 0b0001)
            genSingleCastleMove(isWhite, true, movelist);

        if (castlingRights & 0b0010)
            genSingleCastleMove(isWhite, false, movelist);
    } else {
        if (castlingRights & 0b0100)
            genSingleCastleMove(isWhite, true, movelist);

        if (castlingRights & 0b1000)
            genSingleCastleMove(isWhite, false, movelist);
    }
}

bool CastlingGenerator::kingAndRookOnCastlingSquares(
    bool isWhite,
    bool isKingSide) const
{
    bool kingBitEnabled = isWhite ? (_bitboards.getWhiteKingBitboard() & (1ULL << 3)) != 0
                                  : (_bitboards.getBlackKingBitboard() & (1ULL << 59)) != 0;
    
    if (!kingBitEnabled)
        return false;

    // Since we know that the king is present, we can return if the rook is present or not
    return isWhite ? (isKingSide ? (_bitboards.getWhiteRooksBitboard() & (1ULL << 0)) != 0
                                 : (_bitboards.getWhiteRooksBitboard() & (1ULL << 7)) != 0)
                   : (isKingSide ? (_bitboards.getBlackRooksBitboard() & (1ULL << 56)) != 0
                                 : (_bitboards.getBlackRooksBitboard() & (1ULL << 63)) != 0);
}

void CastlingGenerator::makeTemporaryKingMove(bool isWhite, bool isKingSide)
{
    _moveMaker.makeTemporaryKingMove(isWhite, isKingSide);
}

void CastlingGenerator::unmakeTemporaryKingMove(bool isWhite, bool isKingSide)
{
    _moveRetractor.unmakeTemporaryKingMove(isWhite, isKingSide);
}

void CastlingGenerator::genSingleCastleMove(
    bool isWhite,
    bool isKingSide,
    model::Movelist& moveList)
{                                                  
    // Check that there are no pieces between the king and rook
    bitmask spaceBetweenCastlersBitmask = isWhite ? (isKingSide ? _whiteKingSideCastleBitmask 
                                                                : _whiteQueenSideCastleBitmask)
                                                  : (isKingSide ? _blackKingSideCastleBitmask
                                                                : _blackQueenSideCastleBitmask);
    
    if ((spaceBetweenCastlersBitmask & _stateBitmasks.getOccupiedPiecesBitmask()) != 0)
        return;

    // Check that the king and rook are on the correct squares
    if (!kingAndRookOnCastlingSquares(isWhite, isKingSide))
        return;

    // Check that we are not currently in check
    if (_checkDetection->isInCheck(isWhite))
        return;

    // Move king one square towards the rook, check that the king is not in check
    makeTemporaryKingMove(isWhite, isKingSide);
    
    if (_checkDetection->isInCheck(isWhite)) {
        unmakeTemporaryKingMove(isWhite, isKingSide);
        return;
    }
    
    unmakeTemporaryKingMove(isWhite, isKingSide);

    int moveFlag = isKingSide ? model::Move::KING_CASTLE_FLAG 
                              : model::Move::QUEEN_CASTLE_FLAG;    

    moveList.addMove(model::Move(0, 0, moveFlag));
}

} // namespace logic
```


--------------------

# FILE: src/logic/movegen/CastlingGenerator.h

```cpp
#pragma once
#include "common.h"

namespace model {
    class Move;
    class Movelist;
    class Board;
    class Bitboards;
    class StateBitmasks;
}

namespace logic {

class MoveMaker;
class MoveRetractor;
class CheckDetection;

class CastlingGenerator {
    
public:
    CastlingGenerator(
        model::Board& board,
        logic::MoveMaker& moveMaker,
        logic::MoveRetractor& moveRetractor,
        CheckDetection* checkDetection
    );
    
    void generate(
        bool isWhite, 
        model::Movelist& movelist, 
        unsigned char castlingRights
    );

private:
    const model::Bitboards& _bitboards;
    const model::StateBitmasks& _stateBitmasks;
    
    logic::MoveMaker& _moveMaker;
    logic::MoveRetractor& _moveRetractor;
    
    CheckDetection* _checkDetection;

    bitmask _whiteKingSideCastleBitmask;
    bitmask _whiteQueenSideCastleBitmask;
    bitmask _blackKingSideCastleBitmask;
    bitmask _blackQueenSideCastleBitmask;

    void genSingleCastleMove(
        bool isWhite,
        bool isKingSide,
        model::Movelist& movelist
    );

    bool kingAndRookOnCastlingSquares(bool isWhite, bool isKingSide) const;
    void makeTemporaryKingMove(bool isWhite, bool isKingSide);
    void unmakeTemporaryKingMove(bool isWhite, bool isKingSide);

    const bitmask mWhiteKingSideCastleMask = 0x000000000000006ULL;
    const bitmask mWhiteQueenSideCastleMask = 0x0000000000000070ULL;
    const bitmask mBlackKingSideCastleMask = 0x0600000000000000ULL;
    const bitmask mBlackQueenSideCastleMask = 0x7000000000000000ULL;
};

} // namespace logic
```


--------------------

# FILE: src/logic/movegen/CheckDetection.cpp

```cpp
#include "logic/movegen/CheckDetection.h"

#include "model/board/Board.h"

#include "logic/movegen/RayLogic.h"
#include "logic/movegen/utils/ChessUtils.h"
#include "logic/movegen/utils/BitBasics.h"
#include "logic/movegen/bitmasks/KnightBitmasks.h"
#include "logic/movegen/bitmasks/PawnBitmasks.h"

namespace logic {

CheckDetection::CheckDetection(model::Board& board)
    : _bitboards(board.bitboards)
    , _stateBitmasks(board.stateBitmasks)
{
    _straightRayBitmasks = RayBitmasks::getAllStraightRayBitmasks();
    _diagonalRayBitmasks = RayBitmasks::getAllDiagonalRayBitmasks();
    _knightBitmasks = KnightBitmasks::getAllKnightBitmasks();
    _whitePawnCaptureMoveBitmasks = PawnBitmasks::getAllCapturePawnMoveBitmasks(true);
    _blackPawnCaptureMoveBitmasks = PawnBitmasks::getAllCapturePawnMoveBitmasks(false);
}

bool CheckDetection::isInCheck(bool isWhite) const
{
    int kingIndex, opponentKingIndex, kingRankDiff, kingFileDiff;

    kingIndex = BitBasics::indexOfLSB(isWhite ? _bitboards.getWhiteKingBitboard()
                                   : _bitboards.getBlackKingBitboard());

    // Check if any opponent rooks or queens are attacking the king
    RayBitmasks::StraightRays straightRays = _straightRayBitmasks[kingIndex];
    bitmask opponentRooksAndQueens = isWhite ? _bitboards.getBlackRooksBitboard() | _bitboards.getBlackQueensBitboard()
                                             : _bitboards.getWhiteRooksBitboard() | _bitboards.getWhiteQueensBitboard();

    if (RayLogic::checkStraightRay(straightRays.north, true, opponentRooksAndQueens, _stateBitmasks.getOccupiedPiecesBitmask()))
        return true;

    if (RayLogic::checkStraightRay(straightRays.east, false, opponentRooksAndQueens, _stateBitmasks.getOccupiedPiecesBitmask()))
        return true;

    if (RayLogic::checkStraightRay(straightRays.south, false, opponentRooksAndQueens, _stateBitmasks.getOccupiedPiecesBitmask()))
        return true;

    if (RayLogic::checkStraightRay(straightRays.west, true, opponentRooksAndQueens, _stateBitmasks.getOccupiedPiecesBitmask()))
        return true;

    // Check if any opponent bishops or queens are attacking the king
    RayBitmasks::DiagonalRays diagonalRays = _diagonalRayBitmasks[kingIndex];
    bitmask opponentBishopsAndQueens = isWhite ? _bitboards.getBlackBishopsBitboard() | _bitboards.getBlackQueensBitboard() 
                                               : _bitboards.getWhiteBishopsBitboard() | _bitboards.getWhiteQueensBitboard();

    if (RayLogic::checkDiagonalRay(diagonalRays.northEast, true, opponentBishopsAndQueens, _stateBitmasks.getOccupiedPiecesBitmask()))
        return true;

    if (RayLogic::checkDiagonalRay(diagonalRays.southEast, false, opponentBishopsAndQueens, _stateBitmasks.getOccupiedPiecesBitmask()))
        return true;

    if (RayLogic::checkDiagonalRay(diagonalRays.southWest, false, opponentBishopsAndQueens, _stateBitmasks.getOccupiedPiecesBitmask()))
        return true;

    if (RayLogic::checkDiagonalRay(diagonalRays.northWest, true, opponentBishopsAndQueens, _stateBitmasks.getOccupiedPiecesBitmask()))
        return true;

    // Check if any opponent knights are attacking the king
    bitmask knightMoves = _knightBitmasks[kingIndex];
    bitmask opponentKnights = isWhite ? _bitboards.getBlackKnightsBitboard() 
                                      : _bitboards.getWhiteKnightsBitboard();

    if ((knightMoves & opponentKnights) != 0)
        return true;
    
    // Check if any opponent pawns are attacking the king
    bitmask opponentPawns = isWhite ? _bitboards.getBlackPawnsBitboard() 
                                    : _bitboards.getWhitePawnsBitboard();
    
    bitmask pawnAttackingMoves = isWhite ? _whitePawnCaptureMoveBitmasks[kingIndex] 
                                         : _blackPawnCaptureMoveBitmasks[kingIndex];

    if ((pawnAttackingMoves & opponentPawns) != 0)
        return true;

    // Check if the king is in check from an adjacent king
    opponentKingIndex = BitBasics::indexOfLSB(isWhite ? _bitboards.getBlackKingBitboard()
                                           : _bitboards.getWhiteKingBitboard());

    kingRankDiff = ChessUtils::abs(ChessUtils::rankFromBitIndex(kingIndex) - ChessUtils::rankFromBitIndex(opponentKingIndex));
    kingFileDiff = ChessUtils::abs(ChessUtils::fileFromBitIndex(kingIndex) - ChessUtils::fileFromBitIndex(opponentKingIndex));

    int manhattanDistance = kingRankDiff + kingFileDiff;

    if (manhattanDistance <= 1)
        return true;

    if (manhattanDistance == 2 && kingRankDiff == 1 && kingFileDiff == 1)
        return true;

    return false;
}

} // namespace logic
```


--------------------

# FILE: src/logic/movegen/CheckDetection.h

```cpp
#pragma once

#include "common.h"

#include "logic/movegen/bitmasks/RayBitmasks.h"

namespace model {
    class Board;
    class Bitboards;
    class StateBitmasks;
}

namespace logic {

class CheckDetection {

public:
    CheckDetection(model::Board& board);
    bool isInCheck(bool isWhite) const;

private:
    const model::Bitboards& _bitboards;
    const model::StateBitmasks& _stateBitmasks;

    std::vector<RayBitmasks::StraightRays> _straightRayBitmasks;
    std::vector<RayBitmasks::DiagonalRays> _diagonalRayBitmasks;
    std::vector<bitmask> _knightBitmasks;
    std::vector<bitmask> _whitePawnCaptureMoveBitmasks;
    std::vector<bitmask> _blackPawnCaptureMoveBitmasks;
};

} // namespace logic
```


--------------------

# FILE: src/logic/movegen/KingGenerator.cpp

```cpp
#include "logic/movegen/KingGenerator.h"

#include "model/board/Board.h"
#include "model/move/Movelist.h"

#include "logic/movegen/bitmasks/KingBitmasks.h"
#include "logic/movegen/utils/Containers.h"
#include "logic/movegen/utils/BitBasics.h"

namespace logic {

KingGenerator::KingGenerator(model::Board& board) 
    : _bitboards(board.bitboards)
    , _stateBitmasks(board.stateBitmasks)
{
    _kingBitmasks = KingBitmasks::getAllKingBitmasks();
}

void KingGenerator::generate(bool isWhite, model::Movelist& moveList) 
{
    std::vector<int>& kingIndices = Containers::getPiecePositionIndices();
    std::vector<int>& freeMovesIndices = Containers::getLeapingPiecefreeMovesIndices();
    std::vector<int>& capturableMovesIndices = Containers::getLeapingPieceCapturableMovesIndices();
    
    BitBasics::getBitIndices(kingIndices, isWhite ? _bitboards.getWhiteKingBitboard()
                                              : _bitboards.getBlackKingBitboard());

    // TODO: It makes zero sense to have this in a loop
    for (int currentKingIndex : kingIndices) {
        bitmask kingBitmask = _kingBitmasks[currentKingIndex];

        bitmask freeKingMoves = kingBitmask & _stateBitmasks.getEmptySquaresBitmask();
        
        bitmask enemyPieces = isWhite ? _stateBitmasks.getBlackPiecesBitmask() 
                                      : _stateBitmasks.getWhitePiecesBitmask();

        bitmask capturableKingMoves = kingBitmask & enemyPieces;

        BitBasics::getBitIndices(freeMovesIndices, freeKingMoves);
        BitBasics::getBitIndices(capturableMovesIndices, capturableKingMoves);

        for (int freeKingMoveIndex : freeMovesIndices) {
            moveList.addMove(model::Move(currentKingIndex, freeKingMoveIndex, model::Move::QUITE_FLAG));
        }

        for (int capturableKingMoveIndex : capturableMovesIndices) {
            moveList.addMove(model::Move(currentKingIndex, capturableKingMoveIndex, model::Move::CAPTURE_FLAG));
        }
    }
}

} // namespace logic
```


--------------------

# FILE: src/logic/movegen/KingGenerator.h

```cpp
#pragma once

#include "common.h"

namespace model {
    class Bitboards;
    class StateBitmasks;
    class Board;
    class Movelist;
}

namespace logic {

class KingGenerator {

public:
    KingGenerator(model::Board& board);
    void generate(bool isWhite, model::Movelist& moveList);

private:
    const model::Bitboards& _bitboards;
    const model::StateBitmasks& _stateBitmasks;
    std::vector<bitmask> _kingBitmasks;
};

} // namespace logic
```


--------------------

# FILE: src/logic/movegen/KnightGenerator.cpp

```cpp
#include "logic/movegen/KnightGenerator.h"

#include "model/board/Board.h"
#include "model/move/Movelist.h"

#include "logic/movegen/bitmasks/KnightBitmasks.h"
#include "logic/movegen/utils/Containers.h"
#include "logic/movegen/utils/BitBasics.h"

namespace logic {

KnightGenerator::KnightGenerator(model::Board& board) 
    : _bitboards(board.bitboards)
    , _stateBitmasks(board.stateBitmasks) {
    _knightBitmasks = KnightBitmasks::getAllKnightBitmasks();
}

void KnightGenerator::generate(
    bool isWhite,
    model::Movelist& movelist) 
{
    std::vector<int>& knightIndices = Containers::getPiecePositionIndices();
    std::vector<int>& freeMovesIndices = Containers::getLeapingPiecefreeMovesIndices();
    std::vector<int>& capturableMovesIndices = Containers::getLeapingPieceCapturableMovesIndices();

    BitBasics::getBitIndices(knightIndices, isWhite ? _bitboards.getWhiteKnightsBitboard()
                                                    : _bitboards.getBlackKnightsBitboard());

    for (int currentKnightIndex : knightIndices) {
        bitmask knightBitmask = _knightBitmasks[currentKnightIndex];

        bitmask freeKnightMoves = knightBitmask & _stateBitmasks.getEmptySquaresBitmask();
        
        bitmask enemyPieces = isWhite ? _stateBitmasks.getBlackPiecesBitmask()
                                    : _stateBitmasks.getWhitePiecesBitmask();
        
        bitmask capturableKnightMoves = knightBitmask & enemyPieces;

        BitBasics::getBitIndices(freeMovesIndices, freeKnightMoves);
        BitBasics::getBitIndices(capturableMovesIndices, capturableKnightMoves);

        for (int freeKnightMoveIndex : freeMovesIndices) {
            movelist.addMove(model::Move(currentKnightIndex, freeKnightMoveIndex, model::Move::QUITE_FLAG));
        }

        for (int capturableKnightMoveIndex : capturableMovesIndices) {
            movelist.addMove(model::Move(currentKnightIndex, capturableKnightMoveIndex, model::Move::CAPTURE_FLAG));
        }
    }
}

} // namespace logic
```


--------------------

# FILE: src/logic/movegen/KnightGenerator.h

```cpp
#pragma once

#include "common.h"

namespace model {
    class Movelist;
    class Board;
    class Bitboards;
    class StateBitmasks;
}

namespace logic {

class KnightGenerator {

public:
    KnightGenerator(model::Board& board);
    void generate(bool isWhite, model::Movelist& moveList);

private:
    const model::Bitboards& _bitboards;
    const model::StateBitmasks& _stateBitmasks;
    std::vector<bitmask> _knightBitmasks;
};

} // namespace logic
```


--------------------

# FILE: src/logic/movegen/MoveGenerator.cpp

```cpp
#include "logic/movegen/MoveGenerator.h"

#include "logic/makemove/MoveMaker.h"
#include "logic/makemove/MoveRetractor.h"

#include "model/board/Board.h"
#include "model/move/Movelist.h"

#include "engine/pickmove/SearchMemory.h"

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
    bitmask enpessantTarget,
    unsigned char castlingRights)
{
    moveList.reset();
    genRookMoves(isWhite, moveList);
    genBishopMoves(isWhite, moveList);
    genQueenMoves(isWhite, moveList);
    genKnightMoves(isWhite, moveList);
    genKingMoves(isWhite, moveList);
    genPawnMoves(isWhite, moveList, enpessantTarget);
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
    bitmask enpessantTarget)
{
    _pawnGenerator.generate(isWhite, moveList, enpessantTarget);
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
```


--------------------

# FILE: src/logic/movegen/MoveGenerator.h

```cpp
#pragma once

#include "logic/movegen/PawnGenerator.h"
#include "logic/movegen/BishopGenerator.h"
#include "logic/movegen/KnightGenerator.h"
#include "logic/movegen/CastlingGenerator.h"
#include "logic/movegen/RookGenerator.h"
#include "logic/movegen/QueenGenerator.h"
#include "logic/movegen/KingGenerator.h"
#include "logic/movegen/CheckDetection.h"

#include "logic/movegen/utils/BitBasics.h"

namespace model {
    class Board;
    class Bitboards;
    class StateBitmasks;
    class Movelist;
}

namespace engine {
    class SearchMemory;
}

namespace logic {

class MoveMaker;
class MoveRetractor;

class MoveGenerator {

public:
    MoveGenerator(
        model::Board& board,
        logic::MoveMaker& moveMaker,
        logic::MoveRetractor& moveRetractor
    );

    void genMoves(
        bool isWhite,
        model::Movelist& moveList,
        bitmask enpessantTarget,
        unsigned char castlingRight
    );
    
    void genRookMoves(bool isWhite, model::Movelist& moveList);
    void genBishopMoves(bool isWhite, model::Movelist& moveList);
    void genQueenMoves(bool isWhite, model::Movelist& moveList);
    void genKnightMoves(bool isWite, model::Movelist& moveList);
    void genKingMoves(bool isWhite, model::Movelist& moveList);
    
    void genPawnMoves(
        bool isWhite,
        model::Movelist& moveList,
        bitmask enpessantTarget
    );

    void genCastlingMoves(
        bool isWhite,
        model::Movelist& moveList,
        unsigned char castlingRights
    );
    
    bool isInCheck(bool isWhite);

private:
    const model::Bitboards& _bitboards;
    const model::StateBitmasks& _stateBitmasks;
    
    logic::MoveMaker& _moveMaker;
    logic::MoveRetractor& _moveRetractor;
    
    CheckDetection _checkDetection;
    RookGenerator _rookGenerator;
    BishopGenerator _bishopGenerator;
    QueenGenerator _queenGenerator;
    KnightGenerator _knightGenerator;
    KingGenerator _kingGenerator;
    PawnGenerator _pawnGenerator;
    CastlingGenerator _castlingGenerator;
};

} // namespace logic
```


--------------------

# FILE: src/logic/movegen/PawnGenerator.cpp

```cpp
#include "logic/movegen/PawnGenerator.h"

#include "model/board/Board.h"
#include "model/move/Movelist.h"

#include "logic/movegen/bitmasks/PawnBitmasks.h"
#include "logic/movegen/utils/Containers.h"
#include "logic/movegen/utils/ChessUtils.h"
#include "logic/movegen/utils/BitBasics.h"

namespace logic {

PawnGenerator::PawnGenerator(model::Board& board) 
    : _bitboards(board.bitboards)
    , _stateBitmasks(board.stateBitmasks)
{
    _whitePawnStraightMoveBitmasks = PawnBitmasks::getAllStraightPawnMoveBitmasks(true);
    _whitePawnCaptureMoveBitmasks = PawnBitmasks::getAllCapturePawnMoveBitmasks(true);
    _blackPawnStraightMoveBitmasks = PawnBitmasks::getAllStraightPawnMoveBitmasks(false);
    _blackPawnCaptureMoveBitmasks = PawnBitmasks::getAllCapturePawnMoveBitmasks(false);
}

void PawnGenerator::generate(
    bool isWhite,
    model::Movelist& moveList,
    bitmask enpessantTarget)
{
    std::vector<int>& pawnIndices = Containers::getPiecePositionIndices();
    std::vector<int>& freeMovesIndices = Containers::getLeapingPiecefreeMovesIndices();
    std::vector<int>& capturableMovesIndices = Containers::getLeapingPieceCapturableMovesIndices();

    BitBasics::getBitIndices(pawnIndices, isWhite ? _bitboards.getWhitePawnsBitboard()
                                              : _bitboards.getBlackPawnsBitboard());

    for (int currentPawnIndex : pawnIndices) {

        bitmask straightPawnMoveBitmask = isWhite ? _whitePawnStraightMoveBitmasks[currentPawnIndex]
                                                  : _blackPawnStraightMoveBitmasks[currentPawnIndex];

        bitmask capturePawnMoveBitmask = isWhite ? _whitePawnCaptureMoveBitmasks[currentPawnIndex]
                                                 : _blackPawnCaptureMoveBitmasks[currentPawnIndex];

        bitmask freePawnMoves = straightPawnMoveBitmask & _stateBitmasks.getEmptySquaresBitmask();
        
        bitmask enemyPieces = isWhite ? _stateBitmasks.getBlackPiecesBitmask()
                                      : _stateBitmasks.getWhitePiecesBitmask();
        
        bitmask capturablePawnMoves = capturePawnMoveBitmask & enemyPieces;

        BitBasics::getBitIndices(freeMovesIndices, freePawnMoves);
        BitBasics::getBitIndices(capturableMovesIndices, capturablePawnMoves);

        int offset = isWhite ? 8 : -8;
        bool canPromote = (isWhite && ChessUtils::rankFromBitIndex(currentPawnIndex) == 6) || (!isWhite && ChessUtils::rankFromBitIndex(currentPawnIndex) == 1);

        if (freeMovesIndices.size() == 2) {
            int singleStepIndex = (isWhite ? 0 : 1);
            int doubleStepIndex = (isWhite ? 1 : 0);
            
            moveList.addMove(model::Move(currentPawnIndex, freeMovesIndices[singleStepIndex], model::Move::QUITE_FLAG));
            moveList.addMove(model::Move(currentPawnIndex, freeMovesIndices[doubleStepIndex], model::Move::DOUBLE_PAWN_PUSH_FLAG));

        } else if (freeMovesIndices.size() == 1 && freeMovesIndices[0] == currentPawnIndex + offset) {
            // Only add them move it is direcly in front of the pawn, to avoid jumping over pieces
            if (canPromote) {
                moveList.addMove(model::Move(currentPawnIndex, freeMovesIndices[0], model::Move::KNIGHT_PROMO_FLAG));
                moveList.addMove(model::Move(currentPawnIndex, freeMovesIndices[0], model::Move::BISHOP_PROMO_FLAG));
                moveList.addMove(model::Move(currentPawnIndex, freeMovesIndices[0], model::Move::ROOK_PROMO_FLAG));
                moveList.addMove(model::Move(currentPawnIndex, freeMovesIndices[0], model::Move::QUEEN_PROMO_FLAG));
            
            } else {
                moveList.addMove(model::Move(currentPawnIndex, freeMovesIndices[0], model::Move::QUITE_FLAG));
            }
        }

        for (int capturablePawnMoveIndex : capturableMovesIndices) {
            if (canPromote) {
                moveList.addMove(model::Move(currentPawnIndex, capturablePawnMoveIndex, model::Move::QUEEN_PROMO_CAPTURE_FLAG));
                moveList.addMove(model::Move(currentPawnIndex, capturablePawnMoveIndex, model::Move::ROOK_PROMO_CAPTURE_FLAG));
                moveList.addMove(model::Move(currentPawnIndex, capturablePawnMoveIndex, model::Move::BISHOP_PROMO_CAPTURE_FLAG));
                moveList.addMove(model::Move(currentPawnIndex, capturablePawnMoveIndex, model::Move::KNIGHT_PROMO_CAPTURE_FLAG));
            } else {
                moveList.addMove(model::Move(currentPawnIndex, capturablePawnMoveIndex, model::Move::CAPTURE_FLAG));
            }
        }

        if ((capturePawnMoveBitmask & enpessantTarget) != 0) {
            moveList.addMove(model::Move(currentPawnIndex, BitBasics::indexOfLSB(capturePawnMoveBitmask & enpessantTarget), model::Move::EP_CAPTURE_FLAG));
        }
    }
}

} // namespace logic
```


--------------------

# FILE: src/logic/movegen/PawnGenerator.h

```cpp
#pragma once

#include "common.h"

namespace model {
    class Board;
    class Movelist;
    class Bitboards;
    class StateBitmasks;
}

namespace logic {

class PawnGenerator {

public:
    PawnGenerator(model::Board& board);
    
    void generate(
        bool isWhite,
        model::Movelist& moveList,
        bitmask enpessantTarget
    );

private:
    const model::Bitboards& _bitboards;
    const model::StateBitmasks& _stateBitmasks;

    std::vector<bitmask> _whitePawnStraightMoveBitmasks;
    std::vector<bitmask> _blackPawnStraightMoveBitmasks;
    std::vector<bitmask> _whitePawnCaptureMoveBitmasks;
    std::vector<bitmask> _blackPawnCaptureMoveBitmasks;
};

} // namespace logic
```


--------------------

# FILE: src/logic/movegen/QueenGenerator.cpp

```cpp
#include "logic/movegen/QueenGenerator.h"

#include "model/board/Board.h"

#include "logic/movegen/RayLogic.h"
#include "logic/movegen/utils/Containers.h"
#include "logic/movegen/utils/BitBasics.h"
#include "logic/movegen/utils/ChessUtils.h"

namespace logic {

QueenGenerator::QueenGenerator(model::Board& board) 
    : _bitboards(board.bitboards)
    , _stateBitmasks(board.stateBitmasks)
{
    _straightRayBitmasks = RayBitmasks::getAllStraightRayBitmasks();
    _diagonalRayBitmasks = RayBitmasks::getAllDiagonalRayBitmasks();
}

void QueenGenerator::generate(
    bool isWhite,
    model::Movelist& moveList)
{
    std::vector<int>& queenIndices = Containers::getPiecePositionIndices();

    BitBasics::getBitIndices(queenIndices, isWhite ? _bitboards.getWhiteQueensBitboard()
                                               : _bitboards.getBlackQueensBitboard());

    for (int currentQueenIndex : queenIndices) {
        RayBitmasks::StraightRays straightRays = _straightRayBitmasks[currentQueenIndex];
        RayBitmasks::DiagonalRays diagonalRays = _diagonalRayBitmasks[currentQueenIndex];
        
        int queenRank = ChessUtils::rankFromBitIndex(currentQueenIndex);
        int queenFile = ChessUtils::fileFromBitIndex(currentQueenIndex);

        RayLogic::addMovesFromStraightRay(
            straightRays.north,
            true,
            false,
            isWhite,
            currentQueenIndex,
            queenRank,
            queenFile,
            moveList,
            _stateBitmasks.getWhitePiecesBitmask(),
            _stateBitmasks.getOccupiedPiecesBitmask()
        );

        RayLogic::addMovesFromStraightRay(
            straightRays.east,
            false,
            true,
            isWhite,
            currentQueenIndex,
            queenRank,
            queenFile,
            moveList,
            _stateBitmasks.getWhitePiecesBitmask(),
            _stateBitmasks.getOccupiedPiecesBitmask()
        );

        RayLogic::addMovesFromStraightRay(
            straightRays.south,
            false,
            false,
            isWhite,
            currentQueenIndex,
            queenRank,
            queenFile,
            moveList,
            _stateBitmasks.getWhitePiecesBitmask(),
            _stateBitmasks.getOccupiedPiecesBitmask()
        );

        RayLogic::addMovesFromStraightRay(
            straightRays.west,
            true,
            true,
            isWhite,
            currentQueenIndex,
            queenRank,
            queenFile,
            moveList,
            _stateBitmasks.getWhitePiecesBitmask(),
            _stateBitmasks.getOccupiedPiecesBitmask()
        );

        RayLogic::addMovesFromDiagonalRay(
            diagonalRays.northEast,
            true,
            isWhite,
            currentQueenIndex,
            queenRank,
            queenFile,
            moveList,
            _stateBitmasks.getWhitePiecesBitmask(),
            _stateBitmasks.getOccupiedPiecesBitmask()
        );

        RayLogic::addMovesFromDiagonalRay(
            diagonalRays.southEast,
            false,
            isWhite,
            currentQueenIndex,
            queenRank,
            queenFile,
            moveList,
            _stateBitmasks.getWhitePiecesBitmask(),
            _stateBitmasks.getOccupiedPiecesBitmask()
        );

        RayLogic::addMovesFromDiagonalRay(
            diagonalRays.southWest,
            false,
            isWhite,
            currentQueenIndex,
            queenRank,
            queenFile,
            moveList,
            _stateBitmasks.getWhitePiecesBitmask(),
            _stateBitmasks.getOccupiedPiecesBitmask()
        );
        
        RayLogic::addMovesFromDiagonalRay(
            diagonalRays.northWest,
            true,
            isWhite,
            currentQueenIndex,
            queenRank,
            queenFile,
            moveList,
            _stateBitmasks.getWhitePiecesBitmask(),
            _stateBitmasks.getOccupiedPiecesBitmask()
        );
    }
}

} // namespace logic
```


--------------------

# FILE: src/logic/movegen/QueenGenerator.h

```cpp
#pragma once

#include "common.h"

#include "logic/movegen/bitmasks/RayBitmasks.h"

namespace model {
    class Movelist;
    class Board;
    class Bitboards;
    class StateBitmasks;
}

namespace logic {

class QueenGenerator {

public:
    QueenGenerator(model::Board& board);
    
    void generate(
        bool isWhite, 
        model::Movelist& moveList
    );

private:
    const model::Bitboards& _bitboards;
    const model::StateBitmasks& _stateBitmasks;

    std::vector<RayBitmasks::StraightRays> _straightRayBitmasks;
    std::vector<RayBitmasks::DiagonalRays> _diagonalRayBitmasks;
};

} // namespace logic
```


--------------------

# FILE: src/logic/movegen/RayLogic.cpp

```cpp
#include "logic/movegen/RayLogic.h"

#include "model/move/Movelist.h"

#include "model/move/Move.h"

#include "logic/movegen/utils/Containers.h"
#include "logic/movegen/utils/ChessUtils.h"
#include "logic/movegen/utils/BitBasics.h"

namespace logic {

void RayLogic::addMovesFromFreeRay(
    bitmask freeRay,
    int bitIndexFrom,
    model::Movelist& moveList)
{
    std::vector<int>& freeRayIndices = Containers::getSlidingPiecefreeMovesIndices();
    BitBasics::getBitIndices(freeRayIndices, freeRay);

    for (int bitIndex : freeRayIndices) {
        moveList.addMove(model::Move(bitIndexFrom, bitIndex, model::Move::QUITE_FLAG));
    }
}

void RayLogic::addMoveIfBlockerIsEnemy(
    int blockerIndex,
    bool isWhite,
    int bitIndexFrom,
    model::Movelist& moveList,
    bitmask whitePiecesBitmask)
{
    bool blockerIsWhite = BitBasics::getBit(whitePiecesBitmask, blockerIndex);

    if (blockerIsWhite != isWhite)
        moveList.addMove(model::Move(bitIndexFrom, blockerIndex, model::Move::CAPTURE_FLAG));
}

void RayLogic::addMovesBetweenBlockerAndPieceOnStraightRay(
    int blockerIndex,
    bool alongFile, 
    bool startFromBlocker, int rookRank, 
    int rookFile, 
    int bitIndexFrom,
    model::Movelist& moveList)
{
    int start = startFromBlocker 
                ? (alongFile ? ChessUtils::fileFromBitIndex(blockerIndex) 
                             : ChessUtils::rankFromBitIndex(blockerIndex)) 
                : (alongFile ? rookFile 
                             : rookRank);
                            
    int stop = startFromBlocker 
               ? (alongFile ? rookFile 
                            : rookRank) 
               : (alongFile ? ChessUtils::fileFromBitIndex(blockerIndex) 
                            : ChessUtils::rankFromBitIndex(blockerIndex));

    for (int i = start - 1; i > stop; --i) {
        int rankOrFileIndex = alongFile ? rookRank * 8 + i 
                                        : i * 8 + rookFile;
        
        moveList.addMove(model::Move(bitIndexFrom, rankOrFileIndex, model::Move::QUITE_FLAG));
    }
}

void RayLogic::addMovesBetweenBlockerAndPieceOnDiagonalRay(
    int blockerIndex,
    bool startFromBlocker, 
    int bishopRank, 
    int bishopFile, 
    int bitIndexFrom,
    model::Movelist& moveList)
{
    int startRank = startFromBlocker
                    ? ChessUtils::rankFromBitIndex(blockerIndex)
                    : bishopRank;

    int startFile = startFromBlocker
                    ? ChessUtils::fileFromBitIndex(blockerIndex)
                    : bishopFile;

    int stopRank = startFromBlocker
                   ? bishopRank
                   : ChessUtils::rankFromBitIndex(blockerIndex);

    int stopFile = startFromBlocker 
                   ? bishopFile
                   : ChessUtils::fileFromBitIndex(blockerIndex);

    int rankDiff = startRank - stopRank;
    int fileDiff = startFile - stopFile;

    int rankIncrement = rankDiff > 0 ? -1 : 1;
    int fileIncrement = fileDiff > 0 ? -1 : 1;

    for (int i = startRank + rankIncrement, j = startFile + fileIncrement; i != stopRank; i += rankIncrement, j += fileIncrement) {
        int rankOrFileIndex = i * 8 + j;

        moveList.addMove(model::Move(bitIndexFrom, rankOrFileIndex, model::Move::QUITE_FLAG));
    }
}

void RayLogic::addMovesFromStraightRay(
    bitmask ray, 
    bool blockerOnLSB, 
    bool alongFile, 
    bool isWhite, 
    int pieceIndex, 
    int pieceRank, 
    int pieceFile, 
    model::Movelist& moveList,
    bitmask whitePiecesBitmask,
    bitmask occupiedPiecesBitmask) 
{
        bitmask blockerBitmask = ray & occupiedPiecesBitmask;          

        if (blockerBitmask != 0) {
            int blockerIndex = blockerOnLSB
                               ? BitBasics::indexOfLSB(blockerBitmask)
                               : BitBasics::indexOfMSB(blockerBitmask);
                               
            addMoveIfBlockerIsEnemy(
                blockerIndex,
                isWhite,
                pieceIndex, 
                moveList,
                whitePiecesBitmask
            );

            addMovesBetweenBlockerAndPieceOnStraightRay(
                blockerIndex,
                alongFile, 
                blockerOnLSB, 
                pieceRank, 
                pieceFile, 
                pieceIndex, 
                moveList
            );

        } else {
            addMovesFromFreeRay(ray, pieceIndex, moveList);
        }
}

void RayLogic::addMovesFromDiagonalRay(
    bitmask ray, 
    bool blockerOnLSB, 
    bool isWhite, 
    int pieceIndex, 
    int pieceRank, 
    int pieceFile, 
    model::Movelist& moveList,
    bitmask whitePiecesBitmask,
    bitmask occupiedPiecesBitmask)
{
    bitmask blockerBitmask = ray & occupiedPiecesBitmask;

    if (blockerBitmask != 0) {
        int blockerIndex = blockerOnLSB
                           ? BitBasics::indexOfLSB(blockerBitmask) 
                           : BitBasics::indexOfMSB(blockerBitmask);

        addMoveIfBlockerIsEnemy(
            blockerIndex, 
            isWhite, 
            pieceIndex, 
            moveList,
            whitePiecesBitmask
        );

        addMovesBetweenBlockerAndPieceOnDiagonalRay(
            blockerIndex, 
            blockerOnLSB,
            pieceRank, 
            pieceFile, 
            pieceIndex, 
            moveList
        );

    } else {
        addMovesFromFreeRay(ray, pieceIndex, moveList);
    }
}

bool RayLogic::checkStraightRay(
    bitmask straightRay, 
    bool firstBlockerOnLSB, 
    bitmask opponentRooksAndQueens,
    bitmask occupiedPiecesBitmask) 
{
    bitmask rooksAndQueensBlockerBitmask = straightRay & opponentRooksAndQueens;
    
    // There must be a rook or a queen on the file or rank to be in check
    if (rooksAndQueensBlockerBitmask == 0ULL)
        return false;

    bitmask occupiedBlockerBitmask = straightRay & occupiedPiecesBitmask;

    // If there is only one blocker out of all pieces, then it must be a rook or a queen thus the king is in check
    if (BitBasics::popCount(occupiedBlockerBitmask) == 1)
        return true;

    int occupiedBlockerIndex = firstBlockerOnLSB ? BitBasics::indexOfLSB(occupiedBlockerBitmask)
                                                 : BitBasics::indexOfMSB(occupiedBlockerBitmask);

    int rooksAndQueensBlockerIndex = firstBlockerOnLSB ? BitBasics::indexOfLSB(rooksAndQueensBlockerBitmask)
                                                       : BitBasics::indexOfMSB(rooksAndQueensBlockerBitmask);

    // If the the first blocker of any piece is the same as the first blocker of a rook or queen, then the king is in check
    return occupiedBlockerIndex == rooksAndQueensBlockerIndex;
}

bool RayLogic::checkDiagonalRay(
    bitmask diagonalRay, 
    bool firstBlockerOnLSB, 
    bitmask opponentBishopsAndQueens,
    bitmask occupiedPiecesBitmask)
{
    bitmask bishopsAndQueensBlockerBitmask = diagonalRay & opponentBishopsAndQueens;

    if ((bishopsAndQueensBlockerBitmask) == 0)
        return false;

    bitmask occupiedBlockerBitmask = diagonalRay & occupiedPiecesBitmask;

    if (BitBasics::popCount(occupiedBlockerBitmask) == 1)
        return true;

    int occupiedBlockerIndex = firstBlockerOnLSB ? BitBasics::indexOfLSB(occupiedBlockerBitmask)
                                                 : BitBasics::indexOfMSB(occupiedBlockerBitmask);

    int bishopsAndQueensBlockerIndex = firstBlockerOnLSB ? BitBasics::indexOfLSB(bishopsAndQueensBlockerBitmask)
                                                         : BitBasics::indexOfMSB(bishopsAndQueensBlockerBitmask);

    return occupiedBlockerIndex == bishopsAndQueensBlockerIndex;
}

} // namespace logic
```


--------------------

# FILE: src/logic/movegen/RayLogic.h

```cpp
#pragma once

#include "model/move/Movelist.h"

namespace logic {

struct RayLogic {

    static void addMovesFromFreeRay(
        bitmask freeRay,
        int bitIndexFrom,
        model::Movelist& moveList
    );

    static void addMoveIfBlockerIsEnemy(
        int blockerIndex,
        bool isWhite,
        int bitIndexFrom,
        model::Movelist& moveList,
        bitmask whitePiecesBitmask
    );

    static void addMovesBetweenBlockerAndPieceOnStraightRay(
        int blockerIndex,
        bool alongFile, 
        bool startFromBlocker, int rookRank, 
        int rookFile, 
        int bitIndexFrom,
        model::Movelist& moveList
    );

    static void addMovesBetweenBlockerAndPieceOnDiagonalRay(
        int blockerIndex,
        bool startFromBlocker, 
        int bishopRank, 
        int bishopFile, 
        int bitIndexFrom,
        model::Movelist& moveList
    );
    
    static void addMovesFromStraightRay(
        bitmask ray,
        bool blockerOnLSB,
        bool alongFile,
        bool isWhite,
        int pieceIndex,
        int pieceRank,
        int pieceFile,
        model::Movelist& moveList,
        bitmask whitePiecesBitmask,
        bitmask occupiedPiecesBitmask
    );
    
    static void addMovesFromDiagonalRay(
        bitmask ray,
        bool blockerOnLSB,
        bool isWhite,
        int pieceIndex,
        int pieceRank,
        int pieceFile,
        model::Movelist& moveList,
        bitmask whitePiecesBitmask,
        bitmask occupiedPiecesBitmask
    );
    
    static bool checkStraightRay(
        bitmask straightRay,
        bool firstBlockerOnLSB,
        bitmask opponentRooksAndQueens,
        bitmask occupiedPiecesBitmask
    );
    
    static bool checkDiagonalRay(
        bitmask diagonalRay,
        bool firstBlockerOnLSB,
        bitmask opponentBishopsAndQueens,
        bitmask occupiedPiecesBitmask
    );

};

} // namespace logic
```


--------------------

# FILE: src/logic/movegen/RookGenerator.cpp

```cpp
#include "logic/movegen/RookGenerator.h"

#include "model/board/Board.h"

#include "logic/movegen/RayLogic.h"
#include "logic/movegen/utils/Containers.h"
#include "logic/movegen/utils/ChessUtils.h"
#include "logic/movegen/utils/BitBasics.h"

namespace logic {

RookGenerator::RookGenerator(model::Board& board)
    : _bitboards(board.bitboards)
    , _stateBitmasks(board.stateBitmasks)
{
    _straightRayBitmasks = RayBitmasks::getAllStraightRayBitmasks();
}

void RookGenerator::generate(
    bool isWhite,
    model::Movelist& moveList)
{
    std::vector<int>& rookIndices = Containers::getPiecePositionIndices();

    RayBitmasks::StraightRays rays;

    BitBasics::getBitIndices(rookIndices, isWhite ? _bitboards.getWhiteRooksBitboard()
                                       : _bitboards.getBlackRooksBitboard());

    // Loop through all rooks and isolate them
    for (int currentRookIndex : rookIndices) {
        rays = _straightRayBitmasks[currentRookIndex];
        int rookRank = ChessUtils::rankFromBitIndex(currentRookIndex);
        int rookFile = ChessUtils::fileFromBitIndex(currentRookIndex);

        RayLogic::addMovesFromStraightRay(
            rays.north,
            true, 
            false, 
            isWhite, 
            currentRookIndex, 
            rookRank, 
            rookFile, 
            moveList,
            _stateBitmasks.getWhitePiecesBitmask(),
            _stateBitmasks.getOccupiedPiecesBitmask()
        );

        RayLogic::addMovesFromStraightRay(
            rays.east,
            false, 
            true, 
            isWhite, 
            currentRookIndex, 
            rookRank, 
            rookFile, 
            moveList,
            _stateBitmasks.getWhitePiecesBitmask(),
            _stateBitmasks.getOccupiedPiecesBitmask()
        );

        RayLogic::addMovesFromStraightRay(
            rays.south,
            false, 
            false, 
            isWhite, 
            currentRookIndex, 
            rookRank, 
            rookFile, 
            moveList,
            _stateBitmasks.getWhitePiecesBitmask(),
            _stateBitmasks.getOccupiedPiecesBitmask()
        );

        RayLogic::addMovesFromStraightRay(
            rays.west,
            true, 
            true, 
            isWhite, 
            currentRookIndex, 
            rookRank, 
            rookFile, 
            moveList,
            _stateBitmasks.getWhitePiecesBitmask(),
            _stateBitmasks.getOccupiedPiecesBitmask()
        );
    }
}

} // namespace logic
```


--------------------

# FILE: src/logic/movegen/RookGenerator.h

```cpp
#pragma once

#include "common.h"

#include "logic/movegen/bitmasks/RayBitmasks.h"

namespace model {
    class Movelist;
    class Board;
    class Bitboards;
    class StateBitmasks;
}

namespace logic {

class RookGenerator {

public:
    RookGenerator(model::Board& board);
    
    void generate(
        bool isWhite,
        model::Movelist& moveList
    );

private:
    const model::Bitboards& _bitboards;
    const model::StateBitmasks& _stateBitmasks;

    std::vector<RayBitmasks::StraightRays> _straightRayBitmasks;
};

} // namespace logic
```


--------------------

# FILE: src/logic/movegen/utils/BitBasics.cpp

```cpp
#include "logic/movegen/utils/BitBasics.h"

#include <iostream>
#include <bitset>

namespace logic {

void BitBasics::getBitIndices(std::vector<int>& indices, bitboard bitboard)
{ 
    indices.clear();

    while (bitboard != 0) {
        int index = __builtin_ctzll(bitboard);

        indices.push_back(index);

        bitboard &= (bitboard - 1);
    }
}

} // namespace logic
```


--------------------

# FILE: src/logic/movegen/utils/BitBasics.h

```cpp
#pragma once

#include "common.h"

namespace logic {

struct BitBasics {

    static bool getBit(bitmask a, int i);
    static int indexOfLSB(bitmask a); 
    static int indexOfMSB(bitmask a);
    static void getBitIndices(std::vector<int>& indices, bitmask bitboard);
    
    static int popCount(bitmask a);
    static void printBitmaskInBits(bitmask n);
    
};

} // namespace logic

#include "logic/movegen/utils/BitBasics.inl"
```


--------------------

# FILE: src/logic/movegen/utils/BitBasics.inl

```
namespace logic {

inline bool BitBasics::getBit(bitmask a, int i) 
{
    return (a >> i) & 1;
}

inline int BitBasics::indexOfLSB(bitmask a) 
{
    return __builtin_ctzll(a);
}   

inline int BitBasics::indexOfMSB(bitmask a) 
{
    return 63 - __builtin_clzll(a);
}

inline int BitBasics::popCount(bitmask a) 
{
    return __builtin_popcountll(a);
}

} // namespace logic
```


--------------------

# FILE: src/logic/movegen/utils/CMakeLists.txt

```
target_sources(movegen PRIVATE
    BitBasics.cpp
    Containers.cpp
    ChessUtils.cpp
)
```


--------------------

# FILE: src/logic/movegen/utils/ChessUtils.cpp

```cpp
#include "logic/movegen/utils/ChessUtils.h"

namespace logic {

int ChessUtils::manhattanDistance(int i, int j) 
{
    int rankDiff = rankFromBitIndex(i) - rankFromBitIndex(j);
    int fileDiff = fileFromBitIndex(i) - fileFromBitIndex(j);
    return abs(rankDiff) + abs(fileDiff);
}

}
```


--------------------

# FILE: src/logic/movegen/utils/ChessUtils.h

```cpp
#pragma once

#include "model/board/PieceType.h"

namespace logic {
    
struct ChessUtils {
    
    static bitmask getFileMask(int file);
    static bitmask getRankMask(int rank);
    static int rankFromBitIndex(int i);
    static int fileFromBitIndex(int i);
    static bitmask getOccupiedSquaresBitmask(bitmask whitePieces, bitmask blackPieces);
    static bitmask getEmptySquaresBitmask(bitmask whitePieces, bitmask blackPieces);
    static bitmask getWhiteSquaresBitmask();
    static bitmask getBlackSquaresBitmask();
    
    static int abs(int n);
    static int manhattanDistance(int i, int j);

};

} // namespace logic

#include "logic/movegen/utils/ChessUtils.inl"
```


--------------------

# FILE: src/logic/movegen/utils/ChessUtils.inl

```
namespace {

    constexpr bitmask FILE_MASK[8] = {
        0x0101010101010101ULL,
        0x0202020202020202ULL,
        0x0404040404040404ULL,
        0x0808080808080808ULL,
        0x1010101010101010ULL,
        0x2020202020202020ULL,
        0x4040404040404040ULL,
        0x8080808080808080ULL
    };
    
    constexpr bitmask RANK_MASK[8] = {
        0x00000000000000FFULL,
        0x000000000000FF00ULL,
        0x0000000000FF0000ULL,
        0x00000000FF000000ULL,
        0x000000FF00000000ULL,
        0x0000FF0000000000ULL,
        0x00FF000000000000ULL,
        0xFF00000000000000ULL
    };

} // namespace 

namespace logic {

inline bitmask ChessUtils::getFileMask(int file) 
{
    return FILE_MASK[file];
}

inline bitmask ChessUtils::getRankMask(int rank) 
{
    return RANK_MASK[rank];
}

inline int ChessUtils::rankFromBitIndex(int i) 
{
    return i / 8;
}

inline int ChessUtils::fileFromBitIndex(int i) 
{
    return i % 8;
}

inline bitmask ChessUtils::getOccupiedSquaresBitmask(bitmask whitePieces, bitmask blackPieces) 
{
    return whitePieces | blackPieces;
}

inline bitmask ChessUtils::getEmptySquaresBitmask(bitmask whitePieces, bitmask blackPieces) 
{
    return ~getOccupiedSquaresBitmask(whitePieces, blackPieces);
}

inline bitmask ChessUtils::getWhiteSquaresBitmask() 
{
    return 0xAA55AA55AA55AA55ULL;
}

inline bitmask ChessUtils::getBlackSquaresBitmask() 
{
    return 0x55AA55AA55AA55AAULL;
}

inline int ChessUtils::abs(int n) {
    return (n < 0) ? -n : n;
}

} // namespace logic
```


--------------------

# FILE: src/logic/movegen/utils/Containers.cpp

```cpp
#include "logic/movegen/utils/Containers.h"

namespace logic {

std::vector<int> Containers::_slidingPiecefreeMovesIndices;
std::vector<int> Containers::_leapingPiecefreeMovesIndices;
std::vector<int> Containers::_slidingPieceCapturableMovesIndices;
std::vector<int> Containers::_leapingPieceCapturableMovesIndices;
std::vector<int> Containers::_piecePositionIndices;

void Containers::init() {
    _slidingPiecefreeMovesIndices = std::vector<int>(32, 0);
    _leapingPiecefreeMovesIndices = std::vector<int>(8, 0);

    _slidingPieceCapturableMovesIndices = std::vector<int>(32, 0);
    _leapingPieceCapturableMovesIndices = std::vector<int>(8, 0);

    _piecePositionIndices = std::vector<int>(16, 0);
}

std::vector<int>& Containers::getSlidingPiecefreeMovesIndices()
{
    return _slidingPiecefreeMovesIndices;
}

std::vector<int>& Containers::getLeapingPiecefreeMovesIndices()
{
    return _leapingPiecefreeMovesIndices;
}

std::vector<int>& Containers::getSlidingPieceCapturableMovesIndices()
{
    return _slidingPieceCapturableMovesIndices;
}

std::vector<int>& Containers::getLeapingPieceCapturableMovesIndices()
{
    return _leapingPieceCapturableMovesIndices;
}

std::vector<int>& Containers::getPiecePositionIndices()
{
    return _piecePositionIndices;
}

} // namespace logic
```


--------------------

# FILE: src/logic/movegen/utils/Containers.h

```cpp
#pragma once

#include "common.h"

namespace logic {

class Containers {

public:
    static void init();

    static std::vector<int>& getSlidingPiecefreeMovesIndices();
    static std::vector<int>& getLeapingPiecefreeMovesIndices();
    static std::vector<int>& getSlidingPieceCapturableMovesIndices();
    static std::vector<int>& getLeapingPieceCapturableMovesIndices();
    static std::vector<int>& getPiecePositionIndices();

private:
    static std::vector<int> _slidingPiecefreeMovesIndices;
    static std::vector<int> _leapingPiecefreeMovesIndices;
    
    static std::vector<int> _slidingPieceCapturableMovesIndices;
    static std::vector<int> _leapingPieceCapturableMovesIndices;

    static std::vector<int> _piecePositionIndices;
};

} // namespace logic
```


--------------------

# FILE: src/logic/movegen/utils/Containers.inl

```
namespace logic {

inline std::vector<int>& Containers::getSlidingPiecefreeMovesIndices()
{
    return _slidingPiecefreeMovesIndices;
}

inline std::vector<int>& Containers::getLeapingPiecefreeMovesIndices()
{
    return _leapingPiecefreeMovesIndices;
}

inline std::vector<int>& Containers::getSlidingPieceCapturableMovesIndices()
{
    return _slidingPieceCapturableMovesIndices;
}

inline std::vector<int>& Containers::getLeapingPieceCapturableMovesIndices()
{
    return _leapingPieceCapturableMovesIndices;
}

inline std::vector<int>& Containers::getPiecePositionIndices()
{
    return _piecePositionIndices;
}

} // namespace logic
```


--------------------

# FILE: src/logic/movegen/bitmasks/BitmasksGenerator.cpp

```cpp

```


--------------------

# FILE: src/logic/movegen/bitmasks/BitmasksGenerator.h

```cpp

```


--------------------

# FILE: src/logic/movegen/bitmasks/CMakeLists.txt

```
target_sources(movegen PRIVATE
    RayBitmasks.cpp
    KnightBitmasks.cpp
    KingBitmasks.cpp
    PawnBitmasks.cpp
)
```


--------------------

# FILE: src/logic/movegen/bitmasks/KingBitmasks.cpp

```cpp
#include "logic/movegen/bitmasks/KingBitmasks.h"

#include "logic/movegen/utils/ChessUtils.h"

namespace logic {

// Applies all king moves without considering looping around the board
bitmask KingBitmasks::applyKingMoves(int ind) 
{
    bitmask kingMoves = 0ULL;

    if (ind + 8 <= 63) kingMoves |= (1ULL << (ind + 8));
    if (ind + 1 <= 63) kingMoves |= (1ULL << (ind + 1));
    if (ind - 8 >= 0) kingMoves |= (1ULL << (ind - 8));
    if (ind - 1 >= 0) kingMoves |= (1ULL << (ind - 1));
    if (ind + 7 <= 63) kingMoves |= (1ULL << (ind + 7));
    if (ind + 9 <= 63) kingMoves |= (1ULL << (ind + 9));
    if (ind - 7 >= 0) kingMoves |= (1ULL << (ind - 7));
    if (ind - 9 >= 0) kingMoves |= (1ULL << (ind - 9));

    return kingMoves;
}

// Applies file masks to king moves to prevent looping around the board
bitmask KingBitmasks::removeWrapAroundKingMoves(bitmask kingMoves, int ind) 
{
    bitmask notAFile = ~ChessUtils::getFileMask(7);
    bitmask notHFile = ~ChessUtils::getFileMask(0);

    switch (ChessUtils::fileFromBitIndex(ind)) {
        case 0:
            kingMoves &= notAFile;
            break;
        case 7:
            kingMoves &= notHFile;
            break;
    }

    return kingMoves;
}

bitmask KingBitmasks::getKingBitmask(int ind) 
{
    bitmask kingMoves = applyKingMoves(ind);
    kingMoves = removeWrapAroundKingMoves(kingMoves, ind);
    return kingMoves;
}

const std::vector<bitmask> KingBitmasks::getAllKingBitmasks() 
{
    std::vector<bitmask> kingBitmasks;

    for (int i = 0; i < 64; i++) {
        kingBitmasks.push_back(getKingBitmask(i));
    }

    return kingBitmasks;
}

} // namespace logic
```


--------------------

# FILE: src/logic/movegen/bitmasks/KingBitmasks.h

```cpp
#pragma once

#include "common.h"

namespace logic {

struct KingBitmasks {

    static bitmask getKingBitmask(int ind);
    static const std::vector<bitmask> getAllKingBitmasks();
    
    static bitmask applyKingMoves(int ind);
    static bitmask removeWrapAroundKingMoves(bitmask kingMoves, int ind);

};

} // namespace logic
```


--------------------

# FILE: src/logic/movegen/bitmasks/KnightBitmasks.cpp

```cpp
#include "logic/movegen/bitmasks/KnightBitmasks.h"

#include "logic/movegen/utils/ChessUtils.h"

namespace logic {

// Applies all knight moves without considering looping around the board
bitmask KnightBitmasks::applyKnightMoves(int ind) 
{
    bitmask knightMoves = 0ULL;

    if (ind + 15 <= 63) knightMoves |= (1ULL << (ind + 15));
    if (ind + 6 <= 63) knightMoves |= (1ULL << (ind + 6));
    if (ind - 10 >= 0) knightMoves |= (1ULL << (ind - 10));
    if (ind - 17 >= 0) knightMoves |= (1ULL << (ind - 17));
    if (ind - 15 >= 0) knightMoves |= (1ULL << (ind - 15));
    if (ind - 6 >= 0) knightMoves |= (1ULL << (ind - 6));
    if (ind + 10 <= 63) knightMoves |= (1ULL << (ind + 10));
    if (ind + 17 <= 63) knightMoves |= (1ULL << (ind + 17));

    return knightMoves;
}

// Applies file masks to knight moves to prevent looping around the board
bitmask KnightBitmasks::removeWrapAroundKnightMoves(bitmask knightMoves, int ind) 
{
    bitmask aFile = ChessUtils::getFileMask(7);
    bitmask bFile = ChessUtils::getFileMask(6);
    bitmask notAorBFile = ~(aFile | bFile);

    bitmask gFile = ChessUtils::getFileMask(1);
    bitmask hFile = ChessUtils::getFileMask(0);
    bitmask notGorHFile = ~(gFile | hFile);

    switch (ChessUtils::fileFromBitIndex(ind)) {
        case 0:
            knightMoves &= notAorBFile;
            break;
        case 1:
            knightMoves &= notAorBFile;
            break;
        case 6:
            knightMoves &= notGorHFile;
            break;
        case 7:
            knightMoves &= notGorHFile;
            break;
    }

    return knightMoves;
}

bitmask KnightBitmasks::getKnightBitmask(int ind) 
{
    bitmask knightMoves = applyKnightMoves(ind);
    knightMoves = removeWrapAroundKnightMoves(knightMoves, ind);
    return knightMoves;
}

const std::vector<bitmask> KnightBitmasks::getAllKnightBitmasks() 
{
    std::vector<bitmask> knightBitmasks;

    for (int i = 0; i < 64; i++) {
        knightBitmasks.push_back(getKnightBitmask(i));
    }

    return knightBitmasks;
}

} // namespace logic
```


--------------------

# FILE: src/logic/movegen/bitmasks/KnightBitmasks.h

```cpp
#pragma once

#include "common.h"

namespace logic {

struct KnightBitmasks {

    static bitmask getKnightBitmask(int ind);
    static const std::vector<bitmask> getAllKnightBitmasks();
    
    static bitmask applyKnightMoves(int ind);
    static bitmask removeWrapAroundKnightMoves(bitmask knightMoves, int ind);

};

} // namespace logic
```


--------------------

# FILE: src/logic/movegen/bitmasks/PawnBitmasks.cpp

```cpp
#include "logic/movegen/bitmasks/PawnBitmasks.h"

#include "logic/movegen/utils/ChessUtils.h"

namespace logic {

bitmask PawnBitmasks::applyCapturePawnMoves(int ind, bool isWhite) 
{
    bitmask captureMoves = 0ULL;

    int shiftUpLeft = isWhite ? 9 : -9;
    int shiftUpRight = isWhite ? 7 : -7;

    if (ind + shiftUpLeft <= 63 && ind + shiftUpLeft >= 0) captureMoves |= (1ULL << (ind + shiftUpLeft));
    if (ind + shiftUpRight <= 63 && ind + shiftUpRight >= 0) captureMoves |= (1ULL << (ind + shiftUpRight));

    return captureMoves;
}

bitmask PawnBitmasks::removeWrapAroundPawnMoves(bitmask captureMoves, int ind) 
{
    bitmask notAFile = ~ChessUtils::getFileMask(7);
    bitmask notHFile = ~ChessUtils::getFileMask(0);

    switch (ChessUtils::fileFromBitIndex(ind)) {
        case 0:
            captureMoves &= notAFile;
            break;
        case 7:
            captureMoves &= notHFile;
            break;
    }

    return captureMoves;
}

bitmask PawnBitmasks::getCapturePawnMovesBitmask(int ind, bool isWhite) 
{
    bitmask captureMoves = applyCapturePawnMoves(ind, isWhite);
    captureMoves = removeWrapAroundPawnMoves(captureMoves, ind);
    return captureMoves;
}

bitmask PawnBitmasks::getStraightPawnMovesBitmask(int ind, bool isWhite) 
{
    bitmask straightMoves = 0ULL;

    int shiftUp = isWhite ? 8 : -8;
    int shiftUpTwice = isWhite ? 16 : -16;

    if (ind + shiftUp <= 63 && ind + shiftUp >= 0) straightMoves |= (1ULL << (ind + shiftUp));
    if (isWhite && (ind >= 8 && ind <= 15)) {
        if (ind + shiftUpTwice <= 63 && ind + shiftUpTwice >= 0) straightMoves |= (1ULL << (ind + shiftUpTwice));
    } else if (!isWhite && (ind >= 48 && ind <= 55)) {
        if (ind + shiftUpTwice <= 63 && ind + shiftUpTwice >= 0) straightMoves |= (1ULL << (ind + shiftUpTwice));
    }
    return straightMoves;
}

std::vector<bitmask> PawnBitmasks::getAllStraightPawnMoveBitmasks(bool isWhite) 
{
    std::vector<bitmask> straightPawnMoveBitmasks;

    for (int i = 0; i < 64; i++) {
        straightPawnMoveBitmasks.push_back(getStraightPawnMovesBitmask(i, isWhite));
    }

    return straightPawnMoveBitmasks;
}

std::vector<bitmask> PawnBitmasks::getAllCapturePawnMoveBitmasks(bool isWhite) 
{
    std::vector<bitmask> capturePawnMoveBitmasks;

    for (int i = 0; i < 64; i++) {
        capturePawnMoveBitmasks.push_back(getCapturePawnMovesBitmask(i, isWhite));
    }

    return capturePawnMoveBitmasks;
}

} // namespace logic
```


--------------------

# FILE: src/logic/movegen/bitmasks/PawnBitmasks.h

```cpp
#pragma once

#include "common.h"

namespace logic {

struct MoveType {
    std::vector<bitmask> diagonal;
    std::vector<bitmask> straight;
};

struct PawnBitmasks {

    static bitmask getCapturePawnMovesBitmask(int ind, bool isWhite);
    static bitmask getStraightPawnMovesBitmask(int ind, bool isWhite);
    static std::vector<bitmask> getAllStraightPawnMoveBitmasks(bool isWhite);
    static std::vector<bitmask> getAllCapturePawnMoveBitmasks(bool isWhite);
    
    static bitmask applyCapturePawnMoves(int ind, bool isWhite);
    static bitmask removeWrapAroundPawnMoves(bitmask pawnMoves, int ind);

};

} // namespace logic
```


--------------------

# FILE: src/logic/movegen/bitmasks/RayBitmasks.cpp

```cpp
#include "logic/movegen/bitmasks/RayBitmasks.h"

#include "logic/movegen/utils/ChessUtils.h"

namespace logic {

bitmask RayBitmasks::getNorthRay(int ind) 
{
    int rank = ChessUtils::rankFromBitIndex(ind);
    int file = ChessUtils::fileFromBitIndex(ind);

    bitmask north_ray = 0ULL;

    for (int i = rank + 1; i < 8; i++) {
        north_ray |= (1ULL << (i * 8 + file));
    }

    return north_ray;
}

bitmask RayBitmasks::getNorthEastRay(int ind) 
{
    int rank = ChessUtils::rankFromBitIndex(ind);
    int file = ChessUtils::fileFromBitIndex(ind);

    bitmask north_east_ray = 0ULL;

    for (int i = 1; i < 8; i++) {
        if (rank + i < 8 && file - i >= 0) {
            north_east_ray |= (1ULL << ((rank + i) * 8 + file - i));
        }
    }

    return north_east_ray;
}

bitmask RayBitmasks::getEastRay(int ind) 
{
    int rank = ChessUtils::rankFromBitIndex(ind);
    int file = ChessUtils::fileFromBitIndex(ind);

    bitmask east_ray = 0ULL;

    for (int i = file - 1; i >= 0; i--) {
        east_ray |= (1ULL << (rank * 8 + i));
    }

    return east_ray;
}

bitmask RayBitmasks::getSouthEastRay(int ind) 
{
    int rank = ChessUtils::rankFromBitIndex(ind);
    int file = ChessUtils::fileFromBitIndex(ind);

    bitmask south_east_ray = 0ULL;

    for (int i = 1; i < 8; i++) {
        if (rank - i >= 0 && file - i >= 0) {
            south_east_ray |= (1ULL << ((rank - i) * 8 + file - i));
        }
    }

    return south_east_ray;
}

bitmask RayBitmasks::getSouthRay(int ind) 
{
    int rank = ChessUtils::rankFromBitIndex(ind);
    int file = ChessUtils::fileFromBitIndex(ind);

    bitmask south_ray = 0ULL;

    for (int i = rank - 1; i >= 0; i--) {
        south_ray |= (1ULL << (i * 8 + file));
    }

    return south_ray;
}

bitmask RayBitmasks::getSouthWestRay(int ind) 
{
    int rank = ChessUtils::rankFromBitIndex(ind);
    int file = ChessUtils::fileFromBitIndex(ind);

    bitmask south_west_ray = 0ULL;

    for (int i = 1; i < 8; i++) {
        if (rank - i >= 0 && file + i < 8) {
            south_west_ray |= (1ULL << ((rank - i) * 8 + file + i));
        }
    }

    return south_west_ray;
}

bitmask RayBitmasks::getWestRay(int ind) 
{
    int rank = ChessUtils::rankFromBitIndex(ind);
    int file = ChessUtils::fileFromBitIndex(ind);

    bitmask west_ray = 0ULL;

    for (int i = file + 1; i < 8; i++) {
        west_ray |= (1ULL << (rank * 8 + i));
    }

    return west_ray;
}

bitmask RayBitmasks::getNorthWestRay(int ind) 
{
    int rank = ChessUtils::rankFromBitIndex(ind);
    int file = ChessUtils::fileFromBitIndex(ind);

    bitmask north_west_ray = 0ULL;

    for (int i = 1; i < 8; i++) {
        if (rank + i < 8 && file + i < 8) {
            north_west_ray |= (1ULL << ((rank + i) * 8 + file + i));
        }
    }

    return north_west_ray;
}

RayBitmasks::StraightRays RayBitmasks::getStraightRays(int ind) 
{
    return {
        getNorthRay(ind),
        getEastRay(ind),
        getSouthRay(ind),
        getWestRay(ind)
    };
}

RayBitmasks::DiagonalRays RayBitmasks::getDiagonalRays(int ind) 
{
    return {
        getNorthEastRay(ind),
        getSouthEastRay(ind),
        getSouthWestRay(ind),
        getNorthWestRay(ind)
    };
}
std::vector<RayBitmasks::StraightRays> RayBitmasks::getAllStraightRayBitmasks() 
{
    std::vector<RayBitmasks::StraightRays> allStraightRayBitmasks;

    for (int i = 0; i < 64; i++) {
        allStraightRayBitmasks.push_back(getStraightRays(i));
    }

    return allStraightRayBitmasks;
}

std::vector<RayBitmasks::DiagonalRays> RayBitmasks::getAllDiagonalRayBitmasks() 
{
    std::vector<RayBitmasks::DiagonalRays> allDiagonalRayBitmasks;

    for (int i = 0; i < 64; i++) {
        allDiagonalRayBitmasks.push_back(getDiagonalRays(i));
    }

    return allDiagonalRayBitmasks;
}

} // namespace logic
```


--------------------

# FILE: src/logic/movegen/bitmasks/RayBitmasks.h

```cpp
#pragma once

#include "common.h"

namespace logic {

struct RayBitmasks {

    struct StraightRays {
        bitmask north;
        bitmask east;
        bitmask south;
        bitmask west;
    };
    
    struct DiagonalRays {
        bitmask northEast;
        bitmask southEast;
        bitmask southWest;
        bitmask northWest;
    };

    static bitmask getNorthRay(int ind);
    static bitmask getNorthEastRay(int ind);
    static bitmask getEastRay(int ind);
    static bitmask getSouthEastRay(int ind);
    static bitmask getSouthRay(int ind);
    static bitmask getSouthWestRay(int ind);
    static bitmask getWestRay(int ind);
    static bitmask getNorthWestRay(int ind);
    
    static StraightRays getStraightRays(int ind);
    static DiagonalRays getDiagonalRays(int ind);
    
    static std::vector<StraightRays> getAllStraightRayBitmasks();
    static std::vector<DiagonalRays> getAllDiagonalRayBitmasks();
};

} // namespace logic
```


--------------------

# FILE: src/logic/eval/CMakeLists.txt

```
add_library(eval
    Evaluator.cpp
)

target_link_libraries(eval PUBLIC board cxx_setup)
```


--------------------

# FILE: src/logic/eval/Evaluator.cpp

```cpp
#include "logic/eval/Evaluator.h"

#include "model/board/Board.h"

namespace logic {

static const int PAWN_VALUE = 1000;
static const int KNIGHT_VALUE = 3000;
static const int BISHOP_VALUE = 3000;
static const int ROOK_VALUE = 5000;
static const int QUEEN_VALUE = 9000;

Evaluator::Evaluator(model::Board& board) 
    : _bitboards(board.bitboards) 
{}

float Evaluator::evaluate() {
    return 0.0f;
    // int score = 0;

    // score += PAWN_VALUE * (BitBasics::popCount(_board.getBitboardFromPieceType(model::Piece::Type::W_PAWN)));
    // score -= PAWN_VALUE * (BitBasics::popCount(_board.getBitboardFromPieceType(model::Piece::Type::B_PAWN)));

    // score += KNIGHT_VALUE * (BitBasics::popCount(_board.getBitboardFromPieceType(model::Piece::Type::W_KNIGHT)));
    // score -= KNIGHT_VALUE * (BitBasics::popCount(_board.getBitboardFromPieceType(model::Piece::Type::B_KNIGHT)));

    // score += BISHOP_VALUE * (BitBasics::popCount(_board.getBitboardFromPieceType(model::Piece::Type::W_BISHOP)));
    // score -= BISHOP_VALUE * (BitBasics::popCount(_board.getBitboardFromPieceType(model::Piece::Type::B_BISHOP)));

    // score += ROOK_VALUE * (BitBasics::popCount(_board.getBitboardFromPieceType(model::Piece::Type::W_ROOK)));
    // score -= ROOK_VALUE * (BitBasics::popCount(_board.getBitboardFromPieceType(model::Piece::Type::B_ROOK)));

    // score += QUEEN_VALUE * (BitBasics::popCount(_board.getBitboardFromPieceType(model::Piece::Type::W_QUEEN)));
    // score -= QUEEN_VALUE * (BitBasics::popCount(_board.getBitboardFromPieceType(model::Piece::Type::B_QUEEN)));

    // return static_cast<float>(score);
}

} // namespace logic
```


--------------------

# FILE: src/logic/eval/Evaluator.h

```cpp
#pragma once

namespace model {
    class Board;
    class Bitboards;
}

namespace logic {

class Evaluator {

public:
    Evaluator(model::Board& board);
    
    float evaluate();

private:
    model::Bitboards& _bitboards;
};

} // namespace logic
```


--------------------

# FILE: src/logic/makemove/CMakeLists.txt

```
add_library(makemove
    MoveMaker.cpp
    MoveRetractor.cpp
    MoveUtils.cpp
)
target_link_libraries(makemove PUBLIC board move pickmove cxx_setup)
```


--------------------

# FILE: src/logic/makemove/MoveMaker.cpp

```cpp
#include "logic/makemove/MoveMaker.h"

#include "logic/makemove/MoveUtils.h"

#include "model/board/Board.h"
#include "model/move/Move.h"

namespace logic {

MoveMaker::MoveMaker(model::Board& board)
    : _bitboards(board.bitboards) 
    , _stateBitmasks(board.stateBitmasks)
    , _pieceMap(board.pieceMap)
    , _zHasher(board.zHasher)
{}

MoveResult MoveMaker::makeMove(const model::Move& move, bool isWhite)
{
    auto moveResult = MoveResult();

    // If the move is a castle, update and return
    if (move.isAnyCastle()) {
        makeCastleMove(isWhite, move.isKingCastle());
        return moveResult;
    }

    // Get the from and to indices
    int fromIndex = move.getBitIndexFrom();
    int toIndex = move.getBitIndexTo();

    // Pick up the piece from the from square and get the moved piece type
    model::Piece::Type movedPieceType = removeMovedPieceFromBoard(isWhite, fromIndex);

    // If the move is a capture, handle memory and remove the captured piece
    if (move.isAnyCapture()) {
        // Calculate index of captured piece, might be EP
        int captureIndex = MoveUtils::determineCaptureIndex(move, isWhite, toIndex);
        model::Piece::Type capturedPieceType = _pieceMap.getPieceTypeAtIndex(captureIndex);
        removeCapturedPieceFromBoard(move.isEpCapture(), isWhite, captureIndex, capturedPieceType);
        moveResult.capturedPieceType = capturedPieceType;
    }

    // Update the moved piece type if the move is a promotion    
    if (move.isAnyPromo())
        movedPieceType = MoveUtils::getPromotionPieceType(move.getFlag(), isWhite);

    moveResult.movedPieceType = movedPieceType;

    // Place the moved piece on the to square
    placeMovedPieceOnBoard(isWhite, toIndex, movedPieceType);

    // Update occupied and empty squares bitmasks
    _stateBitmasks.updOccupiedAndEmptySquaresBitmasks();

    return moveResult;
}

void MoveMaker::makeCastleMove(bool isWhite, bool isKingSide)
{
    int fromKingInd, toKingInd, fromRookInd, toRookInd;

    if (isWhite) {
        fromKingInd = 3;
        toKingInd = isKingSide ? 1 : 5;
        fromRookInd = isKingSide ? 0 : 7;
        toRookInd = isKingSide ? 2 : 4;

        _bitboards.clearWhiteKingBit(fromKingInd);
        _bitboards.setWhiteKingBit(toKingInd);
        _bitboards.clearWhiteRooksBit(fromRookInd);
        _bitboards.setWhiteRooksBit(toRookInd);

        _stateBitmasks.clearWhitePiecesBit(fromKingInd);
        _stateBitmasks.setWhitePiecesBit(toKingInd);
        _stateBitmasks.clearWhitePiecesBit(fromRookInd);
        _stateBitmasks.setWhitePiecesBit(toRookInd);

        _pieceMap.setPieceTypeAtIndex(fromKingInd, model::Piece::Type::EMPTY);
        _pieceMap.setPieceTypeAtIndex(toKingInd, model::Piece::Type::W_KING);
        _pieceMap.setPieceTypeAtIndex(fromRookInd, model::Piece::Type::EMPTY);
        _pieceMap.setPieceTypeAtIndex(toRookInd, model::Piece::Type::W_ROOK);
    } else {
        fromKingInd = 59;
        toKingInd = isKingSide ? 57 : 61;
        fromRookInd = isKingSide ? 56 : 63;
        toRookInd = isKingSide ? 58 : 60;

        _bitboards.clearBlackKingBit(fromKingInd);
        _bitboards.setBlackKingBit(toKingInd);
        _bitboards.clearBlackRooksBit(fromRookInd);
        _bitboards.setBlackRooksBit(toRookInd);

        _stateBitmasks.clearBlackPiecesBit(fromKingInd);
        _stateBitmasks.setBlackPiecesBit(toKingInd);
        _stateBitmasks.clearBlackPiecesBit(fromRookInd);
        _stateBitmasks.setBlackPiecesBit(toRookInd);

        _pieceMap.setPieceTypeAtIndex(fromKingInd, model::Piece::Type::EMPTY);
        _pieceMap.setPieceTypeAtIndex(toKingInd, model::Piece::Type::B_KING);
        _pieceMap.setPieceTypeAtIndex(fromRookInd, model::Piece::Type::EMPTY);
        _pieceMap.setPieceTypeAtIndex(toRookInd, model::Piece::Type::B_ROOK);
    }

    _stateBitmasks.updOccupiedAndEmptySquaresBitmasks();
}

void MoveMaker::makeTemporaryKingMove(bool isWhite, bool isKingSide)
{
    int from = isWhite ? 3 : 59;

    int to = isKingSide ? (isWhite ? 2 : 58)
                        : (isWhite ? 4 : 60);

    if (isWhite) {
        _bitboards.clearWhiteKingBit(from);
        _bitboards.setWhiteKingBit(to);
    } else {
        _bitboards.clearBlackKingBit(from);
        _bitboards.setBlackKingBit(to);
    }
}


model::Piece::Type MoveMaker::removeMovedPieceFromBoard(bool isWhite, int fromIndex) 
{
    // Determine the piece type of the piece being moved
    model::Piece::Type  movedPieceType = _pieceMap.getPieceTypeAtIndex(fromIndex);

    // Update zobrist hash
    _zHasher.hashSquarePieceType(fromIndex, movedPieceType);

    // Clear the piece from bitboards, squarelookup and gamestate bitmasks
    _bitboards.clearPieceTypeBit(fromIndex, movedPieceType);
    _pieceMap.setPieceTypeAtIndex(fromIndex, model::Piece::Type::EMPTY);

    isWhite ? _stateBitmasks.clearWhitePiecesBit(fromIndex) 
            : _stateBitmasks.clearBlackPiecesBit(fromIndex);

    return movedPieceType;
}

void MoveMaker::placeMovedPieceOnBoard(
    bool isWhite, 
    int toIndex, 
    model::Piece::Type movedPieceType) 
{
    _bitboards.setPieceTypeBit(toIndex, movedPieceType);
    _pieceMap.setPieceTypeAtIndex(toIndex, movedPieceType);

    _zHasher.hashSquarePieceType(toIndex, movedPieceType);

    isWhite ? _stateBitmasks.setWhitePiecesBit(toIndex) 
            : _stateBitmasks.setBlackPiecesBit(toIndex);
}

void MoveMaker::removeCapturedPieceFromBoard(bool isEP, bool isWhite, int captureIndex, model::Piece::Type  capturedPieceType) {
    // Remove captured piece from board models
    _bitboards.clearPieceTypeBit(captureIndex, capturedPieceType);

    isWhite ? _stateBitmasks.clearBlackPiecesBit(captureIndex) 
            : _stateBitmasks.clearWhitePiecesBit(captureIndex);

    _zHasher.hashSquarePieceType(captureIndex, capturedPieceType);

    // Only clear from the squares lookup if the move was an ep capture
    // because the capture index points to the square where the pawn was
    // and is now empty, the square we moved to will have been updated
    if (isEP) {
        _pieceMap.setPieceTypeAtIndex(captureIndex, model::Piece::Type::EMPTY);
    }
}

} // namespace logic
```


--------------------

# FILE: src/logic/makemove/MoveMaker.h

```cpp
#pragma once

#include "logic/makemove/MoveUtils.h"

namespace model {
    class Board;
    class Move;
    class Bitboards;
    class StateBitmasks;
    class PieceMap;
    class ZHasher;
}

namespace logic {

class MoveMaker {

public:
    MoveMaker(model::Board& board);
    
    MoveResult makeMove(const model::Move& move, bool isWhite);
    
    void makeTemporaryKingMove(bool isWhite, bool isKingSide);

private:
    void makeCastleMove(bool isWhite, bool isKingSide);
    
    model::Piece::Type removeMovedPieceFromBoard(
        bool isWhite, 
        int fromIndex
    );

    void removeCapturedPieceFromBoard(
        bool isEP, 
        bool isWhite, 
        int captureIndex, 
        model::Piece::Type capturedPieceType
    );

    void placeMovedPieceOnBoard(
        bool isWhite, 
        int toIndex,
        model::Piece::Type movedPieceType
    );

    model::Bitboards& _bitboards;
    model::StateBitmasks& _stateBitmasks;
    model::PieceMap& _pieceMap;
    model::ZHasher& _zHasher;
};

} // namespace logic
```


--------------------

# FILE: src/logic/makemove/MoveRetractor.cpp

```cpp
#include "logic/makemove/MoveRetractor.h"

#include "model/board/Board.h"
#include "model/move/Move.h"

namespace logic {

MoveRetractor::MoveRetractor(
    model::Board& board
) : _bitboards(board.bitboards), 
    _stateBitmasks(board.stateBitmasks), 
    _pieceMap(board.pieceMap), 
    _zHasher(board.zHasher)
{}

void MoveRetractor::unmakeCastleMove(bool wasWhite, bool wasKingSide)
{
    int fromKingInd, toKingInd, fromRookInd, toRookInd;

    if (wasWhite) {
        fromKingInd = 3;
        toKingInd = wasKingSide ? 1 : 5;
        fromRookInd = wasKingSide ? 0 : 7;
        toRookInd = wasKingSide ? 2 : 4;
        
        _bitboards.clearWhiteKingBit(toKingInd);
        _bitboards.setWhiteKingBit(fromKingInd);
        _bitboards.clearWhiteRooksBit(toRookInd);
        _bitboards.setWhiteRooksBit(fromRookInd);

        _stateBitmasks.setWhitePiecesBit(fromKingInd);
        _stateBitmasks.clearWhitePiecesBit(toKingInd);
        _stateBitmasks.setWhitePiecesBit(fromRookInd);
        _stateBitmasks.clearWhitePiecesBit(toRookInd);

        _pieceMap.setPieceTypeAtIndex(fromKingInd, model::Piece::Type::W_KING);
        _pieceMap.setPieceTypeAtIndex(toKingInd, model::Piece::Type::EMPTY);
        _pieceMap.setPieceTypeAtIndex(fromRookInd, model::Piece::Type::W_ROOK);
        _pieceMap.setPieceTypeAtIndex(toRookInd, model::Piece::Type::EMPTY);
    } else {
        fromKingInd = 59;
        toKingInd = wasKingSide ? 57 : 61;
        fromRookInd = wasKingSide ? 56 : 63;
        toRookInd = wasKingSide ? 58 : 60;

        _bitboards.setBlackKingBit(fromKingInd);
        _bitboards.clearBlackKingBit(toKingInd);
        _bitboards.setBlackRooksBit(fromRookInd);
        _bitboards.clearBlackRooksBit(toRookInd);

        _stateBitmasks.setBlackPiecesBit(fromKingInd);
        _stateBitmasks.clearBlackPiecesBit(toKingInd);
        _stateBitmasks.setBlackPiecesBit(fromRookInd);
        _stateBitmasks.clearBlackPiecesBit(toRookInd);

        _pieceMap.setPieceTypeAtIndex(toKingInd, model::Piece::Type::EMPTY);
        _pieceMap.setPieceTypeAtIndex(fromKingInd, model::Piece::Type::B_KING);
        _pieceMap.setPieceTypeAtIndex(toRookInd, model::Piece::Type::EMPTY);
        _pieceMap.setPieceTypeAtIndex(fromRookInd, model::Piece::Type::B_ROOK);
    }

    _stateBitmasks.updOccupiedAndEmptySquaresBitmasks();
}

void MoveRetractor::unmakeTemporaryKingMove(bool wasWhite, bool isKingSide) 
{
    int from = isKingSide ? (wasWhite ? 2 : 58) 
                          : (wasWhite ? 4 : 60);

    int to = wasWhite ? 3 : 59;

    if (wasWhite) {
        _bitboards.clearWhiteKingBit(from);
        _bitboards.setWhiteKingBit(to);
    } else {
        _bitboards.clearBlackKingBit(from);
        _bitboards.setBlackKingBit(to);
    }
}

void MoveRetractor::removePreviouslyMovedPieceFromBoard(
    const model::Move& move,
    int toIndex,
    model::Piece::Type previouslyMovedPieceType,
    bool wasWhite) 
{
    // Square lookup is dependent on if there was a capture or promotion,
    // handled by the placeBackCapturedPieceOnBoard method 

    // If the move was not a promotion, remove the piece in the bitboard
    // Else, remove the bit for the promoted piece
    if (not move.isAnyPromo()) {
        _bitboards.clearPieceTypeBit(toIndex, previouslyMovedPieceType);
    } else {
        model::Piece::Type promotionPieceType = MoveUtils::getPromotionPieceType(move.getFlag(), wasWhite);
        _bitboards.clearPieceTypeBit(toIndex, promotionPieceType);
    }

    wasWhite ? _stateBitmasks.clearWhitePiecesBit(toIndex) 
             : _stateBitmasks.clearBlackPiecesBit(toIndex);
}


void MoveRetractor::placeBackCapturedPieceOnBoard(
    bool isEP,
    int captureIndex,
    int toIndex,
    bool wasWhite,
    model::Piece::Type previouslyCapturedPieceType) 
{
    _bitboards.setPieceTypeBit(captureIndex, previouslyCapturedPieceType);
    _pieceMap.setPieceTypeAtIndex(captureIndex, previouslyCapturedPieceType);

    // If the move was an ep capture, the to square will be empty
    if (isEP) {
        _pieceMap.setPieceTypeAtIndex(toIndex, model::Piece::Type::EMPTY);
    }

    wasWhite ? _stateBitmasks.setBlackPiecesBit(captureIndex) 
             : _stateBitmasks.setWhitePiecesBit(captureIndex);
}

void MoveRetractor::placeBackMovedPieceOnBoard(
    bool wasWhite, 
    int fromIndex, 
    model::Piece::Type  movedPieceType)
{
    _bitboards.setPieceTypeBit(fromIndex, movedPieceType);
    _pieceMap.setPieceTypeAtIndex(fromIndex, movedPieceType);

    wasWhite ? _stateBitmasks.setWhitePiecesBit(fromIndex) 
             : _stateBitmasks.setBlackPiecesBit(fromIndex);
}

model::Piece::Type MoveRetractor::determineMovedPieceType(
    const model::Move& move, 
    bool wasWhite,
    int toIndex) const
{
    // Piece type of piece being moved
    model::Piece::Type  previouslyMovedPieceType;

    // If the move was a promotion, set the moved piece to a pawn of the same color
    // Else, set the moved piece to the piece occupying the to square
    previouslyMovedPieceType = move.isAnyPromo() ? (wasWhite ? model::Piece::Type::W_PAWN
                                                             : model::Piece::Type::B_PAWN)
                                                 : _pieceMap.getPieceTypeAtIndex(toIndex);

    return previouslyMovedPieceType;
}

void MoveRetractor::unmakeMove(
    const model::Move& previousMove, 
    bool wasWhite, 
    MoveResult previousMoveResults) 
{
    // If the move is a castle, update the bitboards and return
    if (previousMove.isAnyCastle()) {
        unmakeCastleMove(wasWhite, previousMove.isKingCastle());
        return;
    }

    // Get the from and to indices
    // Things get a bit tricky here because the move is being unmade, and so
    // we are "moving to" the from square and "moving from" the to square
    int fromIndex = previousMove.getBitIndexFrom();
    int toIndex = previousMove.getBitIndexTo();
    assert(fromIndex != toIndex);
    

    // Determine the piece type of the piece that was previously moved,
    // takes into consideration if the move was a promotion
    model::Piece::Type  previouslyMovedPieceType = determineMovedPieceType(previousMove, wasWhite, toIndex);

    // We do the move in reverse, so now we pick up the previously moved piece
    removePreviouslyMovedPieceFromBoard(previousMove, toIndex, previouslyMovedPieceType, wasWhite);

    // We place back the captured piece if there was one
    if (previousMove.isAnyCapture()) {
       // Calculate the index of the previously captured piece, might be EP
        int captureIndex = MoveUtils::determineCaptureIndex(previousMove, wasWhite, toIndex);

        placeBackCapturedPieceOnBoard(previousMove.isEpCapture(), captureIndex, toIndex, wasWhite, previousMoveResults.capturedPieceType);
    } else {
        // If there was no capture, we place back an empty square on the to square
        _pieceMap.setPieceTypeAtIndex(toIndex, model::Piece::Type::EMPTY);
    }

    // Place the moved piece back on the from square
    placeBackMovedPieceOnBoard(wasWhite, fromIndex, previouslyMovedPieceType);

    _stateBitmasks.updOccupiedAndEmptySquaresBitmasks();
}

} // namespace logic
```


--------------------

# FILE: src/logic/makemove/MoveRetractor.h

```cpp
#pragma once

#include "logic/makemove/MoveUtils.h"

namespace model {
    class Board;
    class Bitboards;
    class StateBitmasks;
    class PieceMap;
    class ZHasher;
}

namespace logic {

class MoveRetractor {

public:
    // Constructor
    MoveRetractor(
        model::Board& board
    );
    
    void unmakeMove(
        const model::Move& move, 
        bool wasWhite, 
        MoveResult previousMoveResults
    );

    void unmakeTemporaryKingMove(bool wasWhite, bool isKingSide); 

private:
    void unmakeCastleMove(bool wasWhite, bool isKingSide);

    model::Piece::Type  determineMovedPieceType(const model::Move& move, bool wasWhite, int toIndex) const;
    
    void removePreviouslyMovedPieceFromBoard(const model::Move& move, int toIndex, model::Piece::Type  previouslyMovedPieceType, bool wasWhite);
    void placeBackCapturedPieceOnBoard(bool isEP, int captureIndex, int toIndex, bool wasWhite, model::Piece::Type  previouslyCapturedPieceType);
    void placeBackMovedPieceOnBoard(bool wasWhite, int fromIndex, model::Piece::Type  movedPieceType);

    model::Bitboards& _bitboards;
    model::StateBitmasks& _stateBitmasks;
    model::PieceMap& _pieceMap;
    const model::ZHasher& _zHasher;
};

} // namespace logic
```


--------------------

# FILE: src/logic/makemove/MoveUtils.cpp

```cpp
#include "logic/makemove/MoveUtils.h"

#include "model/move/Move.h"

namespace logic {

model::Piece::Type MoveUtils::getPromotionPieceType(int promotionFlag, bool isWhite) 
{
    switch(promotionFlag) {
        case model::Move::KNIGHT_PROMO_FLAG:
            return isWhite ?model::Piece::Type::W_KNIGHT :model::Piece::Type::B_KNIGHT;
            break;
        case model::Move::BISHOP_PROMO_FLAG:
            return isWhite ?model::Piece::Type::W_BISHOP :model::Piece::Type::B_BISHOP;
            break;
        case model::Move::ROOK_PROMO_FLAG:
            return isWhite ?model::Piece::Type::W_ROOK :model::Piece::Type::B_ROOK;
            break;
        case model::Move::QUEEN_PROMO_FLAG:
            return isWhite ?model::Piece::Type::W_QUEEN :model::Piece::Type::B_QUEEN;
            break;
        case model::Move::KNIGHT_PROMO_CAPTURE_FLAG:
            return isWhite ?model::Piece::Type::W_KNIGHT :model::Piece::Type::B_KNIGHT;
            break;
        case model::Move::BISHOP_PROMO_CAPTURE_FLAG:
            return isWhite ?model::Piece::Type::W_BISHOP :model::Piece::Type::B_BISHOP;
            break;
        case model::Move::ROOK_PROMO_CAPTURE_FLAG:
            return isWhite ?model::Piece::Type::W_ROOK :model::Piece::Type::B_ROOK;
            break;
        case model::Move::QUEEN_PROMO_CAPTURE_FLAG:
            return isWhite ?model::Piece::Type::W_QUEEN :model::Piece::Type::B_QUEEN;
            break;
    }

    return model::Piece::Type::EMPTY;
}

int MoveUtils::determineCaptureIndex(const model::Move& move, bool isWhite, int toIndex)
{
    return move.isEpCapture() ? isWhite ? toIndex - 8 
                                        : toIndex + 8 
                              : toIndex;
}
    
} // namespace logic
```


--------------------

# FILE: src/logic/makemove/MoveUtils.h

```cpp
#pragma once

#include "model/board/PieceType.h"

namespace model {
    class Move;
}

namespace logic {

struct MoveResult {
    model::Piece::Type capturedPieceType;
    model::Piece::Type movedPieceType;

    MoveResult() {
        capturedPieceType = model::Piece::Type::EMPTY;
        movedPieceType = model::Piece::Type::EMPTY;
    }
};

struct MoveUtils {
    
    static model::Piece::Type getPromotionPieceType(int promotionFlag, bool isWhite);
    static int determineCaptureIndex(const model::Move& move, bool isWhite, int toIndex);

};


} // namespace logic
```


--------------------

# FILE: src/engine/CMakeLists.txt

```
add_subdirectory(pickmove)
add_subdirectory(perft)
```


--------------------

# FILE: src/engine/pickmove/CMakeLists.txt

```
add_library(pickmove
    MovePicker.cpp
    SearchMemory.cpp
)
target_link_libraries(pickmove PUBLIC io eval makemove movegen cxx_setup)
```


--------------------

# FILE: src/engine/pickmove/MovePicker.cpp

```cpp
#include "engine/pickmove/MovePicker.h"

#include "io/BoardPrinter.h"

#include <cstdlib>
#include <iostream>

namespace engine {

MovePicker::MovePicker(int maxDepth) 
    : _board()
    , _bitboards(_board.bitboards)
    , _pieceMap(_board.pieceMap)
    , _stateBitmasks(_board.stateBitmasks)
    , _zHasher(_board.zHasher)
    , _searchMemory(SearchMemory(maxDepth))
    , _moveMaker(logic::MoveMaker(_board))
    , _moveRetractor(logic::MoveRetractor(_board))
    , _moveGenerator(logic::MoveGenerator(_board, _moveMaker, _moveRetractor))
    , _evaluator(logic::Evaluator(_board))
    , _maxDepth(maxDepth)
{
    _numMoveGenCalls = 0;
    _totalNodes = 0;
    
    _nodeCountPerFirstMove.resize(MAX_LEGAL_MOVES);
    _firstMoves.resize(MAX_LEGAL_MOVES);

    for (int i = 0; i < MAX_LEGAL_MOVES; i++) {
        _nodeCountPerFirstMove[i] = 0;
        _firstMoves[i] = model::Move();
    }

    _lastCapturedPieces.resize(_maxDepth);
    _movelists.resize(_maxDepth);
    _noCapturedOrPawnMoveCounts.resize(_maxDepth);

    for (int i = 0; i < _maxDepth; i++) {
        _lastCapturedPieces[i] = model::Piece::Type::EMPTY;
        _movelists[i] = model::Movelist();
        _noCapturedOrPawnMoveCounts[i] = 0;
    }

    _nodeCount.resize(20);
    _captureCount.resize(20);
    _epCaptureCount.resize(20);
    _castlingCount.resize(20);
    _promotionCount.resize(20);
    _checkCount.resize(20);
    _checkmateCount.resize(20);

    for (int i = 0; i < 20; i++) {
        _nodeCount[i] = 0;
        _captureCount[i] = 0;
        _epCaptureCount[i] = 0;
        _castlingCount[i] = 0;
        _promotionCount[i] = 0;
        _checkCount[i] = 0;
        _checkmateCount[i] = 0;
    }
}

long MovePicker::sumNodesToDepth(int depth) const {
    long sum = 0;

    for (long i = 1; i <= depth; i++) {
        sum += _nodeCount[i];
    }

    return sum;
}

void MovePicker::genMoves(
    bool isWhite,
    int currentDepth,
    bitmask enpessantTarget,
    unsigned char castlingRights) 
{
    _moveGenerator.genMoves(isWhite, _movelists[currentDepth], enpessantTarget, castlingRights);
}

logic::MoveResult MovePicker::makeMove(model::Move move, bool isWhite) 
{
    return _moveMaker.makeMove(move, isWhite);
}

void MovePicker::unmakeMove(
    model::Move move,
    bool isWhite,
    logic::MoveResult previousMoveResult)
{
    _moveRetractor.unmakeMove(move, isWhite, previousMoveResult);
}

void MovePicker::debugPrint(bool verbose) const
{
    if (verbose) {
        io::BoardPrinter boardPrinter = io::BoardPrinter(_bitboards);
        boardPrinter.printBoard();
    }
}

// Helper function to check if there are any castling moves in the movelist
// bool hasTwoCastlingMove(MoveList& movelist) {
//     int count = 0;

//     for (size_t i = 0; i < movelist.numMoves; i++) {
//         if (movelist.moves[i].isAnyCastle()) {
//             count++;
//         }
//     }

//     return count == 2;
// }

// bool noKingSideCastling(MoveList& movelist) {
//     for (size_t i = 0; i < movelist.numMoves; i++) {
//         if (movelist.moves[i].getFlag() == 3) {
//             return false;
//         }
//     }

//     return true;
// }

// bool noQueenSideCastling(MoveList& movelist) {
//     for (size_t i = 0; i < movelist.numMoves; i++) {
//         if (movelist.moves[i].getFlag() == 2) {
//             return false;
//         }
//     }

//     return true;
// }

bool MovePicker::tooManyPiecesOnBoard() 
{
    int count = 0;
    for (int i = 0; i < 64; i++) {
        if (_pieceMap.getPieceTypeAtIndex(i) != model::Piece::Type::EMPTY) {
            count++;
        }
    }

    return count > 32;
}

bool MovePicker::checkCondition(
    int currentDepth,
    bool isMaximizer,
    int firstMoveIndex, 
    model::Move currentMove, 
    model::Move lastMove, 
    bool verbose, 
    size_t i) const
{
    // return not _board.getKingMoved(false);
    // return tooManyPiecesOnBoard();
    // return firstMoveIndex == 19 && currentMove.isAnyCapture();
    // return currentMove.getBitIndexFrom() == 12 && currentMove.getBitIndexTo() == 12;
    // return currentDepth == 3 && firstMoveIndex == 0 && currentMove.getBitIndexFrom() == 34 && currentMove.getBitIndexTo() == 27;
    // return currentMove.isAnyCapture();
    // return true;
    return false;
    // return diffBetweenStateBitmasks();
    // return currentDepth == 2 && firstMoveIndex == 0 && isMaximizer == true && currentMove.getMove() == 66;
}

// TODO: Implement draw by repetition after implementing zobrist hashing
void MovePicker::minimax(
    int currentDepth, 
    bool isMaximizer, 
    int firstMoveIndex, 
    bool recPerftStats, 
    const model::Move& lastMove, 
    bool verbose)
{        
    if (currentDepth == _maxDepth) {
        return;
    }

    genMoves(
        isMaximizer, 
        currentDepth, 
        _searchMemory.getEnPessantTargetAtDepth(currentDepth),
        _searchMemory.getCastlingRightsAtDepth(currentDepth)
    );

    _numMoveGenCalls++;
    
    size_t numIllegalMoves = 0;

    for (size_t i = 0; i < MAX_LEGAL_MOVES; i++) {
        model::Move currentMove = _movelists[currentDepth].getMoveAt(i);

        if (currentMove.getMove() == 0) {
            break;
        }

        if (checkCondition(
            currentDepth, 
            isMaximizer, 
            firstMoveIndex, 
            currentMove, 
            lastMove, 
            verbose, 
            i)) 
        {
            debugPrint(verbose);
            int x = 4;
        }

        // Make the move and check if we are in any way left in check
        logic::MoveResult moveResult = makeMove(currentMove, isMaximizer);

        if (checkCondition(
            currentDepth, 
            isMaximizer, 
            firstMoveIndex, 
            currentMove, 
            lastMove, 
            verbose, 
            i)) 
        {
            debugPrint(verbose);
            int x = 4;
        }

        // FIXME: Move generator should not be queried for this
        if (_moveGenerator.isInCheck(isMaximizer)) {
            numIllegalMoves++;
            unmakeMove(currentMove, isMaximizer, moveResult);

            if (checkCondition(
                currentDepth, 
                isMaximizer, 
                firstMoveIndex, 
                currentMove, 
                lastMove, 
                verbose, 
                i)) 
            {
                debugPrint(verbose);
                int x = 4;
            }

            if (numIllegalMoves == i + 1 && _movelists[currentDepth].getMoveAt(i + 1).getMove() == 0) {
                bool wasInCheckBeforeMove = _moveGenerator.isInCheck(isMaximizer);

                if (wasInCheckBeforeMove) {
                    _checkmateCount[currentDepth]++;
                }

                return;
            }

            continue;
        }

        if (currentMove.isAnyCapture()) {
            _searchMemory.setLastCapturedPieceAtDepth(currentDepth, moveResult.capturedPieceType);
        }

        _searchMemory.handleEnPessantMemory(currentMove, isMaximizer, currentDepth, currentMove.getBitIndexTo());
        _searchMemory.handleNoCaptureCount(currentMove, currentDepth, moveResult.movedPieceType);

        // Move was legal, update castling rights
        _searchMemory.setCastlingRights(
            currentDepth,
            currentMove, 
            isMaximizer, 
            _pieceMap.getPieceTypeAtIndex(currentMove.getBitIndexTo())
        );

        if (recPerftStats) {
            bool retFlag;
            
            recordPerftStats(
                isMaximizer, 
                currentDepth, 
                firstMoveIndex, 
                i, 
                currentMove, 
                retFlag
            );
            
            if (retFlag)
                return;
        }

        minimax(
            currentDepth + 1, 
            !isMaximizer, 
            firstMoveIndex, 
            recPerftStats, 
            currentMove, 
            verbose
        );

        unmakeMove(currentMove, isMaximizer, moveResult);
        _searchMemory.unsetCastlingRights(currentDepth);

        if (currentMove.isDoublePawnPush()) {
            _searchMemory.setEnPessantTargetAtDepth(currentDepth + 1, 0ULL);
        }
    
        if (not currentMove.isAnyCapture() && (moveResult.capturedPieceType != model::Piece::Type::W_PAWN && moveResult.movedPieceType != model::Piece::Type::B_PAWN)) {
            _searchMemory.decrementNoCapturedOrPawnMoveCountAtDepth(currentDepth + 1);
        }

        if (checkCondition(
            currentDepth, 
            isMaximizer, 
            firstMoveIndex, 
            currentMove, 
            lastMove, 
            verbose, 
            i)) 
        {
            debugPrint(verbose);
            int x = 4;
        }
    }

    return;
}

void MovePicker::recordPerftStats(
    bool isMaximizer, 
    int currentDepth, 
    int &firstMoveIndex, 
    size_t i, 
    const model::Move& currentMove, 
    bool &retFlag) 
{
    retFlag = true;
    if (_moveGenerator.isInCheck(!isMaximizer))
    {
        _checkCount[currentDepth + 1]++;
    }

    if (currentDepth == 0) {
        firstMoveIndex = i;
        _firstMoves[i] = currentMove;
    }

    else if (currentDepth == _maxDepth - 1) {
        _nodeCountPerFirstMove[firstMoveIndex]++;
    }

    _nodeCount[currentDepth + 1]++;

    if (currentMove.isAnyCapture()) {
        _captureCount[currentDepth + 1]++;
    }

    if (currentMove.isAnyPromo()) {
        _promotionCount[currentDepth + 1]++;
    }

    if (currentMove.isAnyCastle()) {
        _castlingCount[currentDepth + 1]++;
    }

    if (currentMove.isEpCapture()) {
        _epCaptureCount[currentDepth + 1]++;
    }

    // FIXME: This is temporary
    // if (_board.isDeadPosition() || 49 >= 50)
    // {
    //     unmakeMove(currentMove, isMaximizer, currentDepth);
    //     return;
    // }
    retFlag = false;
}

} // namespace engine
```


--------------------

# FILE: src/engine/pickmove/MovePicker.h

```cpp
#pragma once

#include "engine/pickmove/SearchMemory.h"

#include "logic/eval/Evaluator.h"

#include "model/board/Board.h"

#include "model/board/PieceMap.h"
#include "logic/movegen/MoveGenerator.h"
#include "model/move/Movelist.h"
#include "logic/makemove/MoveMaker.h"
#include "logic/makemove/MoveRetractor.h"
#include "logic/makemove/MoveUtils.h"
#include "model/move/Move.h"
#include "io/Fen.h"

namespace engine {

class MovePicker {

public:
    MovePicker(int maxDepth);
    
    void minimax(
        int current_depth,
        bool isMaximizer, 
        int firstMoveIndex,
        bool recPerftStats = true,
        const model::Move& lastMove = model::Move(),
        bool verbose = true
    );

    void recordPerftStats(
        bool isMaximizer,
        int currentDepth,
        int &firstMoveIndex,
        size_t i,
        const model::Move& currentMove,
        bool &retFlag
    );

    void genMoves(
        bool isWhite,
        int currentDepth,
        bitmask enpessantTarget, 
        unsigned char castlingRights
    );

    logic::MoveResult makeMove(model::Move move, bool isWhite);
    void unmakeMove(model::Move move, bool isWhite, logic::MoveResult prevousMoveResult);
    void undoMove();
    
    int _numMoveGenCalls;
    int _totalNodes;

    long sumNodesToDepth(int depth) const;
    std::vector<long> _nodeCountPerFirstMove;
    std::vector<model::Move> _firstMoves;
    std::vector<long> _nodeCount;
    std::vector<long> _captureCount;
    std::vector<long> _epCaptureCount;
    std::vector<long> _castlingCount;
    std::vector<long> _promotionCount;
    std::vector<long> _checkCount;
    std::vector<long> _checkmateCount;

    void debugPrint(bool verbose) const;
    
    bool checkCondition(
        int currentDepth,
        bool isMaximizer, 
        int firstMoveIndex, 
        model::Move currentMove, 
        model::Move lastMove, 
        bool verbose, 
        size_t i
    ) const;

    void setMaxDepth(int maxDepth) 
    {
        _maxDepth = maxDepth;
    }

    int getMaxDepth() const {
        return _maxDepth;
    }

    const logic::MoveGenerator& getMoveGenerator() const
    {
        return _moveGenerator;
    }

    void setBoardFromFen(const std::string& fen)
    {
        io::Fen::setBoardFromFen(fen, _bitboards, _stateBitmasks, _pieceMap);
    }

    std::string getFenFromBoard() const
    {
        return io::Fen::getFenFromBoard(_pieceMap);
    }

    bool diffBetweenStateBitmasks() const
    {
        return (_stateBitmasks.getBlackPiecesBitmask() | _stateBitmasks.getWhitePiecesBitmask()) != _stateBitmasks.getOccupiedPiecesBitmask();
    }
    
private:
    model::Board _board;
    model::Bitboards& _bitboards;
    model::PieceMap& _pieceMap;
    model::StateBitmasks& _stateBitmasks;
    model::ZHasher& _zHasher;
    
    SearchMemory _searchMemory;
    logic::MoveMaker _moveMaker;
    logic::MoveRetractor _moveRetractor;
    logic::MoveGenerator _moveGenerator;
    logic::Evaluator _evaluator;
    int _maxDepth;

    int _pseudoLegalMovesCount;
    std::vector<model::Movelist> _movelists;
    std::vector<model::Piece::Type> _lastCapturedPieces;
    std::vector<int> _noCapturedOrPawnMoveCounts; 

    bool tooManyPiecesOnBoard();
};

} // namespace engine
```


--------------------

# FILE: src/engine/pickmove/SearchMemory.cpp

```cpp
#include "engine/pickmove/SearchMemory.h"

#include "model/board/PieceType.h"

namespace engine {
    
SearchMemory::SearchMemory(int maxDepth) : _maxDepth(maxDepth) 
{
    _castlingRights.resize(_maxDepth + 1);
    _lastCapturedPieces.resize(_maxDepth + 1);
    _enPessantTargets.resize(_maxDepth + 1);
    _noCapturedOrPawnMoveCounts.resize(_maxDepth + 1);

    for (int i = 0; i <= _maxDepth; i++) {
        _castlingRights[i] = 0b1111;
        _lastCapturedPieces[i] = model::Piece::Type::EMPTY;
        _enPessantTargets[i] = 0ULL;
        _noCapturedOrPawnMoveCounts[i] = 0;
    }
}

void SearchMemory::removeCastlingRightsForRemainingDepths(
    int currentDepth,
    unsigned char rightsToRemove) 
{
    for (int i = currentDepth + 1; i < _maxDepth; i++) {
        _castlingRights[i] &= ~rightsToRemove;
    }
}

void SearchMemory::restoreCastlingRightsForRemainingDepths(int currentDepth) 
{
    for (int i = currentDepth + 1; i < _maxDepth; i++) {
        _castlingRights[i] = _castlingRights[currentDepth];
    }
}

void SearchMemory::setCastlingRights(
    int currentDepth, 
    const model::Move& move, 
    bool isWhite, 
    model::Piece::Type movedPieceType) 
{
    if (move.isAnyCastle()) {
        removeCastlingRightsForRemainingDepths(
            currentDepth, 
            isWhite ? whiteBoth : blackBoth
        );
    }

    if (movedPieceType == model::Piece::Type::W_KING || movedPieceType == model::Piece::Type::B_KING) {
        if (isWhite) {
            if (_castlingRights[currentDepth] & whiteBoth)
                removeCastlingRightsForRemainingDepths(currentDepth, whiteBoth);
        } else {
            if (_castlingRights[currentDepth] & blackBoth)
                removeCastlingRightsForRemainingDepths(currentDepth, blackBoth);
        }
    }

    if (movedPieceType == model::Piece::Type::W_ROOK || movedPieceType == model::Piece::Type::B_ROOK) {
        if (isWhite) {
            if (move.getBitIndexFrom() == 0) {
                if (_castlingRights[currentDepth] & whiteKingSide)
                    removeCastlingRightsForRemainingDepths(currentDepth, whiteKingSide);
            } else if (move.getBitIndexFrom() == 7) {
                if (_castlingRights[currentDepth] & whiteQueenSide)
                    removeCastlingRightsForRemainingDepths(currentDepth, whiteQueenSide);
            }
        } else {
            if (move.getBitIndexFrom() == 56) {
                if (_castlingRights[currentDepth] & blackKingSide)
                    removeCastlingRightsForRemainingDepths(currentDepth, blackKingSide);
            } else if (move.getBitIndexFrom() == 63) {
                if (_castlingRights[currentDepth] & blackQueenSide)
                    removeCastlingRightsForRemainingDepths(currentDepth, blackQueenSide);
            }
        }
    }
}

void SearchMemory::unsetCastlingRights(int currentDepth) 
{
    if (_castlingRights[currentDepth] != _castlingRights[currentDepth + 1]) {
        restoreCastlingRightsForRemainingDepths(currentDepth);
    }
}

void SearchMemory::overrideCastlingRights(unsigned char rights) 
{
    for (int i = 0; i < _maxDepth; i++) {
        _castlingRights[i] = rights;
    }
}

void SearchMemory::handleNoCaptureCount(
    const model::Move& move, 
    int currentDepth, 
    model::Piece::Type  movedPieceType)
{
    // If the move is a capture, reset the no capture count
    if (move.isAnyCapture()) {
        resetNoCapturedOrPawnMoveCountAtDepth(currentDepth + 1);
        return;
    }

    // If the move is a pawn move, reset the no capture count
    if (movedPieceType == model::Piece::Type::W_PAWN || movedPieceType == model::Piece::Type::B_PAWN) {
        resetNoCapturedOrPawnMoveCountAtDepth(currentDepth + 1);
        return;
    }

    // If the move is not a capture or pawn move, increment the no capture count
    incrementNoCapturedOrPawnMoveCountAtDepth(currentDepth + 1);
}

void SearchMemory::handleEnPessantMemory(
    const model::Move& move, 
    bool isWhite, 
    int currentDepth, 
    int toIndex) 
{
    if (not move.isDoublePawnPush()) {
        setEnPessantTargetAtDepth(currentDepth + 1, 0ULL);
        return;
    }

    if (move.isDoublePawnPush()) {
        bitmask enPessantTarget = isWhite ? (1ULL << (toIndex - 8)) 
                                          : (1ULL << (toIndex + 8));

        setEnPessantTargetAtDepth(currentDepth + 1, enPessantTarget);
        
        // FIXME: Temporary because I don't know how to implement this haha
        // _zHasher.hashEnPassantFile(toIndex % 8);
    }
}

} // namespace engine
```


--------------------

# FILE: src/engine/pickmove/SearchMemory.h

```cpp
#pragma once

#include "model/move/Move.h"
#include "model/board/PieceType.h"

namespace {
    constexpr unsigned char whiteKingSide = 0b0001;
    constexpr unsigned char whiteQueenSide = 0b0010;
    constexpr unsigned char whiteBoth = 0b0011;
    constexpr unsigned char blackKingSide = 0b0100;
    constexpr unsigned char blackQueenSide = 0b1000;
    constexpr unsigned char blackBoth = 0b1100;
} // namespace

namespace engine {

class SearchMemory {

public:
    SearchMemory(int maxDepth);

    unsigned char getCastlingRightsAtDepth(int depth) const
    {
        return _castlingRights[depth];
    }

    model::Piece::Type getLastCapturedPieceAtDepth(int depth) const
    {
        return _lastCapturedPieces[depth];
    }

    void setLastCapturedPieceAtDepth(
        int currentDepth,
        model::Piece::Type pieceType) 
    {
        _lastCapturedPieces[currentDepth] = pieceType;
    }

    void setCastlingRights(
        int currentDepth, 
        const model::Move& move, 
        bool isWhite, 
        model::Piece::Type movedPieceType
    );

    void unsetCastlingRights(int currentDepth);

    bitmask getEnPessantTargetAtDepth(int depth) const
    {
        return _enPessantTargets[depth];
    }

    void setEnPessantTargetAtDepth(int depth, bitmask target) 
    {
        _enPessantTargets[depth] = target;
    }

    int getNoCapturedOrPawnMoveCountAtDepth(int depth) const
    {
        return _noCapturedOrPawnMoveCounts[depth];
    }

    void incrementNoCapturedOrPawnMoveCountAtDepth(int depth) 
    {
        _noCapturedOrPawnMoveCounts[depth]++;
    }

    void decrementNoCapturedOrPawnMoveCountAtDepth(int depth) 
    {
        _noCapturedOrPawnMoveCounts[depth]--;
    }

    void resetNoCapturedOrPawnMoveCountAtDepth(int depth) 
    {
        _noCapturedOrPawnMoveCounts[depth] = 0;
    }

    void handleNoCaptureCount(
        const model::Move& move, 
        int currentDepth, 
        model::Piece::Type  movedPieceType
    );

    void handleEnPessantMemory(
            const model::Move& move, 
            bool isWhite, 
            int currentDepth, 
            int toIndex
    );

private:
    int _maxDepth;
    std::vector<unsigned char> _castlingRights;
    std::vector<model::Piece::Type> _lastCapturedPieces;
    std::vector<bitmask> _enPessantTargets;
    std::vector<int> _noCapturedOrPawnMoveCounts;

    void removeCastlingRightsForRemainingDepths(
        int currentDepth, 
        unsigned char rightsToRemove
    );
    
    void restoreCastlingRightsForRemainingDepths(int currentDepth);
    void overrideCastlingRights(unsigned char rights);

};

} // namespace engine
```


--------------------

# FILE: src/engine/perft/CMakeLists.txt

```
add_library(perft
    perft.cpp
    perftData.cpp
)
target_link_libraries(perft PUBLIC movegen eval io makemove cxx_setup)
```


--------------------

# FILE: src/engine/perft/perft.cpp

```cpp
#include "engine/perft/perft.h"

#include "io/BoardPrinter.h"

#include <cstdlib>
#include <iostream>

namespace engine {

perft::perft(int maxDepth)
    : _board(),
      _bitboards(_board.bitboards),
      _pieceMap(_board.pieceMap),
      _stateBitmasks(_board.stateBitmasks),
      _zHasher(_board.zHasher),
      _searchMemory(maxDepth),
      _moveMaker(_board),
      _moveRetractor(_board),
      _moveGenerator(_board, _moveMaker, _moveRetractor),
      _evaluator(_board),
      _perftData(maxDepth),
      _numMoveGenCalls(0),
      _totalNodes(0)
{
    // For any non-uniform initialization:
    _lastCapturedPieces.resize(_maxDepth, model::Piece::Type::EMPTY);
    _movelists.resize(_maxDepth);
    _noCapturedOrPawnMoveCounts.resize(_maxDepth, 0);
}


void perft::genMoves(
    bool isWhite,
    int currentDepth,
    bitmask enpessantTarget,
    unsigned char castlingRights) 
{
    _moveGenerator.genMoves(isWhite, _movelists[currentDepth], enpessantTarget, castlingRights);
}

logic::MoveResult perft::makeMove(
    model::Move move,
    bool isWhite) 
{
    return _moveMaker.makeMove(move, isWhite);
}

void perft::unmakeMove(
    model::Move move,
    bool isWhite,
    logic::MoveResult previousMoveResult)
{
    _moveRetractor.unmakeMove(move, isWhite, previousMoveResult);
}

void perft::debugPrint(bool verbose) const
{
    if (verbose) {
        io::BoardPrinter boardPrinter = io::BoardPrinter(_bitboards);
        boardPrinter.printBoard();
    }
}

// Helper function to check if there are any castling moves in the movelist
// bool hasTwoCastlingMove(MoveList& movelist) {
//     int count = 0;

//     for (size_t i = 0; i < movelist.numMoves; i++) {
//         if (movelist.moves[i].isAnyCastle()) {
//             count++;
//         }
//     }

//     return count == 2;
// }

// bool noKingSideCastling(MoveList& movelist) {
//     for (size_t i = 0; i < movelist.numMoves; i++) {
//         if (movelist.moves[i].getFlag() == 3) {
//             return false;
//         }
//     }

//     return true;
// }

// bool noQueenSideCastling(MoveList& movelist) {
//     for (size_t i = 0; i < movelist.numMoves; i++) {
//         if (movelist.moves[i].getFlag() == 2) {
//             return false;
//         }
//     }

//     return true;
// }

bool perft::tooManyPiecesOnBoard() 
{
    int count = 0;
    for (int i = 0; i < 64; i++) {
        if (_pieceMap.getPieceTypeAtIndex(i) != model::Piece::Type::EMPTY) {
            count++;
        }
    }

    return count > 32;
}

bool perft::checkCondition(
    int currentDepth,
    bool isMaximizer,
    int firstMoveIndex, 
    model::Move currentMove, 
    model::Move lastMove, 
    bool verbose, 
    size_t i) const
{
    // return not _board.getKingMoved(false);
    // return tooManyPiecesOnBoard();
    // return firstMoveIndex == 19 && currentMove.isAnyCapture();
    // return currentMove.getBitIndexFrom() == 12 && currentMove.getBitIndexTo() == 12;
    // return currentDepth == 3 && firstMoveIndex == 0 && currentMove.getBitIndexFrom() == 34 && currentMove.getBitIndexTo() == 27;
    // return currentMove.isAnyCapture();
    // return true;
    return false;
    // return diffBetweenStateBitmasks();
    // return currentDepth == 2 && firstMoveIndex == 0 && isMaximizer == true && currentMove.getMove() == 66;
}

// TODO: Implement draw by repetition after implementing zobrist hashing
void perft::minimax(
    int currentDepth, 
    bool isMaximizer, 
    int firstMoveIndex, 
    bool recPerftStats, 
    const model::Move& lastMove, 
    bool verbose)
{        
    if (currentDepth == _maxDepth)
        return;

    genMoves(
        isMaximizer, 
        currentDepth,
        _searchMemory.getEnPessantTargetAtDepth(currentDepth),
        _searchMemory.getCastlingRightsAtDepth(currentDepth)
    );

    _numMoveGenCalls++;
    
    size_t numIllegalMoves = 0;

    for (size_t i = 0; i < MAX_LEGAL_MOVES; i++) {
        model::Move currentMove = _movelists[currentDepth].getMoveAt(i);

        if (currentMove.getMove() == 0)
            break;

        if (checkCondition(
            currentDepth, 
            isMaximizer, 
            firstMoveIndex, 
            currentMove, 
            lastMove, 
            verbose, 
            i)) 
        {
            debugPrint(verbose);
            int x = 4;
        }

        // Make the move and check if we are in any way left in check
        logic::MoveResult moveResult = makeMove(currentMove, isMaximizer);

        if (checkCondition(
            currentDepth, 
            isMaximizer, 
            firstMoveIndex, 
            currentMove, 
            lastMove, 
            verbose, 
            i)) 
        {
            debugPrint(verbose);
            int x = 4;
        }

        if (_moveGenerator.isInCheck(isMaximizer)) {
            numIllegalMoves++;
            unmakeMove(currentMove, isMaximizer, moveResult);

            if (checkCondition(
                currentDepth, 
                isMaximizer, 
                firstMoveIndex, 
                currentMove, 
                lastMove, 
                verbose, 
                i)) 
            {
                debugPrint(verbose);
                int x = 4;
            }

            if (numIllegalMoves == i + 1 && _movelists[currentDepth].getMoveAt(i + 1).getMove() == 0) {
                bool wasInCheckBeforeMove = _moveGenerator.isInCheck(isMaximizer);

                if (wasInCheckBeforeMove) {
                    _perftData.increaseCheckmateCountAt(currentDepth);
                }

                return;
            }

            continue;
        }

        // Move was legal, update castling rights
        _searchMemory.setCastlingRights(
            currentDepth,
            currentMove, 
            isMaximizer, 
            _pieceMap.getPieceTypeAtIndex(currentMove.getBitIndexTo())
        );

        if (recPerftStats) {
            bool retFlag;
            
            recordPerftStats(
                isMaximizer, 
                currentDepth, 
                firstMoveIndex, 
                i, 
                currentMove, 
                retFlag
            );
            
            if (retFlag)
                return;
        }

        minimax(
            currentDepth + 1, 
            !isMaximizer, 
            firstMoveIndex, 
            recPerftStats, 
            currentMove, 
            verbose
        );

        unmakeMove(currentMove, isMaximizer, moveResult);
        _searchMemory.unsetCastlingRights(currentDepth);
        
        if (checkCondition(
            currentDepth, 
            isMaximizer, 
            firstMoveIndex, 
            currentMove, 
            lastMove, 
            verbose, 
            i)) 
        {
            debugPrint(verbose);
            int x = 4;
        }
    }

    return;
}

void perft::recordPerftStats(
    bool isMaximizer, 
    int currentDepth, 
    int &firstMoveIndex, 
    size_t i, 
    const model::Move& currentMove, 
    bool &retFlag) 
{
    retFlag = true;
    if (_moveGenerator.isInCheck(!isMaximizer)) {
        _perftData.increaseCheckCountAt(currentDepth + 1);
    }

    if (currentDepth == 0) {
        firstMoveIndex = i;
        _perftData.setFirstMoveAt(firstMoveIndex, currentMove);
    } else if (currentDepth == _maxDepth - 1) {
        _perftData.increaseNodeCountPerFirstMoveAt(firstMoveIndex);
    }

    _perftData.increaseNodeCountAt(currentDepth + 1);

    if (currentMove.isAnyCapture()) {
        _perftData.increaseCaptureCountAt(currentDepth + 1);
    }

    if (currentMove.isAnyPromo()) {
        _perftData.increasePromotionCountAt(currentDepth + 1);
    }

    if (currentMove.isAnyCastle()) {
        _perftData.increaseCastlingCountAt(currentDepth + 1);
    }

    if (currentMove.isEpCapture()) {
        _perftData.increaseEpCaptureCountAt(currentDepth + 1);
    }

    // FIXME: This is temporary
    // if (_board.isDeadPosition() || 49 >= 50)
    // {
    //     unmakeMove(currentMove, isMaximizer, currentDepth);
    //     return;
    // }
    retFlag = false;
}

} // namespace engine
```


--------------------

# FILE: src/engine/perft/perft.h

```cpp
#pragma once

#include "engine/perft/perftData.h"
#include "engine/pickmove/SearchMemory.h"

#include "io/Fen.h"

#include "model/board/Board.h"
#include "model/move/Movelist.h"
#include "model/move/Move.h"
#include "model/board/PieceMap.h"

#include "logic/makemove/MoveMaker.h"
#include "logic/makemove/MoveRetractor.h"
#include "logic/movegen/MoveGenerator.h"
#include "logic/eval/Evaluator.h"

namespace engine {

class perft {

public:
    perft(int maxDepth);
    
    void minimax(
        int current_depth,
        bool isMaximizer, 
        int firstMoveIndex,
        bool recPerftStats = true,
        const model::Move& lastMove = model::Move(),
        bool verbose = true
    );

    void recordPerftStats(
        bool isMaximizer,
        int currentDepth,
        int &firstMoveIndex,
        size_t i,
        const model::Move& currentMove,
        bool &retFlag
    );

    void genMoves(
        bool isWhite,
        int currentDepth, 
        bitmask enpessantTarget,
        unsigned char castlingRights
    );

    logic::MoveResult makeMove(model::Move move, bool isWhite);
    void unmakeMove(model::Move move, bool isWhite, logic::MoveResult previousMoveResult);
    void undoMove();
    
    int _numMoveGenCalls;
    int _totalNodes;

    void debugPrint(bool verbose) const;

    bool checkCondition(
        int currentDepth,
        bool isMaximizer, 
        int firstMoveIndex, 
        model::Move currentMove, 
        model::Move lastMove, 
        bool verbose, 
        size_t i
    ) const;

    void setMaxDepth(int maxDepth) 
    {
        _maxDepth = maxDepth;
    }

    int getMaxDepth() const {
        return _maxDepth;
    }

    const logic::MoveGenerator& getMoveGenerator() const
    {
        return _moveGenerator;
    }

    void setBoardFromFen(const std::string& fen)
    {
        io::Fen::setBoardFromFen(fen, _bitboards, _stateBitmasks, _pieceMap);
    }

    std::string getFenFromBoard() const
    {
        return io::Fen::getFenFromBoard(_pieceMap);
    }

    bool diffBetweenStateBitmasks() const
    {
        return (_stateBitmasks.getBlackPiecesBitmask() | _stateBitmasks.getWhitePiecesBitmask()) != _stateBitmasks.getOccupiedPiecesBitmask();
    }

    
private:
    int _maxDepth;
    model::Board _board;
    model::Bitboards& _bitboards;
    model::PieceMap& _pieceMap;
    model::StateBitmasks& _stateBitmasks;
    model::ZHasher& _zHasher;
    
    logic::MoveMaker _moveMaker;
    logic::MoveRetractor _moveRetractor;
    logic::MoveGenerator _moveGenerator;
    logic::Evaluator _evaluator;
    
    SearchMemory _searchMemory;
    perftData _perftData;

    int _pseudoLegalMovesCount;
    std::vector<model::Movelist> _movelists;
    std::vector<model::Piece::Type> _lastCapturedPieces;
    std::vector<int> _noCapturedOrPawnMoveCounts; 

    bool tooManyPiecesOnBoard();
};

} // namespace engine
```


--------------------

# FILE: src/engine/perft/perftData.cpp

```cpp
#include "engine/perft/perftData.h"

namespace engine {

perftData::perftData(int maxDepth)
    : _maxDepth(maxDepth)
{
    _nodeCountPerFirstMove.resize(MAX_LEGAL_MOVES);
    _firstMoves.resize(MAX_LEGAL_MOVES);

    for (int i = 0; i < MAX_LEGAL_MOVES; i++) {
        _nodeCountPerFirstMove[i] = 0;
        _firstMoves[i] = model::Move();
    }

    _nodeCount.resize(_maxDepth + 1);
    _captureCount.resize(_maxDepth + 1);
    _epCaptureCount.resize(_maxDepth + 1);
    _castlingCount.resize(_maxDepth + 1);
    _promotionCount.resize(_maxDepth + 1);
    _checkCount.resize(_maxDepth + 1);
    _checkmateCount.resize(_maxDepth + 1);

    for (int i = 0; i < _maxDepth + 1; i++) {
        _nodeCount[i] = 0;
        _captureCount[i] = 0;
        _epCaptureCount[i] = 0;
        _castlingCount[i] = 0;
        _promotionCount[i] = 0;
        _checkCount[i] = 0;
        _checkmateCount[i] = 0;
    }
}

long perftData::sumNodesToDepth(int depth) const 
{
    long sum = 0;

    for (long i = 1; i <= depth; i++) {
        sum += _nodeCount[i];
    }

    return sum;
}

void perftData::resetNodeCounts() 
{
    for (int i = 0; i < _maxDepth + 1; i++) {
        _nodeCount[i] = 0;
        _captureCount[i] = 0;
        _epCaptureCount[i] = 0;
        _castlingCount[i] = 0;
        _promotionCount[i] = 0;
        _checkCount[i] = 0;
        _checkmateCount[i] = 0;
    }
}

} // namespace engine
```


--------------------

# FILE: src/engine/perft/perftData.h

```cpp
#pragma once
#include "common.h"

#include "model/move/Move.h"

namespace engine {

class perftData {

public:
    perftData(int maxDepth);

    long sumNodesToDepth(int depth) const;
    void resetNodeCounts();

    size_t getNumFirstMoves() const { return _firstMoves.size(); }
    int getNodeCountPerFirstMoveAt(int firstMoveIndex) const { return _nodeCountPerFirstMove[firstMoveIndex]; }

    int getNodeCountAt(int depth) const;
    int getCaptureCountAt(int depth) const;
    int getEpCaptureCountAt(int depth) const;
    int getCastlingCountAt(int depth) const;
    int getPromotionCountAt(int depth) const;
    int getCheckCountAt(int depth) const;
    int getCheckmateCountAt(int depth) const;

    void increaseNodeCountAt(int depth);
    void increaseCaptureCountAt(int depth);
    void increaseEpCaptureCountAt(int depth);
    void increaseCastlingCountAt(int depth);
    void increasePromotionCountAt(int depth);
    void increaseCheckCountAt(int depth);
    void increaseCheckmateCountAt(int depth);

    model::Move getFirstMoveAt(int firstMoveIndex) const { return _firstMoves[firstMoveIndex]; }
    void setFirstMoveAt(int firstMoveIndex, model::Move currentMove);
    void increaseNodeCountPerFirstMoveAt(int firstMoveIndex);

private:
    int _maxDepth;

    std::vector<long> _nodeCountPerFirstMove;
    std::vector<model::Move> _firstMoves;
    std::vector<long> _nodeCount;
    std::vector<long> _captureCount;
    std::vector<long> _epCaptureCount;
    std::vector<long> _castlingCount;
    std::vector<long> _promotionCount;
    std::vector<long> _checkCount;
    std::vector<long> _checkmateCount;
};

} // namespace engine
```


--------------------

# FILE: src/engine/perft/perftData.inl

```
namespace engine {

inline int perftData::getNodeCountAt(int depth) const { return _nodeCount[depth]; }
inline int perftData::getCaptureCountAt(int depth) const { return _captureCount[depth]; }
inline int perftData::getEpCaptureCountAt(int depth) const { return _epCaptureCount[depth]; }
inline int perftData::getCastlingCountAt(int depth) const { return _castlingCount[depth]; }
inline int perftData::getPromotionCountAt(int depth) const { return _promotionCount[depth]; }
inline int perftData::getCheckCountAt(int depth) const { return _checkCount[depth]; }
inline int perftData::getCheckmateCountAt(int depth) const { return _checkmateCount[depth]; }

inline void perftData::increaseNodeCountAt(int depth) { _nodeCount[depth]++; }
inline void perftData::increaseCaptureCountAt(int depth) { _captureCount[depth]++; }
inline void perftData::increaseEpCaptureCountAt(int depth) { _epCaptureCount[depth]++; }
inline void perftData::increaseCastlingCountAt(int depth) { _castlingCount[depth]++; }
inline void perftData::increasePromotionCountAt(int depth) { _promotionCount[depth]++; }
inline void perftData::increaseCheckCountAt(int depth) { _checkCount[depth]++; }
inline void perftData::increaseCheckmateCountAt(int depth) { _checkmateCount[depth]++; }

inline void perftData::setFirstMoveAt(int firstMoveIndex, model::Move currentMove) 
{
    _firstMoves[firstMoveIndex] = currentMove;
}

inline void perftData::increaseNodeCountPerFirstMoveAt(int firstMoveIndex) 
{
    _nodeCountPerFirstMove[firstMoveIndex]++;
}

} // namespace engine
```


--------------------

# FILE: src/model/CMakeLists.txt

```
add_subdirectory(board)
add_subdirectory(move)
```


--------------------

# FILE: src/model/move/CMakeLists.txt

```
add_library(move
    Movelist.cpp
)
target_link_libraries(move PUBLIC board cxx_setup)
```


--------------------

# FILE: src/model/move/Move.h

```cpp
#pragma once

#include <functional>

namespace model {

class Move {

public:
    static const int QUITE_FLAG = 0;
    static const int CAPTURE_FLAG = 1;
    static const int EP_CAPTURE_FLAG = 2;
    static const int KNIGHT_PROMO_CAPTURE_FLAG = 3;
    static const int BISHOP_PROMO_CAPTURE_FLAG = 4;
    static const int ROOK_PROMO_CAPTURE_FLAG = 5;
    static const int QUEEN_PROMO_CAPTURE_FLAG = 6;
    static const int KNIGHT_PROMO_FLAG = 7;
    static const int BISHOP_PROMO_FLAG = 8;
    static const int ROOK_PROMO_FLAG = 9;
    static const int QUEEN_PROMO_FLAG = 10;
    static const int TBD_ONE_FLAG = 11;
    static const int TBD_TWO_FLAG = 12;
    static const int DOUBLE_PAWN_PUSH_FLAG = 13;
    static const int KING_CASTLE_FLAG = 14;
    static const int QUEEN_CASTLE_FLAG = 15;
    
    Move();
    Move(int bitIndexFrom, int bitIndexTo, int flag);
    Move(const Move& move);

    int getMove() const;
    int getFlag() const;

    void operator=(const Move& move);
    bool operator==(const Move& move) const;
    bool operator!=(const Move& move) const;

    int getBitIndexFrom() const;
    int getBitIndexTo() const;
    bool isQuite() const;
    bool isDoublePawnPush() const;
    bool isKingCastle() const;
    bool isQueenCastle() const;
    bool isNormalCapture() const;
    bool isEpCapture() const;
    bool isKnightPromo() const;
    bool isBishopPromo() const;
    bool isRookPromo() const;
    bool isQueenPromo() const;
    bool isKnightPromoCapture() const;
    bool isBishopPromoCapture() const;
    bool isRookPromoCapture() const;
    bool isQueenPromoCapture() const;
    bool isAnyCapture() const;
    bool isAnyPromo() const;
    bool isAnyPromoCapture() const;
    bool isAnyCastle() const;

private:
    int _move;
};

} // namespace model

#include "model/move/Move.inl"

namespace std {
    template <>
    struct hash<model::Move> {
        size_t operator()(const model::Move& move) const {
            size_t h1 = std::hash<int>()(move.getBitIndexFrom());
            size_t h2 = std::hash<int>()(move.getBitIndexTo());
            size_t h3 = std::hash<int>()(move.getFlag());

            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };
}
```


--------------------

# FILE: src/model/move/Move.inl

```
namespace model {

inline Move::Move() {
    _move = 0;
}

inline Move::Move(int bitIndexFrom, int bitIndexTo, int flag) {
    _move = ((flag & 0xf) << 12 | (bitIndexTo & 0x3f) << 6 | (bitIndexFrom & 0x3f));
}

inline Move::Move(const Move& move) {
    _move = move._move;
}

inline int Move::getMove() const {
    return _move;
}

inline int Move::getFlag() const {
    return (_move >> 12) & 0xf;
}

inline void Move::operator=(const Move& move) {
    _move = move._move;
}

inline bool Move::operator==(const Move& move) const {
    return (_move & 0xffff) == (move._move & 0xffff);
}

inline bool Move::operator!=(const Move& move) const {
    return (_move & 0xffff) != (move._move & 0xffff);
}

inline int Move::getBitIndexFrom() const {
    return _move & 0x3f;
}

inline int Move::getBitIndexTo() const {
    return (_move >> 6) & 0x3f;
}

inline bool Move::isQuite() const {
    return ((_move >> 12) & 0xf) == QUITE_FLAG;
}

inline bool Move::isDoublePawnPush() const {
    return ((_move >> 12) & 0xf) == DOUBLE_PAWN_PUSH_FLAG;
}

inline bool Move::isKingCastle() const {
    return ((_move >> 12) & 0xf) == KING_CASTLE_FLAG;
}

inline bool Move::isQueenCastle() const {
    return ((_move >> 12) & 0xf) == QUEEN_CASTLE_FLAG;
}

inline bool Move::isNormalCapture() const {
    return ((_move >> 12) & 0xf) == CAPTURE_FLAG;
}

inline bool Move::isEpCapture() const {
    return ((_move >> 12) & 0xf) == EP_CAPTURE_FLAG;
}

inline bool Move::isKnightPromo() const {
    return ((_move >> 12) & 0xf) == KNIGHT_PROMO_FLAG;
}

inline bool Move::isBishopPromo() const {
    return ((_move >> 12) & 0xf) == BISHOP_PROMO_FLAG;
}

inline bool Move::isRookPromo() const {
    return ((_move >> 12) & 0xf) == ROOK_PROMO_FLAG;
}

inline bool Move::isQueenPromo() const {
    return ((_move >> 12) & 0xf) == QUEEN_PROMO_FLAG;
}

inline bool Move::isKnightPromoCapture() const {
    return ((_move >> 12) & 0xf) == KNIGHT_PROMO_CAPTURE_FLAG;
}

inline bool Move::isBishopPromoCapture() const {
    return ((_move >> 12) & 0xf) == BISHOP_PROMO_CAPTURE_FLAG;
}

inline bool Move::isRookPromoCapture() const {
    return ((_move >> 12) & 0xf) == ROOK_PROMO_CAPTURE_FLAG;
}

inline bool Move::isQueenPromoCapture() const {
    return ((_move >> 12) & 0xf) == QUEEN_PROMO_CAPTURE_FLAG;
}

// Checks if the flag is any number between 1 and 6
inline bool Move::isAnyCapture() const {
    return getFlag() >= 1 && getFlag()  <= 6;
}

inline bool Move::isAnyPromo() const {
    return getFlag() >= 3 && getFlag() <= 10;
}

inline bool Move::isAnyPromoCapture() const {
    return getFlag() >= 3 && getFlag() <= 6;
}

inline bool Move::isAnyCastle() const {
    return getFlag() >= 14;
}

} // namespace model
```


--------------------

# FILE: src/model/move/Movelist.cpp

```cpp
#include "model/move/Movelist.h"

namespace model {

Movelist::Movelist() {
    _movelist = std::vector<Move>(MAX_LEGAL_MOVES);
    _moveIndex = 0;
}

void Movelist::addMove(const Move& move) {
    _movelist[_moveIndex] = move;
    _moveIndex++;
}

} // namespace model
```


--------------------

# FILE: src/model/move/Movelist.h

```cpp
#pragma once
#include "common.h"

#include "model/move/Move.h"

namespace model {

class Movelist {

public:
    Movelist();

    const Move& getMoveAt(int index) const;
    int getMoveIndex() const;
    
    void addMove(const Move& move);
    void addNullMove();
    void reset();
    
private:
    std::vector<Move> _movelist;
    int _moveIndex;
};

} // namespace model

#include "model/move/Movelist.inl"
```


--------------------

# FILE: src/model/move/Movelist.inl

```
namespace model {

inline void Movelist::addNullMove() {
    _movelist[_moveIndex] = Move();
}

inline void Movelist::reset() {
    _moveIndex = 0;
}

inline const Move& Movelist::getMoveAt(int index) const {
    return _movelist[index];
}

inline int Movelist::getMoveIndex() const {
    return _moveIndex;
}

}
```


--------------------

# FILE: src/model/board/Bitboards.cpp

```cpp
#include "model/board/Board.h"

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
```


--------------------

# FILE: src/model/board/Bitboards.h

```cpp
#pragma once

#include "model/board/PieceType.h"

namespace model {

class Bitboards {

public:
    Bitboards();

    const bitboard& getBitboardFromPieceType(Piece::Type pieceType) const;
    const bitboard& getBitboardFromIndex(int index) const;
    bool pieceTypeBitIsSet(int index, Piece::Type pieceType) const;
    
    void clearPieceTypeBit(int index, Piece::Type pieceType);
    void setPieceTypeBit(int index, Piece::Type pieceType);
    void resetBitboards();

    // Set, clear and get for all individual piece types / bitboards
    #define DEFINE_BITBOARD_MACROS(PIECE_NAME, VARIABLE_NAME) \
        inline void set##PIECE_NAME##Bit(int square) \
        { \
            VARIABLE_NAME |= (1ULL << square); \
        } \
        inline void clear##PIECE_NAME##Bit(int square) \
        { \
            VARIABLE_NAME &= ~(1ULL << square); \
        } \
        inline const bitboard& get##PIECE_NAME##Bitboard() const \
        { \
            return VARIABLE_NAME; \
        }

    DEFINE_BITBOARD_MACROS(WhitePawns, _whitePawnsBitboard)
    DEFINE_BITBOARD_MACROS(WhiteKnights, _whiteKnightsBitboard)
    DEFINE_BITBOARD_MACROS(WhiteBishops, _whiteBishopsBitboard)
    DEFINE_BITBOARD_MACROS(WhiteRooks, _whiteRooksBitboard)
    DEFINE_BITBOARD_MACROS(WhiteQueens, _whiteQueensBitboard)
    DEFINE_BITBOARD_MACROS(WhiteKing, _whiteKingBitboard)
    DEFINE_BITBOARD_MACROS(BlackPawns, _blackPawnsBitboard)
    DEFINE_BITBOARD_MACROS(BlackKnights, _blackKnightsBitboard)
    DEFINE_BITBOARD_MACROS(BlackBishops, _blackBishopsBitboard)
    DEFINE_BITBOARD_MACROS(BlackRooks, _blackRooksBitboard)
    DEFINE_BITBOARD_MACROS(BlackQueens, _blackQueensBitboard)
    DEFINE_BITBOARD_MACROS(BlackKing, _blackKingBitboard)

private:
    std::vector<bitboard*> _bitboards;

    bitboard _whitePawnsBitboard;
    bitboard _whiteKnightsBitboard;
    bitboard _whiteBishopsBitboard;
    bitboard _whiteRooksBitboard;
    bitboard _whiteQueensBitboard;
    bitboard _whiteKingBitboard;

    bitboard _blackPawnsBitboard;
    bitboard _blackKnightsBitboard;
    bitboard _blackBishopsBitboard;
    bitboard _blackRooksBitboard;
    bitboard _blackQueensBitboard;
    bitboard _blackKingBitboard;
};

} // namespace model

#include "model/board/Bitboards.inl"
```


--------------------

# FILE: src/model/board/Bitboards.inl

```
namespace model {

inline const bitboard& Bitboards::getBitboardFromPieceType(Piece::Type pieceType) const 
{
    return *(_bitboards[Piece::getIntFromType(pieceType)]);
}

inline const bitboard& Bitboards::getBitboardFromIndex(int index) const 
{
    return *(_bitboards[index]);
}

inline bool Bitboards::pieceTypeBitIsSet(int index, Piece::Type pieceType) const 
{
    return (*(_bitboards[Piece::getIntFromType(pieceType)]) & (1ULL << index)) != 0;
}

inline void Bitboards::clearPieceTypeBit(int index, Piece::Type pieceType)
{
    *(_bitboards[Piece::getIntFromType(pieceType)]) &= ~(1ULL << index);
}

inline void Bitboards::setPieceTypeBit(int index, Piece::Type pieceType) 
{
    *(_bitboards[Piece::getIntFromType(pieceType)]) |= (1ULL << index);
}

}
```


--------------------

# FILE: src/model/board/Board.h

```cpp
#pragma once

#include "model/board/Bitboards.h"
#include "model/board/PieceMap.h"
#include "model/board/StateBitmasks.h"
#include "model/board/ZHasher.h"

namespace model {

struct Board {
    Bitboards bitboards;
    PieceMap pieceMap;
    StateBitmasks stateBitmasks;
    ZHasher zHasher;

    Board() 
        : pieceMap(bitboards)
        , stateBitmasks(bitboards)
        , zHasher(pieceMap) 
    {}
};

} // namespace model
```


--------------------

# FILE: src/model/board/CMakeLists.txt

```
add_library(board 
    Bitboards.cpp
    StateBitmasks.cpp
    PieceMap.cpp
    ZHasher.cpp
    PieceType.cpp
)
target_link_libraries(board PUBLIC cxx_setup)
```


--------------------

# FILE: src/model/board/PieceMap.cpp

```cpp
#include "model/board/PieceMap.h"

#include "model/board/Bitboards.h"

namespace model {

PieceMap::PieceMap(const Bitboards& bitboards) 
{
    fillPieceMapFromBitboards(bitboards);
}

void PieceMap::fillPieceMapFromBitboards(const Bitboards& bitboards) 
{
    _pieceMap = std::vector<Piece::Type>(64, Piece::Type::EMPTY);

    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 12; j++) {
            if (bitboards.getBitboardFromIndex(j) & (1ULL << i)) {
                _pieceMap[i] = Piece::getTypeFromInt(j);
                break;
            }
        }
    }
}

} // namespace model
```


--------------------

# FILE: src/model/board/PieceMap.h

```cpp
#pragma once
#include "common.h"

#include "model/board/PieceType.h"

namespace model {

class Bitboards;

class PieceMap {

public:
    PieceMap(const Bitboards& bitboards);

    Piece::Type getPieceTypeAtIndex(int index) const;
    
    void setPieceTypeAtIndex(int index, Piece::Type pieceType);
    void fillPieceMapFromBitboards(const Bitboards& bitboards);
    
private:
    std::vector<Piece::Type> _pieceMap;
};

} // namespace model

#include "model/board/PieceMap.inl"
```


--------------------

# FILE: src/model/board/PieceMap.inl

```
namespace model {

inline Piece::Type PieceMap::getPieceTypeAtIndex(int index) const
{
    return _pieceMap[index];
}

inline void PieceMap::setPieceTypeAtIndex(int index, Piece::Type pieceType)
{
    _pieceMap[index] = pieceType;
}

} // namespace model
```


--------------------

# FILE: src/model/board/PieceType.cpp

```cpp
#include "model/board/PieceType.h"

namespace model {

Piece::Type Piece::getTypeFromChar(char c) {
    switch (c) {
        case 'P': return Piece::Type::W_PAWN;
        case 'N': return Piece::Type::W_KNIGHT;
        case 'B': return Piece::Type::W_BISHOP;
        case 'R': return Piece::Type::W_ROOK;
        case 'Q': return Piece::Type::W_QUEEN;
        case 'K': return Piece::Type::W_KING;
        case 'p': return Piece::Type::B_PAWN;
        case 'n': return Piece::Type::B_KNIGHT;
        case 'b': return Piece::Type::B_BISHOP;
        case 'r': return Piece::Type::B_ROOK;
        case 'q': return Piece::Type::B_QUEEN;
        case 'k': return Piece::Type::B_KING;
        default : return Piece::Type::ERROR;
    }
}

char Piece::getCharFromType(Piece::Type type) {
    switch(type) {
        case Piece::Type::W_PAWN: return 'P';
        case Piece::Type::W_KNIGHT: return 'N';
        case Piece::Type::W_BISHOP: return 'B';
        case Piece::Type::W_ROOK: return 'R';
        case Piece::Type::W_QUEEN: return 'Q';
        case Piece::Type::W_KING: return 'K';
        case Piece::Type::B_PAWN: return 'p';
        case Piece::Type::B_KNIGHT: return 'n';
        case Piece::Type::B_BISHOP: return 'b';
        case Piece::Type::B_ROOK: return 'r';
        case Piece::Type::B_QUEEN: return 'q';
        case Piece::Type::B_KING: return 'k';
        default : return 'e';
    }
}

} // namespace model
```


--------------------

# FILE: src/model/board/PieceType.h

```cpp
#pragma once
#include "common.h"

namespace model {

struct Piece {
    enum class Type : int {
        W_PAWN,
        W_KNIGHT,
        W_BISHOP,
        W_ROOK,
        W_QUEEN,
        W_KING,
        B_PAWN,
        B_KNIGHT,
        B_BISHOP,
        B_ROOK,
        B_QUEEN,
        B_KING,
        EMPTY,
        ERROR
    };

    static Type getTypeFromChar(char c);
    static char getCharFromType(Type type);
    static int getIntFromType(Type type);
    static Type getTypeFromInt(int integer);
};

} // namespace model

#include "model/board/PieceType.inl"
```


--------------------

# FILE: src/model/board/PieceType.inl

```
namespace model {

inline int Piece::getIntFromType(Piece::Type type)
{
    return static_cast<int>(type);
}

inline Piece::Type Piece::getTypeFromInt(int integer)
{
    return static_cast<Piece::Type>(integer);
}

} // namespace model
```


--------------------

# FILE: src/model/board/StateBitmasks.cpp

```cpp
#include "model/board/StateBitmasks.h"

#include "model/board/Bitboards.h"

namespace model {

StateBitmasks::StateBitmasks(const Bitboards& bitboards) 
{
    fillWhitePiecesBitmaskFromBitboards(bitboards);
    fillBlackPiecesBitmaskFromBitboards(bitboards);
    updOccupiedAndEmptySquaresBitmasks();
}

void StateBitmasks::fillWhitePiecesBitmaskFromBitboards(const Bitboards& bitboards) 
{
    bitmask w_all = 0;
    for (int i = 0; i < 6; i++) {
        w_all |= bitboards.getBitboardFromIndex(i);
    }

    _whitePiecesBitmask = w_all;
}

void StateBitmasks::fillBlackPiecesBitmaskFromBitboards(const Bitboards& bitboards) 
{
    bitmask b_all = 0;
    for (int i = 6; i < 12; i++) {
        b_all |= bitboards.getBitboardFromIndex(i);
    }

    _blackPiecesBitmask = b_all;
}

void StateBitmasks::updOccupiedAndEmptySquaresBitmasks() 
{
    updOccupiedPiecesBitmask();
    updEmptySquaresBitmask();
}

} // namespace model
```


--------------------

# FILE: src/model/board/StateBitmasks.h

```cpp
#pragma once
#include "common.h"

namespace model {

class Bitboards;

class StateBitmasks {

public:
    StateBitmasks(const Bitboards& bitboards);

    bool whitePiecesBitIsSet(int index) const;
    bool blackPiecesBitIsSet(int index) const;
    const bitmask& getWhitePiecesBitmask() const;
    const bitmask& getBlackPiecesBitmask() const;
    const bitmask& getOccupiedPiecesBitmask() const;
    const bitmask& getEmptySquaresBitmask() const;
    
    void setWhitePiecesBit(int index);
    void clearWhitePiecesBit(int index);
    void setBlackPiecesBit(int index);
    void clearBlackPiecesBit(int index);
    void updOccupiedAndEmptySquaresBitmasks();
    void fillWhitePiecesBitmaskFromBitboards(const Bitboards& bitboards);
    void fillBlackPiecesBitmaskFromBitboards(const Bitboards& bitboards);

private:
    void updOccupiedPiecesBitmask();
    void updEmptySquaresBitmask();

    bitmask _whitePiecesBitmask;
    bitmask _blackPiecesBitmask;
    bitmask _occupiedPiecesBitmask;
    bitmask _emptySquaresBitmask;
};

} // namespace model

#include "model/board/StateBitmasks.inl"
```


--------------------

# FILE: src/model/board/StateBitmasks.inl

```
namespace model {

inline bool StateBitmasks::whitePiecesBitIsSet(int index) const 
{
    return (_whitePiecesBitmask & (1ULL << index)) != 0;
}

inline bool StateBitmasks::blackPiecesBitIsSet(int index) const 
{
    return (_blackPiecesBitmask & (1ULL << index)) != 0;
}

inline void StateBitmasks::setWhitePiecesBit(int index) 
{
    _whitePiecesBitmask |= (1ULL << index);
}

inline void StateBitmasks::clearWhitePiecesBit(int index) 
{
    _whitePiecesBitmask &= ~(1ULL << index);
}

inline const bitmask& StateBitmasks::getWhitePiecesBitmask() const 
{
    return _whitePiecesBitmask;
}

inline void StateBitmasks::setBlackPiecesBit(int index) 
{
    _blackPiecesBitmask |= (1ULL << index);
}

inline void StateBitmasks::clearBlackPiecesBit(int index) 
{
    _blackPiecesBitmask &= ~(1ULL << index);
}

inline const bitmask& StateBitmasks::getBlackPiecesBitmask() const 
{
    return _blackPiecesBitmask;
}

inline const bitmask& StateBitmasks::getOccupiedPiecesBitmask() const 
{
    return _occupiedPiecesBitmask;
}

inline const bitmask& StateBitmasks::getEmptySquaresBitmask() const 
{
    return _emptySquaresBitmask;
}

inline void StateBitmasks::updOccupiedPiecesBitmask() 
{
    _occupiedPiecesBitmask = _whitePiecesBitmask | _blackPiecesBitmask;
}

inline void StateBitmasks::updEmptySquaresBitmask() 
{
    _emptySquaresBitmask = ~_occupiedPiecesBitmask;
}

} // namespace model
```


--------------------

# FILE: src/model/board/ZHasher.cpp

```cpp
#include "model/board/ZHasher.h"

#include "model/board/PieceMap.h"

#include <iostream>
#include <random>

namespace model {

ZHasher::ZHasher(PieceMap& pieceMap) 
    : _pieceMap(pieceMap)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<uint64_t> dis(0, std::numeric_limits<uint64_t>::max());

    // Generate random numbers for board/piece table
    for (size_t i = 0; i < 64; i++) {
        for (size_t j = 0; j < 12; j++) {
            _randBoardPieceTypeNums[i][j] = dis(gen);
        }
    }

    // Generate random numbers for castle rights table
    for (size_t i = 0; i < 4; i++) {
        _randCastleRightsNums[i] = dis(gen);
    }

    // Generate random numbers for en passant file table
    for (size_t i = 0; i < 8; i++) {
        _randEnPassantFileNums[i] = dis(gen);
    }

    // Generate random number for is white table
    _randIsWhiteNum = dis(gen);
    
    computeInitialHash();
}

void ZHasher::computeInitialHash()
{
    _hash = 0;

    for (size_t i = 0; i < 64; i++) {
        Piece::Type pieceType = _pieceMap.getPieceTypeAtIndex(i);

        if (pieceType != Piece::Type::EMPTY)
            hashSquarePieceType(i, pieceType);
    }

    // We know that we have castling rights, so we can just add them
    for (size_t i = 0; i < 4; i++) {
        hashCastleRights(i);
    }

    // There can be no en passant file

    // We know that it is white's turn
    hashIsWhite(true);
}

} // namespace model
```


--------------------

# FILE: src/model/board/ZHasher.h

```cpp
#pragma once
#include "common.h"

#include "model/board/PieceType.h"

namespace model {

class PieceMap;

class ZHasher {

public:
    ZHasher(PieceMap& pieceMap);
    void hashSquarePieceType(int squareIndex, Piece::Type pieceType);
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

#include "model/board/ZHasher.inl"
```


--------------------

# FILE: src/model/board/ZHasher.inl

```
namespace model {

inline void ZHasher::hashSquarePieceType(int squareIndex, Piece::Type pieceType)
{
    _hash ^= _randBoardPieceTypeNums[squareIndex][static_cast<int>(pieceType)];
}

inline void ZHasher::hashCastleRights(int castleRights)
{
    _hash ^= _randCastleRightsNums[castleRights];
}

inline void ZHasher::hashEnPassantFile(int file)
{
    _hash ^= _randEnPassantFileNums[file];
}

inline void ZHasher::hashIsWhite(bool isWhite)
{
    _hash ^= _randIsWhiteNum;
}

} // namespace model
```


--------------------

# FILE: tests/CMakeLists.txt

```
if (BUILD_TESTING)
    include(GoogleTest)
    add_subdirectory(engine)
    # add_subdirectory(io)
    add_subdirectory(logic)
    add_subdirectory(model)
endif()
```


--------------------

# FILE: tests/logic/CMakeLists.txt

```
# add_subdirectory(eval)
add_subdirectory(makemove)
add_subdirectory(movegen)
```


--------------------

# FILE: tests/logic/movegen/BaseGenerator_test.h

```cpp
#pragma once

#include <gtest/gtest.h>

#include "model/board/Board.h"
#include "model/move/Move.h"
#include "model/move/Movelist.h"
#include "logic/movegen/MoveGenerator.h"

#include "logic/makemove/MoveMaker.h"
#include "logic/makemove/MoveRetractor.h"

#include "engine/pickmove/SearchMemory.h"
#include "io/Fen.h"


#include <unordered_set>

namespace logic {

class BaseGenerator : public ::testing::Test 
{
protected:
    model::Board board;
    model::Bitboards& bitboards;
    model::StateBitmasks& stateBitmasks;
    model::PieceMap& pieceMap;
    model::ZHasher& zHasher;
    engine::SearchMemory searchMemory;
    MoveMaker moveMaker;
    MoveRetractor moveRetractor;
    MoveGenerator moveGenerator;
    std::string startingPos;
    model::Movelist movelist;

    BaseGenerator() 
        : board(),
          bitboards(board.bitboards),
          stateBitmasks(board.stateBitmasks),
          pieceMap(board.pieceMap),
          zHasher(board.zHasher),
          searchMemory(engine::SearchMemory(0)),
          moveMaker(board),
          moveRetractor(board),
          moveGenerator(MoveGenerator(board, moveMaker, moveRetractor)),
          movelist(model::Movelist()) {}

    virtual void SetUp() override {
        // board = model::ChessBoard();
        // moveGenerator = MoveGenerator(board);
        startingPos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";;
    }

    virtual void TearDown() override {
        bitboards.resetBitboards();
    }

    void insertExpectedMoves(std::unordered_set<model::Move>& moves, int fromBitIndex, const std::vector<int>& toBitIndices, const std::vector<int>& flags) {
        for (size_t i = 0; i < toBitIndices.size(); i++) {
            moves.insert(model::Move(fromBitIndex, toBitIndices[i], flags[i]));
        }
    }

    model::Movelist getMoves() {
        return movelist;
    }
};

} // namespace logic
```


--------------------

# FILE: tests/logic/movegen/BishopGenerator_test.cpp

```cpp
#include <gtest/gtest.h>

#include "BaseGenerator_test.h"

namespace logic {

class MoveGeneratorBishopTest : public BaseGenerator 
{
protected:
    std::string fenOne;

    void SetUp() override {
        BaseGenerator::SetUp();
        fenOne = "5P2/4b3/1P6/2B2B2/1r3b2/8/8/8";
    }
};

TEST_F(MoveGeneratorBishopTest, genBishopMoves_fenOneWhite_ShouldReturn18Moves) 
{
    io::Fen::setBoardFromFen(fenOne, bitboards, stateBitmasks, pieceMap);
    moveGenerator.genBishopMoves(true, movelist);

    model::Movelist moves = getMoves();
    std::unordered_set<model::Move> expectedMoves;
    insertExpectedMoves(expectedMoves, 34, {41, 48, 25, 16, 27, 20, 13, 6, 43, 52, 61}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    insertExpectedMoves(expectedMoves, 37, {44, 51, 28, 19, 10, 1, 30}, {0, 1, 0, 0, 0, 0, 1});

    for (int i = 0; i < movelist.getMoveIndex(); i++) {
        auto found = expectedMoves.find(movelist.getMoveAt(i));
        ASSERT_TRUE(found != expectedMoves.end());
        expectedMoves.erase(found); // Remove found move from the set
    }

    ASSERT_TRUE(expectedMoves.empty());
}

TEST_F(MoveGeneratorBishopTest, genBishopMoves_fenOneBlack_ShouldReturn18Moves) 
{
    io::Fen::setBoardFromFen(fenOne, bitboards, stateBitmasks, pieceMap);
    moveGenerator.genBishopMoves(false, movelist);

    model::Movelist moves = getMoves();
    std::unordered_set<model::Move> expectedMoves;
    insertExpectedMoves(expectedMoves, 51, {58, 42, 33, 24, 44, 37, 60}, {1, 0, 0, 0, 0, 1, 0});
    insertExpectedMoves(expectedMoves, 26, {33, 40, 17, 8, 19, 12, 5, 35, 44, 53, 62}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});

    for (int i = 0; i < movelist.getMoveIndex(); i++) {
        auto found = expectedMoves.find(movelist.getMoveAt(i));
        ASSERT_TRUE(found != expectedMoves.end());
        expectedMoves.erase(found); // Remove found move from the set
    }

    ASSERT_TRUE(expectedMoves.empty());
}

TEST_F(MoveGeneratorBishopTest, genBishopMoves_startingPosWhite_ShouldReturn0Moves) 
{
    io::Fen::setBoardFromFen(startingPos, bitboards, stateBitmasks, pieceMap);
    moveGenerator.genBishopMoves(true, movelist);

    ASSERT_EQ(movelist.getMoveIndex(), 0);
}

TEST_F(MoveGeneratorBishopTest, genBishopMoves_startingPosBlack_ShouldReturn0Moves) 
{
    io::Fen::setBoardFromFen(startingPos, bitboards, stateBitmasks, pieceMap);
    moveGenerator.genBishopMoves(false, movelist);;

    ASSERT_EQ(movelist.getMoveIndex(), 0);
}

} // namespace logic
```


--------------------

# FILE: tests/logic/movegen/CMakeLists.txt

```
add_executable(movegen_tests)

# Add test subdirectories
add_subdirectory(utils)
add_subdirectory(bitmasks)

target_sources(movegen_tests PRIVATE
    RookGenerator_test.cpp
    BishopGenerator_test.cpp
    QueenGenerator_test.cpp
    KnightGenerator_test.cpp
    KingGenerator_test.cpp
    PawnGenerator_test.cpp
    CastlingGenerator_test.cpp
    CheckDetection_test.cpp
)

target_link_libraries(movegen_tests PRIVATE makemove perft movegen board GTest::gtest_main)

gtest_discover_tests(movegen_tests)
```


--------------------

# FILE: tests/logic/movegen/CastlingGenerator_test.cpp

```cpp
// #include <gtest/gtest.h>

// #include "BaseGenerator_test.h"


//     class MoveGeneratorCastlingTest : public BaseGenerator {
//         protected:
//             std::string fenOne;
//             std::string fenTwo;
//             std::string fenThree;
//             std::string bugFen;
//             std::string castlingWhileInCheckWhiteFen;
//             std::string castlingWhileInCheckBlackFen;
//             std::string castlingThroughAttacksWhiteFen;
//             std::string castlingThroughAttacksBlackFen;

//             void SetUp() override {
//                 BaseGenerator::SetUp();
//                 fenOne = "r3k2r/8/8/8/8/8/8/R3K2R";
//                 fenTwo = "r1n1k1Nr/8/8/8/8/8/8/R2pK1PR";
//                 fenThree = "4k2r/r7/8/8/8/8/7R/R3K3";
//                 bugFen = "6n1/8/7N/1p2b3/P2B4/8/4q1r1/4Q1R1";
//                 castlingWhileInCheckWhiteFen = "r3k2r/4q3/8/8/8/8/8/R3K2R";
//                 castlingWhileInCheckBlackFen = "r3k2r/8/8/8/8/8/4Q3/R3K2R";
//                 castlingThroughAttacksWhiteFen = "r3k2r/3r1r2/8/8/8/8/8/R3K2R";
//                 castlingThroughAttacksBlackFen = "r3k2r/8/8/8/8/8/3R1R2/R3K2R";
//             }
//     };

//     TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_fenOneWhite_ShouldReturn2Moves) {
//         io::Fen::setBoardFromFen(fenOne, bitboards, stateBitmasks, pieceMap);
//         moveGenerator.genCastlingMoves(true, movelist);

//         model::Movelist moves = getMoves();
//         std::unordered_set<model::Move> expectedMoves;
//         insertExpectedMoves(expectedMoves, 0, {0, 0}, {model::Move::KING_CASTLE_FLAG, model::Move::QUEEN_CASTLE_FLAG});

//         for (int i = 0; i < movelist.getMoveIndex(); i++) {
//             auto found = expectedMoves.find(movelist.getMoveAt(i));
//             ASSERT_TRUE(found != expectedMoves.end());
//             expectedMoves.erase(found); // Remove found move from the set
//         }

//         ASSERT_TRUE(expectedMoves.empty());
//     }

//     TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_fenOneBlack_ShouldReturn2Moves) {
//         io::Fen::setBoardFromFen(fenOne, bitboards, stateBitmasks, pieceMap);
//         moveGenerator.genCastlingMoves(true, movelist);

//         model::Movelist moves = getMoves();
//         std::unordered_set<model::Move> expectedMoves;
//         insertExpectedMoves(expectedMoves, 0, {0, 0}, {model::Move::KING_CASTLE_FLAG, model::Move::QUEEN_CASTLE_FLAG});

//         for (int i = 0; i < movelist.getMoveIndex(); i++) {
//             auto found = expectedMoves.find(movelist.getMoveAt(i));
//             ASSERT_TRUE(found != expectedMoves.end());
//             expectedMoves.erase(found); // Remove found move from the set
//         }

//         ASSERT_TRUE(expectedMoves.empty());
//     }


//     TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_fenTwo_ShouldReturn0Moves) {
//         io::Fen::setBoardFromFen(fenTwo, bitboards, stateBitmasks, pieceMap);
//         moveGenerator.genCastlingMoves(true, movelist);
//         moveGenerator.genCastlingMoves(false, movelist);

//         ASSERT_EQ(movelist.getMoveIndex(), 0);
//     }

//     TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_fenThreeWhite_ShouldReturn1Moves) {
//         io::Fen::setBoardFromFen(fenThree, bitboards, stateBitmasks, pieceMap);
//         moveGenerator.getBoard().setRookHMoved(true, true);
//         moveGenerator.genCastlingMoves(true, movelist);

//         model::Movelist moves = getMoves();
//         std::unordered_set<model::Move> expectedMoves;

//         insertExpectedMoves(expectedMoves, 0, {0}, {model::Move::QUEEN_CASTLE_FLAG});

//         for (int i = 0; i < movelist.getMoveIndex(); i++) {
//             auto found = expectedMoves.find(movelist.getMoveAt(i));
//             ASSERT_TRUE(found != expectedMoves.end());
//             expectedMoves.erase(found); // Remove found move from the set
//         }

//         ASSERT_TRUE(expectedMoves.empty());
//     }

//     TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_fenThreeBlack_ShouldReturn1Moves) {
//         io::Fen::setBoardFromFen(fenThree, bitboards, stateBitmasks, pieceMap);
//         moveGenerator.getBoard().setRookAMoved(false, true);
//         moveGenerator.genCastlingMoves(false, movelist);

//         model::Movelist moves = getMoves();
//         std::unordered_set<model::Move> expectedMoves;

//         insertExpectedMoves(expectedMoves, 0, {0}, {model::Move::KING_CASTLE_FLAG});

//         for (int i = 0; i < movelist.getMoveIndex(); i++) {
//             auto found = expectedMoves.find(movelist.getMoveAt(i));
//             ASSERT_TRUE(found != expectedMoves.end());
//             expectedMoves.erase(found); // Remove found move from the set
//         }

//         ASSERT_TRUE(expectedMoves.empty());
//     }

//     TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_fenStartingPos_ShouldReturn0Moves) {
//         io::Fen::setBoardFromFen(startingPos, bitboards, stateBitmasks, pieceMap);
//         moveGenerator.genCastlingMoves(true, movelist);
//         moveGenerator.genCastlingMoves(false, movelist);

//         ASSERT_EQ(movelist.getMoveIndex(), 0);
//     }

//     TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_fenOneWhiteKingMoved_ShouldReturn0Moves) {
//         io::Fen::setBoardFromFen(fenOne, bitboards, stateBitmasks, pieceMap);
//         moveGenerator.getBoard().setKingMoved(true, true);
//         moveGenerator.genCastlingMoves(true, movelist);

//         ASSERT_EQ(movelist.getMoveIndex(), 0);
//     }

//     TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_fenOneBlackKingMoved_ShouldReturn0Moves) {
//         io::Fen::setBoardFromFen(fenOne, bitboards, stateBitmasks, pieceMap);
//         moveGenerator.getBoard().setKingMoved(false, true);
//         moveGenerator.genCastlingMoves(false, movelist);

//         ASSERT_EQ(movelist.getMoveIndex(), 0);
//     }

//     TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_fenOneWhiteRookAMoved_ShouldReturn1Moves) {
//         io::Fen::setBoardFromFen(fenOne, bitboards, stateBitmasks, pieceMap);
//         moveGenerator.getBoard().setRookAMoved(true, true);
//         moveGenerator.genCastlingMoves(true, movelist);

//         model::Movelist moves = getMoves();
//         std::unordered_set<model::Move> expectedMoves;

//         insertExpectedMoves(expectedMoves, 0, {0}, {model::Move::KING_CASTLE_FLAG});

//         for (int i = 0; i < movelist.getMoveIndex(); i++) {
//             auto found = expectedMoves.find(movelist.getMoveAt(i));
//             ASSERT_TRUE(found != expectedMoves.end());
//             expectedMoves.erase(found); // Remove found move from the set
//         }

//         ASSERT_TRUE(expectedMoves.empty());
//     }

//     TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_fenOneBlackRookAMoved_ShouldReturn1Moves) {
//         io::Fen::setBoardFromFen(fenOne, bitboards, stateBitmasks, pieceMap);
//         moveGenerator.getBoard().setRookAMoved(false, true);
//         moveGenerator.genCastlingMoves(false, movelist);

//         model::Movelist moves = getMoves();
//         std::unordered_set<model::Move> expectedMoves;

//         insertExpectedMoves(expectedMoves, 0, {0}, {model::Move::KING_CASTLE_FLAG});

//         for (int i = 0; i < movelist.getMoveIndex(); i++) {
//             auto found = expectedMoves.find(movelist.getMoveAt(i));
//             ASSERT_TRUE(found != expectedMoves.end());
//             expectedMoves.erase(found); // Remove found move from the set
//         }

//         ASSERT_TRUE(expectedMoves.empty());
//     }

//     TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_fenOneWhiteRookHMoved_ShouldReturn1Moves) {
//         io::Fen::setBoardFromFen(fenOne, bitboards, stateBitmasks, pieceMap);
//         moveGenerator.getBoard().setRookHMoved(true, true);
//         moveGenerator.genCastlingMoves(true, movelist);

//         model::Movelist moves = getMoves();
//         std::unordered_set<model::Move> expectedMoves;

//         insertExpectedMoves(expectedMoves, 0, {0}, {model::Move::QUEEN_CASTLE_FLAG});

//         for (int i = 0; i < movelist.getMoveIndex(); i++) {
//             auto found = expectedMoves.find(movelist.getMoveAt(i));
//             ASSERT_TRUE(found != expectedMoves.end());
//             expectedMoves.erase(found); // Remove found move from the set
//         }

//         ASSERT_TRUE(expectedMoves.empty());
//     }

//     TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_fenOneBlackRookHMoved_ShouldReturn1Moves) {
//         io::Fen::setBoardFromFen(fenOne, bitboards, stateBitmasks, pieceMap);
//         moveGenerator.getBoard().setRookHMoved(false, true);
//         moveGenerator.genCastlingMoves(false, movelist);

//         model::Movelist moves = getMoves();
//         std::unordered_set<model::Move> expectedMoves;

//         insertExpectedMoves(expectedMoves, 0, {0}, {model::Move::QUEEN_CASTLE_FLAG});

//         for (int i = 0; i < movelist.getMoveIndex(); i++) {
//             auto found = expectedMoves.find(movelist.getMoveAt(i));
//             ASSERT_TRUE(found != expectedMoves.end());
//             expectedMoves.erase(found); // Remove found move from the set
//         }

//         ASSERT_TRUE(expectedMoves.empty());
//     }

//     TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_bugFenWhite_ShouldReturn0Moves) {
//         io::Fen::setBoardFromFen(bugFen, bitboards, stateBitmasks, pieceMap);
//         moveGenerator.genCastlingMoves(true, movelist);

//         ASSERT_EQ(movelist.getMoveIndex(), 0);
//     }

//     TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_castlingWhileInCheckWhiteFen_ShouldReturn0Moves) {
//         io::Fen::setBoardFromFen(castlingWhileInCheckWhiteFen, bitboards, stateBitmasks, pieceMap);
//         moveGenerator.genCastlingMoves(true, movelist);

//         ASSERT_EQ(movelist.getMoveIndex(), 0);
//     }

//     TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_castlingWhileInCheckBlackFen_ShouldReturn0Moves) {
//         io::Fen::setBoardFromFen(castlingWhileInCheckBlackFen, bitboards, stateBitmasks, pieceMap);
//         moveGenerator.genCastlingMoves(false, movelist);

//         ASSERT_EQ(movelist.getMoveIndex(), 0);
//         }

//     TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_castlingThroughAttacksWhiteFenWhite_ShouldReturn0Moves) {
//         io::Fen::setBoardFromFen(castlingThroughAttacksWhiteFen, bitboards, stateBitmasks, pieceMap);
//         moveGenerator.genCastlingMoves(true, movelist);

//         ASSERT_EQ(movelist.getMoveIndex(), 0);
//     }

//     TEST_F(MoveGeneratorCastlingTest, genCastlingMoves_castlingThroughAttacksBlackFenBlack_ShouldReturn0Moves) {
//         io::Fen::setBoardFromFen(castlingThroughAttacksBlackFen, bitboards, stateBitmasks, pieceMap);
//         moveGenerator.genCastlingMoves(false, movelist);

//         ASSERT_EQ(movelist.getMoveIndex(), 0);
//     }
// }
```


--------------------

# FILE: tests/logic/movegen/CheckDetection_test.cpp

```cpp
#include <gtest/gtest.h>

#include "BaseGenerator_test.h"

namespace logic {

class MoveGeneratorCheckDetectionTest : public BaseGenerator {
protected:
    std::string fenPawnChecks;
    std::string fenKnightChecks;
    std::string fenBishopChecksTrue;
    std::string fenBishopChecksFalse;
    std::string fenRookChecksTrue;
    std::string fenRookChecksFalse;
    std::string fenQueenChecksTrue;
    std::string fenQueenChecksFalse;

    void SetUp() override {
        BaseGenerator::SetUp();
        fenPawnChecks = "8/8/5k2/6P1/8/2p5/3K4/8";
        fenKnightChecks = "8/8/5k2/3N4/8/3n4/1K6/8";
        fenBishopChecksTrue = "8/4B3/5k2/4b3/8/8/1K6/8";
        fenBishopChecksFalse = "3B4/4N3/5k2/4b3/3p4/8/1K6/8";
        fenRookChecksTrue = "8/8/1R3k2/8/8/8/1K6/1r6";
        fenRookChecksFalse = "5R2/5p2/5k2/8/8/8/1K2N2r/8";
        fenQueenChecksTrue = "8/8/5k1Q/8/8/8/1K6/q7";
        fenQueenChecksFalse = "8/8/5kqQ/1q6/1P6/8/1K6/8";
    }
};

TEST_F(MoveGeneratorCheckDetectionTest, checkDetection_fenPawnChecksWhite_ShouldReturnTrue) 
{
    io::Fen::setBoardFromFen(fenPawnChecks, bitboards, stateBitmasks, pieceMap);
    ASSERT_TRUE(moveGenerator.isInCheck(true));
}

TEST_F(MoveGeneratorCheckDetectionTest, checkDetection_fenPawnChecksBlack_ShouldReturnTrue)
{
    io::Fen::setBoardFromFen(fenPawnChecks, bitboards, stateBitmasks, pieceMap);
    ASSERT_TRUE(moveGenerator.isInCheck(false));
}

TEST_F(MoveGeneratorCheckDetectionTest, checkDetection_fenKnightChecksWhite_ShouldReturnTrue)
{
    io::Fen::setBoardFromFen(fenKnightChecks, bitboards, stateBitmasks, pieceMap);
    ASSERT_TRUE(moveGenerator.isInCheck(true));
}

TEST_F(MoveGeneratorCheckDetectionTest, checkDetection_fenKnightChecksBlack_ShouldReturnTrue)
{
    io::Fen::setBoardFromFen(fenKnightChecks, bitboards, stateBitmasks, pieceMap);
    ASSERT_TRUE(moveGenerator.isInCheck(false));
}

TEST_F(MoveGeneratorCheckDetectionTest, checkDetection_fenBishopChecksTrueWhite_ShouldReturnTrue)
{
    io::Fen::setBoardFromFen(fenBishopChecksTrue, bitboards, stateBitmasks, pieceMap);
    ASSERT_TRUE(moveGenerator.isInCheck(true));
}

TEST_F(MoveGeneratorCheckDetectionTest, checkDetection_fenBishopChecksTrueBlack_ShouldReturnTrue)
{
    io::Fen::setBoardFromFen(fenBishopChecksTrue, bitboards, stateBitmasks, pieceMap);
    ASSERT_TRUE(moveGenerator.isInCheck(false));
}

TEST_F(MoveGeneratorCheckDetectionTest, checkDetection_fenBishopChecksFalseWhite_ShouldReturnFalse)
{
    io::Fen::setBoardFromFen(fenBishopChecksFalse, bitboards, stateBitmasks, pieceMap);
    ASSERT_FALSE(moveGenerator.isInCheck(true));
}

TEST_F(MoveGeneratorCheckDetectionTest, checkDetection_fenBishopChecksFalseBlack_ShouldReturnFalse)
{
    io::Fen::setBoardFromFen(fenBishopChecksFalse, bitboards, stateBitmasks, pieceMap);
    ASSERT_FALSE(moveGenerator.isInCheck(false));
}

TEST_F(MoveGeneratorCheckDetectionTest, checkDetection_fenRookChecksTrueWhite_ShouldReturnTrue)
{
    io::Fen::setBoardFromFen(fenRookChecksTrue, bitboards, stateBitmasks, pieceMap);
    ASSERT_TRUE(moveGenerator.isInCheck(true));
}

TEST_F(MoveGeneratorCheckDetectionTest, checkDetection_fenRookChecksTrueBlack_ShouldReturnTrue)
{
    io::Fen::setBoardFromFen(fenRookChecksTrue, bitboards, stateBitmasks, pieceMap);
    ASSERT_TRUE(moveGenerator.isInCheck(false));
}

TEST_F(MoveGeneratorCheckDetectionTest, checkDetection_fenRookChecksFalseWhite_ShouldReturnFalse)
{
    io::Fen::setBoardFromFen(fenRookChecksFalse, bitboards, stateBitmasks, pieceMap);
    ASSERT_FALSE(moveGenerator.isInCheck(true));
}

TEST_F(MoveGeneratorCheckDetectionTest, checkDetection_fenRookChecksFalseBlack_ShouldReturnFalse)
{
    io::Fen::setBoardFromFen(fenRookChecksFalse, bitboards, stateBitmasks, pieceMap);
    ASSERT_FALSE(moveGenerator.isInCheck(false));
}

TEST_F(MoveGeneratorCheckDetectionTest, checkDetection_fenQueenChecksTrueWhite_ShouldReturnTrue)
{
    io::Fen::setBoardFromFen(fenQueenChecksTrue, bitboards, stateBitmasks, pieceMap);
    ASSERT_TRUE(moveGenerator.isInCheck(true));
}

TEST_F(MoveGeneratorCheckDetectionTest, checkDetection_fenQueenChecksTrueBlack_ShouldReturnTrue)
{
    io::Fen::setBoardFromFen(fenQueenChecksTrue, bitboards, stateBitmasks, pieceMap);
    ASSERT_TRUE(moveGenerator.isInCheck(false));
}

TEST_F(MoveGeneratorCheckDetectionTest, checkDetection_fenQueenChecksFalseWhite_ShouldReturnFalse)
{
    io::Fen::setBoardFromFen(fenQueenChecksFalse, bitboards, stateBitmasks, pieceMap);
    ASSERT_FALSE(moveGenerator.isInCheck(true));
}

TEST_F(MoveGeneratorCheckDetectionTest, checkDetection_fenQueenChecksFalseBlack_ShouldReturnFalse)
{
    io::Fen::setBoardFromFen(fenQueenChecksFalse, bitboards, stateBitmasks, pieceMap);
    ASSERT_FALSE(moveGenerator.isInCheck(false));
}

TEST_F(MoveGeneratorCheckDetectionTest, checkDetection_startingPosWhite_ShouldReturnFalse)
{
    io::Fen::setBoardFromFen(startingPos, bitboards, stateBitmasks, pieceMap);
    ASSERT_FALSE(moveGenerator.isInCheck(true));
}

TEST_F(MoveGeneratorCheckDetectionTest, checkDetection_startingPosBlack_ShouldReturnFalse)
{
    io::Fen::setBoardFromFen(startingPos, bitboards, stateBitmasks, pieceMap);
    ASSERT_FALSE(moveGenerator.isInCheck(false));
}

} // namespace logic
```


--------------------

# FILE: tests/logic/movegen/KingGenerator_test.cpp

```cpp
#include <gtest/gtest.h>

#include "BaseGenerator_test.h"

namespace logic {

class MoveGeneratorKingTest : public BaseGenerator 
{
protected:
    std::string fenOne;

    void SetUp() override {
        BaseGenerator::SetUp();
        fenOne = "5r2/3qk3/4R3/8/8/8/Pn6/1K6";
    }
};

TEST_F(MoveGeneratorKingTest, genKingMoves_fenOneWhite_ShouldReturn4Moves)
{
    io::Fen::setBoardFromFen(fenOne, bitboards, stateBitmasks, pieceMap);
    moveGenerator.genKingMoves(true, movelist);

    model::Movelist moves = getMoves();
    std::unordered_set<model::Move> expectedMoves;
    insertExpectedMoves(expectedMoves, 6, {14, 13, 5, 7}, {1, 0, 0, 0});

    for (int i = 0; i < movelist.getMoveIndex(); i++) {
        auto found = expectedMoves.find(movelist.getMoveAt(i));
        ASSERT_TRUE(found != expectedMoves.end());
        expectedMoves.erase(found); // Remove found move from the set
    }

    ASSERT_TRUE(expectedMoves.empty());
}

TEST_F(MoveGeneratorKingTest, genKingMoves_fenOneBlack_ShouldReturn6Moves)
{
    io::Fen::setBoardFromFen(fenOne, bitboards, stateBitmasks, pieceMap);
    moveGenerator.genKingMoves(false, movelist);

    model::Movelist moves = getMoves();
    std::unordered_set<model::Move> expectedMoves;
    insertExpectedMoves(expectedMoves, 51, {59, 50, 42, 43, 44, 60}, {0, 0, 0, 1, 0, 0});

    for (int i = 0; i < movelist.getMoveIndex(); i++) {
        auto found = expectedMoves.find(movelist.getMoveAt(i));
        ASSERT_TRUE(found != expectedMoves.end());
        expectedMoves.erase(found); // Remove found move from the set
    }

    ASSERT_TRUE(expectedMoves.empty());
}

TEST_F(MoveGeneratorKingTest, genKingMoves_startingPosWhite_ShouldReturn0Moves)
{
    io::Fen::setBoardFromFen(startingPos, bitboards, stateBitmasks, pieceMap);
    moveGenerator.genKingMoves(true, movelist);

    ASSERT_EQ(movelist.getMoveIndex(), 0);
}

TEST_F(MoveGeneratorKingTest, genKingMoves_startingPosBlack_ShouldReturn0Moves)
{
    io::Fen::setBoardFromFen(startingPos, bitboards, stateBitmasks, pieceMap);
    moveGenerator.genKingMoves(false, movelist);

    ASSERT_EQ(movelist.getMoveIndex(), 0);
}

} // namespace logic
```


--------------------

# FILE: tests/logic/movegen/KnightGenerator_test.cpp

```cpp
#include <gtest/gtest.h>

#include "BaseGenerator_test.h"

namespace logic {

class MoveGeneratorKnightTest : public BaseGenerator 
{
protected:
    std::string fenOne;

    void SetUp() override {
        BaseGenerator::SetUp();
        fenOne = "8/1n1n4/5R2/2N5/4P3/8/5p2/7N";
    }
};

TEST_F(MoveGeneratorKnightTest, genKnightMoves_startPosWhite_ShouldReturn4Moves) 
{
    io::Fen::setBoardFromFen(startingPos, bitboards, stateBitmasks, pieceMap);
    moveGenerator.genKnightMoves(true, movelist);

    model::Movelist moves = getMoves();
    std::unordered_set<model::Move> expectedMoves;
    insertExpectedMoves(expectedMoves, 1, {16, 18}, {0, 0});
    insertExpectedMoves(expectedMoves, 6, {21, 23}, {0, 0});

    for (int i = 0; i < movelist.getMoveIndex(); i++) {
        auto found = expectedMoves.find(movelist.getMoveAt(i));
        ASSERT_TRUE(found != expectedMoves.end());
        expectedMoves.erase(found); // Remove found move from the set
    }

    ASSERT_TRUE(expectedMoves.empty());
}

TEST_F(MoveGeneratorKnightTest, genKnightMoves_startPosBlack_ShouldReturn4Moves) 
{
    io::Fen::setBoardFromFen(startingPos, bitboards, stateBitmasks, pieceMap);
    moveGenerator.genKnightMoves(false, movelist);

    model::Movelist moves = getMoves();
    std::unordered_set<model::Move> expectedMoves;
    insertExpectedMoves(expectedMoves, 57, {40, 42}, {0, 0});
    insertExpectedMoves(expectedMoves, 62, {45, 47}, {0, 0});

    for (int i = 0; i < movelist.getMoveIndex(); i++) {
        auto found = expectedMoves.find(movelist.getMoveAt(i));
        ASSERT_TRUE(found != expectedMoves.end());
        expectedMoves.erase(found); // Remove found move from the set
    }

    ASSERT_TRUE(expectedMoves.empty());
}

TEST_F(MoveGeneratorKnightTest, genKnightMoves_fenOneWhite_ShouldReturn8Moves) 
{
    io::Fen::setBoardFromFen(fenOne, bitboards, stateBitmasks, pieceMap);
    moveGenerator.genKnightMoves(true, movelist);

    model::Movelist moves = getMoves();
    std::unordered_set<model::Move> expectedMoves;
    insertExpectedMoves(expectedMoves, 0, {10, 17}, {1, 0});
    insertExpectedMoves(expectedMoves, 37, {52, 43, 20, 22, 31, 47, 54}, {1, 0, 0, 0, 0, 0, 1});

    for (int i = 0; i < movelist.getMoveIndex(); i++) {
        auto found = expectedMoves.find(movelist.getMoveAt(i));
        ASSERT_TRUE(found != expectedMoves.end());
        expectedMoves.erase(found); // Remove found move from the set
    }

    ASSERT_TRUE(expectedMoves.empty());
}

TEST_F(MoveGeneratorKnightTest, genKnightMoves_fenOneBlack_ShouldReturn10Moves) 
{
    io::Fen::setBoardFromFen(fenOne, bitboards, stateBitmasks, pieceMap);
    moveGenerator.genKnightMoves(false, movelist);

    model::Movelist moves = getMoves();
    std::unordered_set<model::Move> expectedMoves;
    insertExpectedMoves(expectedMoves, 52, {58, 42, 35, 37, 46, 62}, {0, 1, 0, 1, 0, 0});
    insertExpectedMoves(expectedMoves, 54, {60, 44, 37, 39}, {0, 0, 1, 0});

    for (int i = 0; i < movelist.getMoveIndex(); i++) {
        auto found = expectedMoves.find(movelist.getMoveAt(i));
        ASSERT_TRUE(found != expectedMoves.end());
        expectedMoves.erase(found); // Remove found move from the set
    }

    ASSERT_TRUE(expectedMoves.empty());
}

} // namespace logic
```


--------------------

# FILE: tests/logic/movegen/PawnGenerator_test.cpp

```cpp
#include <gtest/gtest.h>

#include "ChessEngine/tests/logic/movegen/BaseGenerator_test.h"

namespace logic {

class MoveGeneratorPawnTest : public BaseGenerator 
{
protected:
    std::string fenOne;
    std::string fenEnPessantForWhiteTest;
    std::string fenEnPessantForBlackTest;
    std::string fenPromotionTest;
    std::string fenBuggedPawnTest;
    // ponder::perft::SearchMemory searchMemory;

    void SetUp() override {
        BaseGenerator::SetUp();
        fenOne = "8/5p1p/5R1p/2p5/3P4/p7/5P1P/8";
        fenEnPessantForWhiteTest = "rnbqkb1r/pppp1ppp/5n2/3Pp3/8/8/8/RNBQKBNR";
        fenEnPessantForBlackTest = "rnbqkbnr/8/8/8/Pp6/8/1PPPPPPP/RNBQKBNR";
        fenPromotionTest = "3q4/2P3P1/8/8/8/8/1p5p/2N5";
        fenBuggedPawnTest = "rnbqkb1r/pppppppp/7n/8/8/2N4N/PPPPPPPP/R1BQKB1R";
        // searchMemory = ponder::perft::SearchMemory(0);
    }
};

TEST_F(MoveGeneratorPawnTest, genPawnMoves_startPosWhite_ShouldReturn16Moves) 
{
    io::Fen::setBoardFromFen(startingPos, bitboards, stateBitmasks, pieceMap);
    moveGenerator.genPawnMoves(true, movelist, 0);

    model::Movelist moves = getMoves();
    std::unordered_set<model::Move> expectedMoves;
    insertExpectedMoves(expectedMoves, 8, {16, 24}, {0, model::Move::DOUBLE_PAWN_PUSH_FLAG});
    insertExpectedMoves(expectedMoves, 9, {17, 25}, {0, model::Move::DOUBLE_PAWN_PUSH_FLAG});
    insertExpectedMoves(expectedMoves, 10, {18, 26}, {0, model::Move::DOUBLE_PAWN_PUSH_FLAG});
    insertExpectedMoves(expectedMoves, 11, {19, 27}, {0, model::Move::DOUBLE_PAWN_PUSH_FLAG});
    insertExpectedMoves(expectedMoves, 12, {20, 28}, {0, model::Move::DOUBLE_PAWN_PUSH_FLAG});
    insertExpectedMoves(expectedMoves, 13, {21, 29}, {0, model::Move::DOUBLE_PAWN_PUSH_FLAG});
    insertExpectedMoves(expectedMoves, 14, {22, 30}, {0, model::Move::DOUBLE_PAWN_PUSH_FLAG});
    insertExpectedMoves(expectedMoves, 15, {23, 31}, {0, model::Move::DOUBLE_PAWN_PUSH_FLAG});

    for (int i = 0; i < movelist.getMoveIndex(); i++) {
        auto found = expectedMoves.find(movelist.getMoveAt(i));
        ASSERT_TRUE(found != expectedMoves.end());
        expectedMoves.erase(found); // Remove found move from the set
    }

    ASSERT_TRUE(expectedMoves.empty());
}

TEST_F(MoveGeneratorPawnTest, genPawnMoves_startPosBlack_ShouldReturn16Moves) 
{
    io::Fen::setBoardFromFen(startingPos, bitboards, stateBitmasks, pieceMap);
    moveGenerator.genPawnMoves(false, movelist, 0);

    model::Movelist moves = getMoves();
    std::unordered_set<model::Move> expectedMoves;
    insertExpectedMoves(expectedMoves, 48, {40, 32}, {0, model::Move::DOUBLE_PAWN_PUSH_FLAG});
    insertExpectedMoves(expectedMoves, 49, {41, 33}, {0, model::Move::DOUBLE_PAWN_PUSH_FLAG});
    insertExpectedMoves(expectedMoves, 50, {42, 34}, {0, model::Move::DOUBLE_PAWN_PUSH_FLAG});
    insertExpectedMoves(expectedMoves, 51, {43, 35}, {0, model::Move::DOUBLE_PAWN_PUSH_FLAG});
    insertExpectedMoves(expectedMoves, 52, {44, 36}, {0, model::Move::DOUBLE_PAWN_PUSH_FLAG});
    insertExpectedMoves(expectedMoves, 53, {45, 37}, {0, model::Move::DOUBLE_PAWN_PUSH_FLAG});
    insertExpectedMoves(expectedMoves, 54, {46, 38}, {0, model::Move::DOUBLE_PAWN_PUSH_FLAG});
    insertExpectedMoves(expectedMoves, 55, {47, 39}, {0, model::Move::DOUBLE_PAWN_PUSH_FLAG});

    for (int i = 0; i < movelist.getMoveIndex(); i++) {
        auto found = expectedMoves.find(movelist.getMoveAt(i));
        ASSERT_TRUE(found != expectedMoves.end());
        expectedMoves.erase(found); // Remove found move from the set
    }

    ASSERT_TRUE(expectedMoves.empty());
}

TEST_F(MoveGeneratorPawnTest, genPawnMoves_fenOneWhite_ShouldReturn6Moves) 
{
    io::Fen::setBoardFromFen(fenOne, bitboards, stateBitmasks, pieceMap);
    moveGenerator.genPawnMoves(true, movelist, 0);

    model::Movelist moves = getMoves();
    std::unordered_set<model::Move> expectedMoves;
    insertExpectedMoves(expectedMoves, 28, {37, 36}, {1, 0});
    insertExpectedMoves(expectedMoves, 10, {18, 26}, {0, model::Move::DOUBLE_PAWN_PUSH_FLAG});
    insertExpectedMoves(expectedMoves, 8, {16, 24}, {0, model::Move::DOUBLE_PAWN_PUSH_FLAG});

    for (int i = 0; i < movelist.getMoveIndex(); i++) {
        auto found = expectedMoves.find(movelist.getMoveAt(i));
        ASSERT_TRUE(found != expectedMoves.end());
        expectedMoves.erase(found); // Remove found move from the set
    }

    ASSERT_TRUE(expectedMoves.empty());
}

TEST_F(MoveGeneratorPawnTest, genPawnMoves_fenOneBlack_ShouldReturnXMoves) 
{
    io::Fen::setBoardFromFen(fenOne, bitboards, stateBitmasks, pieceMap);
    moveGenerator.genPawnMoves(false, movelist, 0);

    model::Movelist moves = getMoves();
    std::unordered_set<model::Move> expectedMoves;
    insertExpectedMoves(expectedMoves, 40, {32}, {0});
    insertExpectedMoves(expectedMoves, 37, {28, 29}, {1, 0});
    insertExpectedMoves(expectedMoves, 23, {15}, {0});

    for (int i = 0; i < movelist.getMoveIndex(); i++) {
        auto found = expectedMoves.find(movelist.getMoveAt(i));
        ASSERT_TRUE(found != expectedMoves.end());
        expectedMoves.erase(found); // Remove found move from the set
    }

    ASSERT_TRUE(expectedMoves.empty());
}

TEST_F(MoveGeneratorPawnTest, genPawnMoves_fenEnPessantForWhite_ShouldReturn2Moves) 
{
    io::Fen::setBoardFromFen(fenEnPessantForWhiteTest, bitboards, stateBitmasks, pieceMap);
    // moveGenerator.getBoard().setEnPessantTargetAtIndex(43);
    moveGenerator.genPawnMoves(true, movelist, 0);

    model::Movelist moves = getMoves();
    std::unordered_set<model::Move> expectedMoves;
    insertExpectedMoves(expectedMoves, 36, {44, 43}, {0, model::Move::EP_CAPTURE_FLAG});

    for (int i = 0; i < movelist.getMoveIndex(); i++) {
        auto found = expectedMoves.find(movelist.getMoveAt(i));
        ASSERT_TRUE(found != expectedMoves.end());
        expectedMoves.erase(found); // Remove found move from the set
    }

    ASSERT_TRUE(expectedMoves.empty());
}

TEST_F(MoveGeneratorPawnTest, genPawnMoves_fenEnPessantForBlack_ShouldReturn2Moves) 
{
    io::Fen::setBoardFromFen(fenEnPessantForBlackTest, bitboards, stateBitmasks, pieceMap);
    // moveGenerator.getBoard().setEnPessantTargetAtIndex(23);
    moveGenerator.genPawnMoves(false, movelist, 0);

    model::Movelist moves = getMoves();
    std::unordered_set<model::Move> expectedMoves;
    insertExpectedMoves(expectedMoves, 30, {22, 23}, {0, model::Move::EP_CAPTURE_FLAG});

    for (int i = 0; i < movelist.getMoveIndex(); i++) {
        auto found = expectedMoves.find(movelist.getMoveAt(i));
        ASSERT_TRUE(found != expectedMoves.end());
        expectedMoves.erase(found); // Remove found move from the set
    }

    ASSERT_TRUE(expectedMoves.empty());
}

TEST_F(MoveGeneratorPawnTest, genPawnMoves_fenPromotionTestWhite_ShouldReturn12Moves) 
{
    io::Fen::setBoardFromFen(fenPromotionTest, bitboards, stateBitmasks, pieceMap);
    moveGenerator.genPawnMoves(true, movelist, 0);

    model::Movelist moves = getMoves();
    std::unordered_set<model::Move> expectedMoves;
    insertExpectedMoves(expectedMoves, 53, {61, 61, 61, 61}, {model::Move::KNIGHT_PROMO_FLAG, model::Move::BISHOP_PROMO_FLAG, model::Move::ROOK_PROMO_FLAG, model::Move::QUEEN_PROMO_FLAG});
    insertExpectedMoves(expectedMoves, 53, {60, 60, 60, 60}, {model::Move::KNIGHT_PROMO_CAPTURE_FLAG, model::Move::BISHOP_PROMO_CAPTURE_FLAG, model::Move::ROOK_PROMO_CAPTURE_FLAG, model::Move::QUEEN_PROMO_CAPTURE_FLAG});
    insertExpectedMoves(expectedMoves, 49, {57, 57, 57, 57}, {model::Move::KNIGHT_PROMO_FLAG, model::Move::BISHOP_PROMO_FLAG, model::Move::ROOK_PROMO_FLAG, model::Move::QUEEN_PROMO_FLAG});

    for (int i = 0; i < movelist.getMoveIndex(); i++) {
        auto found = expectedMoves.find(movelist.getMoveAt(i));
        ASSERT_TRUE(found != expectedMoves.end());
        expectedMoves.erase(found); // Remove found move from the set
    }

    ASSERT_TRUE(expectedMoves.empty());
}

TEST_F(MoveGeneratorPawnTest, genPawnMoves_fenPromotionTestBlack_ShouldReturn12Moves) 
{
    io::Fen::setBoardFromFen(fenPromotionTest, bitboards, stateBitmasks, pieceMap);
    moveGenerator.genPawnMoves(false, movelist, 0);

    model::Movelist moves = getMoves();
    std::unordered_set<model::Move> expectedMoves;
    insertExpectedMoves(expectedMoves, 8, {0, 0, 0, 0}, {model::Move::KNIGHT_PROMO_FLAG, model::Move::BISHOP_PROMO_FLAG, model::Move::ROOK_PROMO_FLAG, model::Move::QUEEN_PROMO_FLAG});
    insertExpectedMoves(expectedMoves, 14, {5, 5, 5, 5}, {model::Move::KNIGHT_PROMO_CAPTURE_FLAG, model::Move::BISHOP_PROMO_CAPTURE_FLAG, model::Move::ROOK_PROMO_CAPTURE_FLAG, model::Move::QUEEN_PROMO_CAPTURE_FLAG});
    insertExpectedMoves(expectedMoves, 14, {6, 6, 6, 6}, {model::Move::KNIGHT_PROMO_FLAG, model::Move::BISHOP_PROMO_FLAG, model::Move::ROOK_PROMO_FLAG, model::Move::QUEEN_PROMO_FLAG});

    for (int i = 0; i < movelist.getMoveIndex(); i++) {
        auto found = expectedMoves.find(movelist.getMoveAt(i));
        ASSERT_TRUE(found != expectedMoves.end());
        expectedMoves.erase(found); // Remove found move from the set
    }

    ASSERT_TRUE(expectedMoves.empty());
}

TEST_F(MoveGeneratorPawnTest, genPawnMoves_fenBuggedPawnTestWhite_ShouldNotReturnh6) 
{
    io::Fen::setBoardFromFen(fenBuggedPawnTest, bitboards, stateBitmasks, pieceMap);
    moveGenerator.genPawnMoves(false, movelist, 0);

    model::Movelist moves = getMoves();

    for (int i = 0; i < movelist.getMoveIndex(); i++) {
        ASSERT_NE(movelist.getMoveAt(i), model::Move(46, 38, model::Move::QUITE_FLAG));
    }
}

} // namespace logic
```


--------------------

# FILE: tests/logic/movegen/QueenGenerator_test.cpp

```cpp
#include <gtest/gtest.h>

#include "BaseGenerator_test.h"

namespace logic {

class MoveGeneratorQueenTest : public BaseGenerator 
{
protected:
    std::string fenOne;

    void SetUp() override {
        BaseGenerator::SetUp();
        fenOne = "5k2/2P5/5q2/b4N2/1q6/2Q2Q2/8/8";
    }
};

TEST_F(MoveGeneratorQueenTest, genQueenMoves_fenOneBlack_ShouldReturnXMoves) 
{
    io::Fen::setBoardFromFen(fenOne, bitboards, stateBitmasks, pieceMap);
    moveGenerator.genQueenMoves(false, movelist);

    model::Movelist moves = getMoves();
    std::unordered_set<model::Move> expectedMoves;
    insertExpectedMoves(expectedMoves, 42, {50, 49, 56, 41, 40, 33, 24, 34, 35, 28, 21, 43, 44, 45, 46, 47, 51, 60, 50}, {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0});
    insertExpectedMoves(expectedMoves, 30, {38, 46, 54, 62, 37, 44, 51, 29, 28, 27, 26, 25, 24, 21, 22, 14, 6, 23, 31}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0});

    for (int i = 0; i < movelist.getMoveIndex(); i++) {
        auto found = expectedMoves.find(movelist.getMoveAt(i));
        ASSERT_TRUE(found != expectedMoves.end());
        expectedMoves.erase(found); // Remove found move from the set
    }

    ASSERT_TRUE(expectedMoves.empty());
}

TEST_F(MoveGeneratorQueenTest, genQueenMoves_fenOneWhite_ShouldReturnXMoves) 
{
    io::Fen::setBoardFromFen(fenOne, bitboards, stateBitmasks, pieceMap);
    moveGenerator.genQueenMoves(true, movelist);

    model::Movelist moves = getMoves();
    std::unordered_set<model::Move> expectedMoves;
    insertExpectedMoves(expectedMoves, 21, {29, 37, 45, 28, 35, 42, 20, 19, 12, 3, 13, 5, 14, 7, 22, 23, 30}, {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1});
    insertExpectedMoves(expectedMoves, 18, {26, 25, 32, 17, 16, 9, 0, 10, 2, 11, 4, 19, 20, 27, 36, 45, 54, 63}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});

    for (int i = 0; i < movelist.getMoveIndex(); i++) {
        auto found = expectedMoves.find(movelist.getMoveAt(i));
        ASSERT_TRUE(found != expectedMoves.end());
        expectedMoves.erase(found); // Remove found move from the set
    }

    ASSERT_TRUE(expectedMoves.empty());
}

TEST_F(MoveGeneratorQueenTest, genQueenMoves_StartPosWhite_ShouldReturn0Moves) 
{
    io::Fen::setBoardFromFen(startingPos, bitboards, stateBitmasks, pieceMap);
    moveGenerator.genQueenMoves(true, movelist);

    ASSERT_EQ(movelist.getMoveIndex(), 0);
}

TEST_F(MoveGeneratorQueenTest, genQueenMoves_StartPosBlack_ShouldReturn0Moves) 
{
    io::Fen::setBoardFromFen(startingPos, bitboards, stateBitmasks, pieceMap);
    moveGenerator.genQueenMoves(false, movelist);

    ASSERT_EQ(movelist.getMoveIndex(), 0);
}

} // namespace logic
```


--------------------

# FILE: tests/logic/movegen/RookGenerator_test.cpp

```cpp
#include <gtest/gtest.h>

#include "BaseGenerator_test.h"

namespace logic {

class MoveGeneratorRookTest : public BaseGenerator 
{
    protected:
        std::string fenOne;
        std::string fenTwo;

        void SetUp() override {
            BaseGenerator::SetUp();
            fenOne = "8/2P5/8/8/8/b1R5/4q3/8";
            fenTwo = "7r/8/8/1P6/8/1R1p3R/8/8";
        }
};

TEST_F(MoveGeneratorRookTest, genRookMoves_fenOneWhite_ShouldReturn12Moves) 
{
    io::Fen::setBoardFromFen(fenOne, bitboards, stateBitmasks, pieceMap);
    moveGenerator.genRookMoves(true, movelist);

    model::Movelist moves = getMoves();
    std::unordered_set<model::Move> expectedMoves;
    insertExpectedMoves(expectedMoves, 21, {29, 37, 45, 16, 17, 18, 19, 20, 5, 13, 23, 22}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}); 

    for (int i = 0; i < movelist.getMoveIndex(); i++) {
        auto found = expectedMoves.find(movelist.getMoveAt(i));
        ASSERT_TRUE(found != expectedMoves.end());
        expectedMoves.erase(found); // Remove found move from the set
    }

    ASSERT_TRUE(expectedMoves.empty());
}

TEST_F(MoveGeneratorRookTest, genRookMoves_fenOneBlack_ShouldReturn0Moves) 
{
    io::Fen::setBoardFromFen(fenOne, bitboards, stateBitmasks, pieceMap);
    moveGenerator.genRookMoves(false, movelist);

    ASSERT_EQ(movelist.getMoveIndex(), 0);
}

TEST_F(MoveGeneratorRookTest, genRookMoves_startingPosWhite_ShouldReturn0Moves) 
{
    io::Fen::setBoardFromFen(startingPos, bitboards, stateBitmasks, pieceMap);
    moveGenerator.genRookMoves(true, movelist);

    ASSERT_EQ(movelist.getMoveIndex(), 0);
}

TEST_F(MoveGeneratorRookTest, genRookMoves_startingPosBlack_ShouldReturn0Moves) 
{
    io::Fen::setBoardFromFen(startingPos, bitboards, stateBitmasks, pieceMap);
    moveGenerator.genRookMoves(false, movelist);

    ASSERT_EQ(movelist.getMoveIndex(), 0);
}

TEST_F(MoveGeneratorRookTest, genRookMoves_fenTwoWhite_ShouldReturn17Moves) 
{
    io::Fen::setBoardFromFen(fenTwo, bitboards, stateBitmasks, pieceMap);
    moveGenerator.genRookMoves(true, movelist);

    model::Movelist moves = getMoves();
    std::unordered_set<model::Move> expectedMoves;
    insertExpectedMoves(expectedMoves, 16, {24, 32, 40, 48, 56, 8, 0, 17, 18, 19, 20}, {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1});
    insertExpectedMoves(expectedMoves, 22, {30, 21, 20, 14, 6, 23}, {0, 0, 1, 0, 0, 0});

    for (int i = 0; i < movelist.getMoveIndex(); i++) {
        auto found = expectedMoves.find(movelist.getMoveAt(i));
        ASSERT_TRUE(found != expectedMoves.end());
        expectedMoves.erase(found); // Remove found move from the set
    }

    ASSERT_TRUE(expectedMoves.empty());        
}

TEST_F(MoveGeneratorRookTest, genRookMoves_fenTwoBlack_ShouldReturn12Moves) 
{
    io::Fen::setBoardFromFen(fenTwo, bitboards, stateBitmasks, pieceMap);
    moveGenerator.genRookMoves(false, movelist);

    model::Movelist moves = getMoves();
    std::unordered_set<model::Move> expectedMoves;
    insertExpectedMoves(expectedMoves, 56, {48, 40, 32, 24, 16, 57, 58, 59, 60, 61, 62, 63}, {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0});

    for (int i = 0; i < movelist.getMoveIndex(); i++) {
        auto found = expectedMoves.find(movelist.getMoveAt(i));
        ASSERT_TRUE(found != expectedMoves.end());
        expectedMoves.erase(found); // Remove found move from the set
    }

    ASSERT_TRUE(expectedMoves.empty());   
}

} // namespace logic
```


--------------------

# FILE: tests/logic/movegen/utils/BitBasics_test.cpp

```cpp
#include <gtest/gtest.h>

#include "logic/movegen/utils/BitBasics.h"

namespace logic {

class BitBasicsTest : public ::testing::Test {
protected:
    bitmask a;

    void SetUp() override {
        a = 0x000000000000FF00ULL;
    }
};

// BitBasics::getBit tests
TEST_F(BitBasicsTest, getBit_A_Pos0_ShouldReturnFalse) 
{
    bool expected = 0;
    bool actual = BitBasics::getBit(a, 0);
    ASSERT_EQ(expected, actual);
}

TEST_F(BitBasicsTest, getBit_A_Pos9_ShouldReturnTrue)
{
    bool expected = 1;
    bool actual = BitBasics::getBit(a, 9);
    ASSERT_EQ(expected, actual);
}

TEST_F(BitBasicsTest, getBit_A_Pos15_ShouldReturnTrue) 
{
    bool expected = 1;
    bool actual = BitBasics::getBit(a, 15);
    ASSERT_EQ(expected, actual);
}

TEST_F(BitBasicsTest, getBit_A_Pos16_ShouldReturnFalse) 
{
    bool expected = 0;
    bool actual = BitBasics::getBit(a, 16);
    ASSERT_EQ(expected, actual);
}

// BitBasics::indexOfLSB tests
TEST_F(BitBasicsTest, indexOfLSB_A_ShouldReturn8) 
{
    int expected = 8;
    int actual = BitBasics::indexOfLSB(a);
    ASSERT_EQ(expected, actual);
}

// BitBasics::indexOfMSB tests
TEST_F(BitBasicsTest, indexOfMSB_A_ShouldReturn15) 
{
    int expected = 15;
    int actual = BitBasics::indexOfMSB(a);
    ASSERT_EQ(expected, actual);
}

TEST_F(BitBasicsTest, indexOfMSB_0_ShouldReturnNeg1) 
{
    int expected = -1;
    int actual = BitBasics::indexOfMSB(0ULL);
    ASSERT_EQ(expected, actual);
}

} // namespace logic
```


--------------------

# FILE: tests/logic/movegen/utils/CMakeLists.txt

```
target_sources(movegen_tests PRIVATE
    BitBasics_test.cpp
    ChessUtils_test.cpp
)
```


--------------------

# FILE: tests/logic/movegen/utils/ChessUtils_test.cpp

```cpp
#include <gtest/gtest.h>

#include "logic/movegen/utils/ChessUtils.h"
#include "logic/movegen/utils/BitBasics.h"

namespace logic {

class ChessUtilsTest : public ::testing::Test 
{
protected:
    bitmask a;
    bitmask b;
    bitmask c;
    bitmask whitePieces;
    bitmask blackPieces;
    void SetUp() override {
        a = 0x0000000000FF0501;
        b = 0x0000000000000000;
        c = 0xFFFFFFFFFFFFFFFF;
        whitePieces = 0x000000000000FFFFULL;
        blackPieces = 0xFFFF000000000000ULL;
    }
};

TEST_F(ChessUtilsTest, getFileMask_File7_ShouldReturn0x8080808080808080)
{
    bitmask expected = 0x8080808080808080ULL;
    bitmask actual = ChessUtils::getFileMask(7);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, getFileMask_File6_ShouldReturn0x4040404040404040)
{
    bitmask expected = 0x4040404040404040ULL;
    bitmask actual = ChessUtils::getFileMask(6);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, getFileMask_File5_ShouldReturn0x2020202020202020)
{
    bitmask expected = 0x2020202020202020ULL;
    bitmask actual = ChessUtils::getFileMask(5);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, getFileMask_File4_ShouldReturn0x1010101010101010)
{
    bitmask expected = 0x1010101010101010ULL;
    bitmask actual = ChessUtils::getFileMask(4);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, getFileMask_File3_ShouldReturn0x0808080808080808)
{
    bitmask expected = 0x0808080808080808ULL;
    bitmask actual = ChessUtils::getFileMask(3);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, getFileMask_File2_ShouldReturn0x0404040404040404)
{
    bitmask expected = 0x0404040404040404ULL;
    bitmask actual = ChessUtils::getFileMask(2);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, getFileMask_File1_ShouldReturn0x0202020202020202)
{
    bitmask expected = 0x0202020202020202ULL;
    bitmask actual = ChessUtils::getFileMask(1);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, getFileMask_File0_ShouldReturn0x0101010101010101)
{
    bitmask expected = 0x0101010101010101ULL;
    bitmask actual = ChessUtils::getFileMask(0);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, getRankMask_Rank0_ShouldReturn0x00000000000000FF)
{
    bitmask expected = 0x00000000000000FFULL;
    bitmask actual = ChessUtils::getRankMask(0);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, getRankMask_Rank1_ShouldReturn0x000000000000FF00)
{
    bitmask expected = 0x000000000000FF00ULL;
    bitmask actual = ChessUtils::getRankMask(1);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, getRankMask_Rank2_ShouldReturn0x0000000000FF0000)
{
    bitmask expected = 0x0000000000FF0000ULL;
    bitmask actual = ChessUtils::getRankMask(2);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, getRankMask_Rank3_ShouldReturn0x00000000FF000000)
{
    bitmask expected = 0x00000000FF000000ULL;
    bitmask actual = ChessUtils::getRankMask(3);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, getRankMask_Rank4_ShouldReturn0x000000FF00000000)
{
    bitmask expected = 0x000000FF00000000ULL;
    bitmask actual = ChessUtils::getRankMask(4);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, getRankMask_Rank5_ShouldReturn0x0000FF0000000000)
{
    bitmask expected = 0x0000FF0000000000ULL;
    bitmask actual = ChessUtils::getRankMask(5);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, getRankMask_Rank6_ShouldReturn0x00FF000000000000)
{
    bitmask expected = 0x00FF000000000000ULL;
    bitmask actual = ChessUtils::getRankMask(6);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, getRankMask_Rank7_ShouldReturn0xFF00000000000000)
{
    bitmask expected = 0xFF00000000000000ULL;
    bitmask actual = ChessUtils::getRankMask(7);
    ASSERT_EQ(expected, actual);
}

    // ChessUtils::rankFromBitIndex tests
TEST_F(ChessUtilsTest, rankFromBitIndex_Pos0_ShouldReturn0)
{
    int expected = 0;
    int actual = ChessUtils::rankFromBitIndex(0);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, rankFromBitIndex_Pos7_ShouldReturn0)
{
    int expected = 0;
    int actual = ChessUtils::rankFromBitIndex(7);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, rankFromBitIndex_Pos8_ShouldReturn1)
{
    int expected = 1;
    int actual = ChessUtils::rankFromBitIndex(8);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, rankFromBitIndex_Pos15_ShouldReturn1)
{
    int expected = 1;
    int actual = ChessUtils::rankFromBitIndex(15);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, rankFromBitIndex_Pos63_ShouldReturn7)
{
    int expected = 7;
    int actual = ChessUtils::rankFromBitIndex(63);
    ASSERT_EQ(expected, actual);
}

// ChessUtils::fileFromBitIndex tests
TEST_F(ChessUtilsTest, fileFromBitIndex_Pos0_ShouldReturn0)
{
    int expected = 0;
    int actual = ChessUtils::fileFromBitIndex(0);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, fileFromBitIndex_Pos7_ShouldReturn7)
{
    int expected = 7;
    int actual = ChessUtils::fileFromBitIndex(7);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, fileFromBitIndex_Pos8_ShouldReturn0)
{
    int expected = 0;
    int actual = ChessUtils::fileFromBitIndex(8);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, fileFromBitIndex_Pos15_ShouldReturn7)
{
    int expected = 7;
    int actual = ChessUtils::fileFromBitIndex(15);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, fileFromBitIndex_Pos63_ShouldReturn7)
{
    int expected = 7;
    int actual = ChessUtils::fileFromBitIndex(63);
    ASSERT_EQ(expected, actual);
}


TEST_F(ChessUtilsTest, getBitIndices_a_ShouldReturn0_8_10_16to23)
{
    std::vector<int> expected = {0, 8, 10, 16, 17, 18, 19, 20, 21, 22, 23};
    std::vector<int> actual;
    BitBasics::getBitIndices(actual, a);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, getBitIndices_b_ShouldReturnEmptyVector)
{
    std::vector<int> expected = {};
    std::vector<int> actual;
    BitBasics::getBitIndices(actual, b);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, getBitIndices_c_ShouldReturn0to63)
{
    std::vector<int> expected = {};

    for (int i = 0; i < 64; i++)
{
        expected.push_back(i);
    }
    
    std::vector<int> actual;
    BitBasics::getBitIndices(actual, c);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, getOccupiedSquaresBitmask_StartingPosition_ShouldReturn0xFFFF00000000FFFF)
{
    bitmask expected = 0xFFFF00000000FFFFULL;
    bitmask actual = ChessUtils::getOccupiedSquaresBitmask(whitePieces, blackPieces);
    ASSERT_EQ(expected, actual);
}

TEST_F(ChessUtilsTest, getEmptySquaresBitmask_StartingPosition_ShouldReturn0x0000FFFFFFFF0000)
{
    bitmask expected = 0x0000FFFFFFFF0000ULL;
    bitmask actual = ChessUtils::getEmptySquaresBitmask(whitePieces, blackPieces);
    ASSERT_EQ(expected, actual);
}

} // namespace logic
```


--------------------

# FILE: tests/logic/movegen/bitmasks/CMakeLists.txt

```
target_sources(movegen_tests PRIVATE
    RayBitmasks_test.cpp
    KnightBitmasks_test.cpp
    KingBitmasks_test.cpp
    PawnBitmasks_test.cpp
)
```


--------------------

# FILE: tests/logic/movegen/bitmasks/KingBitmasks_test.cpp

```cpp
#include <gtest/gtest.h>

#include "logic/movegen/bitmasks/KingBitmasks.h"

namespace logic {

class Kingbitboards : public ::testing::Test 
{
protected:
    void SetUp() override {

    }
};

TEST_F(Kingbitboards, getKingBitmask_0_ShouldReturn0x0000000000000302) 
{
    bitboard expected = 0x0000000000000302ULL;
    bitboard actual = KingBitmasks::getKingBitmask(0);
    ASSERT_EQ(expected, actual);
}

TEST_F(Kingbitboards, getKingBitmask_7_ShouldReturn0x000000000000C040) 
{
    bitboard expected = 0x000000000000C040ULL;
    bitboard actual = KingBitmasks::getKingBitmask(7);
    ASSERT_EQ(expected, actual);
}

TEST_F(Kingbitboards, getKingBitmask_9_ShouldReturn0x0000000000070507) 
{
    bitboard expected = 0x0000000000070507ULL;
    bitboard actual = KingBitmasks::getKingBitmask(9);
    ASSERT_EQ(expected, actual);
}

TEST_F(Kingbitboards, getKingBitmask_56_ShouldReturn0x0203000000000000) 
{
    bitboard expected = 0x0203000000000000ULL;
    bitboard actual = KingBitmasks::getKingBitmask(56);
    ASSERT_EQ(expected, actual);
}

TEST_F(Kingbitboards, getKingBitmask_63_ShouldReturn0x40C0000000000000) 
{
    bitboard expected = 0x40C0000000000000ULL;
    bitboard actual = KingBitmasks::getKingBitmask(63);
    ASSERT_EQ(expected, actual);
}

} // namespace logic
```


--------------------

# FILE: tests/logic/movegen/bitmasks/KnightBitmasks_test.cpp

```cpp
#include <gtest/gtest.h>

#include "logic/movegen/bitmasks/KnightBitmasks.h"

namespace logic {

class KnightBitmasksTest : public ::testing::Test 
{
protected:

    void SetUp() override {

    }
};

TEST_F(KnightBitmasksTest, getKnightBitmask_0_ShouldReturn0x0000000000020400) 
{
    bitmask expected = 0x0000000000020400ULL;
    bitmask actual = KnightBitmasks::getKnightBitmask(0);
    ASSERT_EQ(expected, actual);
}

TEST_F(KnightBitmasksTest, getKnightBitmask_1_ShouldReturn0x0000000000050800) 
{
    bitmask expected = 0x0000000000050800ULL;
    bitmask actual = KnightBitmasks::getKnightBitmask(1);
    ASSERT_EQ(expected, actual);
}

TEST_F(KnightBitmasksTest, getKnightBitmask_2_ShouldReturn0x00000000000A1100) 
{
    bitmask expected = 0x00000000000A1100ULL;
    bitmask actual = KnightBitmasks::getKnightBitmask(2);
    ASSERT_EQ(expected, actual);
}

TEST_F(KnightBitmasksTest, getKnightBitmask_15_ShouldReturn0x0000000040200020) 
{
    bitmask expected = 0x0000000040200020ULL;
    bitmask actual = KnightBitmasks::getKnightBitmask(15);
    ASSERT_EQ(expected, actual);
}

TEST_F(KnightBitmasksTest, getKnightBitmask_20_ShouldReturn0x0000002844004428) 
{
    bitmask expected = 0x0000002844004428ULL;
    bitmask actual = KnightBitmasks::getKnightBitmask(20);
    ASSERT_EQ(expected, actual);
}

TEST_F(KnightBitmasksTest, getKnightBitmask_49_ShouldReturn0x0800080500000000) 
{
    bitmask expected = 0x0800080500000000ULL;
    bitmask actual = KnightBitmasks::getKnightBitmask(49);
    ASSERT_EQ(expected, actual);
}

TEST_F(KnightBitmasksTest, getKnightBitmask_60_ShouldReturn0x0044280000000000) 
{
    bitmask expected = 0x0044280000000000ULL;
    bitmask actual = KnightBitmasks::getKnightBitmask(60);
    ASSERT_EQ(expected, actual);
}

TEST_F(KnightBitmasksTest, getKnightBitmask_63_ShouldReturn0x20400000000000)
{
    bitmask expected = 0x20400000000000ULL;
    bitmask actual = KnightBitmasks::getKnightBitmask(63);
    ASSERT_EQ(expected, actual);
}

} // namespace logic
```


--------------------

# FILE: tests/logic/movegen/bitmasks/PawnBitmasks_test.cpp

```cpp
#include <gtest/gtest.h>

#include "logic/movegen/bitmasks/PawnBitmasks.h"

namespace logic {

class PawnBitmasksTest : public ::testing::Test 
{
protected:
    void SetUp() override {

    }
};

TEST_F(PawnBitmasksTest, getCapturePawnMovesBitmask_8asWhite_ShouldReturn0x0000000001030000) {
    bitmask expectedCapture = 0x0000000000020000ULL;
    bitmask expectedStraight = 0x0000000001010000ULL;
    bitmask actualCapture = PawnBitmasks::getCapturePawnMovesBitmask(8, true);
    bitmask actualStraight = PawnBitmasks::getStraightPawnMovesBitmask(8, true);
    ASSERT_EQ(expectedCapture, actualCapture);
    ASSERT_EQ(expectedStraight, actualStraight);
}

TEST_F(PawnBitmasksTest, getCapturePawnMovesBitmask_12asWhite_ShouldReturn0x0000000010380000) {
    bitmask expectedStraight = 0x0000000010100000ULL;
    bitmask expectedCapture = 0x0000000000280000ULL;
    bitmask actualCapture = PawnBitmasks::getCapturePawnMovesBitmask(12, true);
    bitmask actualStraight = PawnBitmasks::getStraightPawnMovesBitmask(12, true);
    ASSERT_EQ(expectedCapture, actualCapture);
    ASSERT_EQ(expectedStraight, actualStraight);
}

TEST_F(PawnBitmasksTest, getCapturePawnMovesBitmask_15asWhite_ShouldReturn0x0000000080C00000) {
    bitmask expectedCapture = 0x0000000000400000ULL;
    bitmask expectedStraight = 0x0000000080800000ULL;
    bitmask actualCapture = PawnBitmasks::getCapturePawnMovesBitmask(15, true);
    bitmask actualStraight = PawnBitmasks::getStraightPawnMovesBitmask(15, true);
    ASSERT_EQ(expectedCapture, actualCapture);
    ASSERT_EQ(expectedStraight, actualStraight);
}

TEST_F(PawnBitmasksTest, getCapturePawnMovesBitmask_20asWhite_ShouldReturn0x000000038000000) {
    bitmask expectedCapture = 0x0000000028000000ULL;
    bitmask expectedStraight = 0x0000000010000000ULL;
    bitmask actualCapture = PawnBitmasks::getCapturePawnMovesBitmask(20, true);
    bitmask actualStraight = PawnBitmasks::getStraightPawnMovesBitmask(20, true);
    ASSERT_EQ(expectedCapture, actualCapture);
    ASSERT_EQ(expectedStraight, actualStraight);
}

TEST_F(PawnBitmasksTest, getCapturePawnMovesBitmask_48asBlack_ShouldReturn0x000030100000000) {
    bitmask expectedCapture = 0x0000020000000000ULL;
    bitmask expectedStraight = 0x0000010100000000ULL;
    bitmask actualCapture = PawnBitmasks::getCapturePawnMovesBitmask(48, false);
    bitmask actualStraight = PawnBitmasks::getStraightPawnMovesBitmask(48, false);
    ASSERT_EQ(expectedCapture, actualCapture);
    ASSERT_EQ(expectedStraight, actualStraight);
}

TEST_F(PawnBitmasksTest, getCapturePawnMovesBitmask_52asBlack_ShouldReturn0x0000381000000000) {
    bitmask expectedCapture = 0x0000280000000000ULL;
    bitmask expectedStraight = 0x0000101000000000ULL;
    bitmask actualCapture = PawnBitmasks::getCapturePawnMovesBitmask(52, false);
    bitmask actualStraight = PawnBitmasks::getStraightPawnMovesBitmask(52, false);
    ASSERT_EQ(expectedCapture, actualCapture);
    ASSERT_EQ(expectedStraight, actualStraight);
}

TEST_F(PawnBitmasksTest, getCapturePawnMovesBitmask_55asBlack_ShouldReturn0x0000C08000000000) {
    bitmask expectedCapture = 0x0000400000000000ULL;
    bitmask expectedStraight = 0x0000808000000000ULL;
    bitmask actualCapture = PawnBitmasks::getCapturePawnMovesBitmask(55, false);
    bitmask actualStraight = PawnBitmasks::getStraightPawnMovesBitmask(55, false);
    ASSERT_EQ(expectedCapture, actualCapture);
    ASSERT_EQ(expectedStraight, actualStraight);
}

TEST_F(PawnBitmasksTest, getCapturePawnMovesBitmask_44asBlack_ShouldReturn0x0000003800000000) {
    bitmask expectedCapture = 0x0000002800000000ULL;
    bitmask expectedStraight = 0x0000001000000000ULL;
    bitmask actualCapture = PawnBitmasks::getCapturePawnMovesBitmask(44, false);
    bitmask actualStraight = PawnBitmasks::getStraightPawnMovesBitmask(44, false);
    ASSERT_EQ(expectedCapture, actualCapture);
    ASSERT_EQ(expectedStraight, actualStraight);
}
    
} // namespace logic

```


--------------------

# FILE: tests/logic/movegen/bitmasks/RayBitmasks_test.cpp

```cpp
#include <gtest/gtest.h>

#include "logic/movegen/bitmasks/RayBitmasks.h"

namespace logic {

class RayBitmasks_test : public ::testing::Test 
{
protected:
    int indOne;
    int indTwo;
    int indThree;
    int indFour;
    int indFive;
    int indSix;
    bitmask whitePieces;
    bitmask blackPieces;

    void SetUp() override {
        indOne = 0;
        indTwo = 7;
        indThree = 28;
        indFour = 32;
        indFive = 56;
        indSix = 63;
        whitePieces = 0x000000000000FFFFULL;
        blackPieces = 0xFFFF000000000000ULL;
    }
};

// RayBitmasks::getNorthRay tests
TEST_F(RayBitmasks_test, getNorthRay_Pos0_ShouldReturn0x0101010101010100) 
{
    bitmask expected = 0x0101010101010100ULL;
    bitmask actual = RayBitmasks::getNorthRay(indOne);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getNorthRay_Pos7_ShouldReturn0x8080808080808000) 
{
    bitmask expected = 0x8080808080808000ULL;
    bitmask actual = RayBitmasks::getNorthRay(indTwo);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getNorthRay_Pos28_ShouldReturn0x1010101000000000) 
{
    bitmask expected = 0x1010101000000000ULL;
    bitmask actual = RayBitmasks::getNorthRay(indThree);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getNorthRay_Pos32_ShouldReturn0x0101010000000000) 
{
    bitmask expected = 0x0101010000000000ULL;
    bitmask actual = RayBitmasks::getNorthRay(indFour);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getNorthRay_Pos56_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = RayBitmasks::getNorthRay(indFive);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getNorthRay_Pos63_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = RayBitmasks::getNorthRay(indSix);
    ASSERT_EQ(expected, actual);
}

// RayBitmasks::getNorthEastRay tests

TEST_F(RayBitmasks_test, getNorthEastRay_Pos0_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = RayBitmasks::getNorthEastRay(indOne);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getNorthEastRay_Pos7_ShouldReturn0x0102040810204000) 
{
    bitmask expected = 0x0102040810204000ULL;
    bitmask actual = RayBitmasks::getNorthEastRay(indTwo);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getNorthEastRay_Pos28_ShouldReturn0x0102040800000000) 
{
    bitmask expected = 0x0102040800000000ULL;
    bitmask actual = RayBitmasks::getNorthEastRay(indThree);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getNorthEastRay_Pos32_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000;
    bitmask actual = RayBitmasks::getNorthEastRay(indFour);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getNorthEastRay_Pos56_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000;
    bitmask actual = RayBitmasks::getNorthEastRay(indFive);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getNorthEastRay_Pos63_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000;
    bitmask actual = RayBitmasks::getNorthEastRay(indSix);
    ASSERT_EQ(expected, actual);
}

// RayBitmasks::getEastRay tests
TEST_F(RayBitmasks_test, getEastRay_Pos0_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = RayBitmasks::getEastRay(indOne);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getEastRay_Pos7_ShouldReturn0x000000000000007F) 
{
    bitmask expected = 0x000000000000007FULL;
    bitmask actual = RayBitmasks::getEastRay(indTwo);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getEastRay_Pos28_ShouldReturn0x000000000F000000) 
{
    bitmask expected = 0x000000000F000000ULL;
    bitmask actual = RayBitmasks::getEastRay(indThree);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getEastRay_Pos32_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = RayBitmasks::getEastRay(indFour);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getEastRay_Pos56_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = RayBitmasks::getEastRay(indFive);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getEastRay_Pos63_ShouldReturn0x7F000000000000000) 
{
    bitmask expected = 0x7F00000000000000ULL;
    bitmask actual = RayBitmasks::getEastRay(indSix);
    ASSERT_EQ(expected, actual);
}

// RayBitmasks::getSouthEastRay tests
TEST_F(RayBitmasks_test, getSouthEastRay_Pos0_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = RayBitmasks::getSouthEastRay(indOne);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthEastRay_Pos7_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = RayBitmasks::getSouthEastRay(indTwo);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthEastRay_Pos28_ShouldReturn0x0000000000080402) 
{
    bitmask expected = 0x0000000000080402ULL;
    bitmask actual = RayBitmasks::getSouthEastRay(indThree);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthEastRay_Pos32_ShouldReturn0x0000000000000000)
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = RayBitmasks::getSouthEastRay(indFour);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthEastRay_Pos56_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = RayBitmasks::getSouthEastRay(indFive);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthEastRay_Pos63_ShouldReturn0x0040201008040201) 
{
    bitmask expected = 0x0040201008040201ULL;
    bitmask actual = RayBitmasks::getSouthEastRay(indSix);
    ASSERT_EQ(expected, actual);
}

// RayBitmasks::getSouthRay tests
TEST_F(RayBitmasks_test, getSouthRay_Pos0_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = RayBitmasks::getSouthRay(indOne);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthRay_Pos7_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = RayBitmasks::getSouthRay(indTwo);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthRay_Pos28_ShouldReturn0x0000000000101010) 
{
    bitmask expected = 0x0000000000101010ULL;
    bitmask actual = RayBitmasks::getSouthRay(indThree);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthRay_Pos32_ShouldReturn0x0000000001010101) 
{
    bitmask expected = 0x0000000001010101ULL;
    bitmask actual = RayBitmasks::getSouthRay(indFour);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthRay_Pos56_ShouldReturn0x001010101010101) 
{
    bitmask expected = 0x001010101010101ULL;
    bitmask actual = RayBitmasks::getSouthRay(indFive);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthRay_Pos63_ShouldReturn0x0080808080808080) 
{
    bitmask expected = 0x0080808080808080ULL;
    bitmask actual = RayBitmasks::getSouthRay(indSix);
    ASSERT_EQ(expected, actual);
}

// RayBitmasks::getSouthWestRay tests
TEST_F(RayBitmasks_test, getSouthWestRay_Pos0_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = RayBitmasks::getSouthWestRay(indOne);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthWestRay_Pos7_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = RayBitmasks::getSouthWestRay(indTwo);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthWestRay_Pos28_ShouldReturn0x0000000000204080) 
{
    bitmask expected = 0x0000000000204080ULL;
    bitmask actual = RayBitmasks::getSouthWestRay(indThree);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthWestRay_Pos32_ShouldReturn0x0000000002040810) 
{
    bitmask expected = 0x0000000002040810ULL;
    bitmask actual = RayBitmasks::getSouthWestRay(indFour);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthWestRay_Pos56_ShouldReturn0x002040810204080) 
{
    bitmask expected = 0x002040810204080ULL;
    bitmask actual = RayBitmasks::getSouthWestRay(indFive);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getSouthWestRay_Pos63_ShouldReturn0x0000000000000000)
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = RayBitmasks::getSouthWestRay(indSix);
    ASSERT_EQ(expected, actual);
}

// RayBitmasks::getWestRay tests

TEST_F(RayBitmasks_test, getWestRay_Pos0_ShouldReturn0x00000000000000FE) 
{
    bitmask expected = 0x00000000000000FEULL;
    bitmask actual = RayBitmasks::getWestRay(indOne);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getWestRay_Pos7_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = RayBitmasks::getWestRay(indTwo);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getWestRay_Pos28_ShouldReturn0x00000000E0000000) 
{
    bitmask expected = 0x00000000E0000000ULL;
    bitmask actual = RayBitmasks::getWestRay(indThree);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getWestRay_Pos32_ShouldReturn0x000000FE00000000) 
{
    bitmask expected = 0x000000FE00000000ULL;
    bitmask actual = RayBitmasks::getWestRay(indFour);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getWestRay_Pos56_ShouldReturn0xFE00000000000000) 
{
    bitmask expected = 0xFE00000000000000ULL;
    bitmask actual = RayBitmasks::getWestRay(indFive);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getWestRay_Pos63_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = RayBitmasks::getWestRay(indSix);
    ASSERT_EQ(expected, actual);
}

// RayBitmasks::getNorthWestRay tests

TEST_F(RayBitmasks_test, getNorthWestRay_Pos0_ShouldReturn0x8040201008040200) 
{
    bitmask expected = 0x8040201008040200ULL;
    bitmask actual = RayBitmasks::getNorthWestRay(indOne);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getNorthWestRay_Pos7_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = RayBitmasks::getNorthWestRay(indTwo);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getNorthWestRay_Pos28_ShouldReturn0x0080402000000000) 
{
    bitmask expected = 0x0080402000000000ULL;
    bitmask actual = RayBitmasks::getNorthWestRay(indThree);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getNorthWestRay_Pos32_ShouldReturn0x0804020000000000) 
{
    bitmask expected = 0x0804020000000000ULL;
    bitmask actual = RayBitmasks::getNorthWestRay(indFour);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getNorthWestRay_Pos56_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = RayBitmasks::getNorthWestRay(indFive);
    ASSERT_EQ(expected, actual);
}

TEST_F(RayBitmasks_test, getNorthWestRay_Pos63_ShouldReturn0x0000000000000000) 
{
    bitmask expected = 0x0000000000000000ULL;
    bitmask actual = RayBitmasks::getNorthWestRay(indSix);
    ASSERT_EQ(expected, actual);
}

} // namespace logic
```


--------------------

# FILE: tests/logic/makemove/CMakeLists.txt

```
add_executable(makemove_tests
    MakeUnmakeMove_test.cpp
)

target_link_libraries(makemove_tests PRIVATE movegen makemove GTest::gtest_main)
gtest_discover_tests(makemove_tests)
```


--------------------

# FILE: tests/logic/makemove/MakeUnmakeMove_test.cpp

```cpp
// #include <gtest/gtest.h>

// #include "game/movegen/MoveGenerator.h"
// #include "game/move/Move.h"

// #include <unordered_set>
// #include <vector>
// #include <unordered_map>

// namespace logic {

//     class MakeUnmakeMoveTest : public ::testing::Test {
//         protected:
//             ChessBoard board;
//             MoveGenerator moveGenerator;
//             std::string startingPos;
//             std::string castling;
//             std::string captures;
//             std::string normalPromotion;
//             std::string capturePromotion;
//             std::string enPessant;
//             std::vector<model::Move> movelist;

//             MakeUnmakeMoveTest() : board(ChessBoard()), moveGenerator(MoveGenerator(board)) {}

//             virtual void SetUp() override {
//                 castling = "r3k2r/8/8/8/8/8/8/R3K2R";
//                 captures = "6n1/8/7N/1p2b3/P2B4/8/4q1r1/4Q1R1";
//                 normalPromotion = "8/1P6/8/8/8/8/6p1/8";
//                 capturePromotion = "p1b5/1P6/8/8/8/8/6p1/5N1Q";
//                 startingPos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
//                 enPessant = "8/4p3/8/3P4/7p/8/6P1/8";
//                 movelist = std::vector<model::Move>(movegen::MoveGenerator::MAX_LEGAL_MOVES);
//             }

//             std::vector<model::Move> getMoves() {
//                 return movelist;
//             }

//             std::vector<bitboard> getBitboards() {
//                 return moveGenerator.getBoard().getBitboards();
//             }

//             std::vector<Piece::Type> getPieceMap() {
//                 return moveGenerator.getBoard().getPieceMap();
//             }
//     };

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_StartingPositionWhite) {
//         moveGenerator.genMoves(true, movelist);

//         // Makes copies of the bitboards and pieceMap
//         std::vector<bitboard> bitboardsCopy = getBitboards();
//         std::vector<Piece::Type> pieceMapCopy = getPieceMap();
//         std::vector<model::Move> moves = movelist;

//         for (int i = 0; i < movelist.getMoveIndex(); i++) {
//             moveGenerator.getBoard().makeMove(moves[i], true);
//             moveGenerator.getBoard().unmakeMove(moves[i], true);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getPieceMap(), pieceMapCopy);
//         }
//     }

//         TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_StartingPositionBlack) {
//         moveGenerator.genMoves(false, movelist);

//         // Makes copies of the bitboards and pieceMap
//         std::vector<bitboard> bitboardsCopy = getBitboards();
//         std::vector<Piece::Type> pieceMapCopy = getPieceMap();
//         std::vector<model::Move> moves = getMoves();

//         for (int i = 0; i < movelist.getMoveIndex(); i++) {
//             moveGenerator.getBoard().makeMove(moves[i], false);
//             moveGenerator.getBoard().unmakeMove(moves[i], false);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getPieceMap(), pieceMapCopy);
//         }

//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_CapturesWhite) {
//         io::Fen::setBoardFromFen(captures, bitboards, stateBitmasks, pieceMap);
//         moveGenerator.genMoves(true, movelist);

//         // Makes copies of the bitboards and pieceMap
//         std::vector<bitboard> bitboardsCopy = getBitboards();
//         std::vector<Piece::Type> pieceMapCopy = getPieceMap();
//         std::vector<model::Move> moves = getMoves();

//         for (int i = 0; i < movelist.getMoveIndex(); i++) {
//             moveGenerator.getBoard().makeMove(moves[i], true);
//             moveGenerator.getBoard().unmakeMove(moves[i], true);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getPieceMap(), pieceMapCopy);
//         }
//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_CapturesBlack) {
//         io::Fen::setBoardFromFen(captures, bitboards, stateBitmasks, pieceMap);
//         moveGenerator.genMoves(false, movelist);

//         // Makes copies of the bitboards and pieceMap
//         std::vector<bitboard> bitboardsCopy = getBitboards();
//         std::vector<Piece::Type> pieceMapCopy = getPieceMap();
//         std::vector<model::Move> moves = getMoves();

//         for (int i = 0; i < movelist.getMoveIndex(); i++) {
//             moveGenerator.getBoard().makeMove(moves[i], false);
//             moveGenerator.getBoard().unmakeMove(moves[i], false);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getPieceMap(), pieceMapCopy);
//         }
//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_CastlingWhite) {
//         io::Fen::setBoardFromFen(castling, bitboards, stateBitmasks, pieceMap);
//         moveGenerator.genMoves(true, movelist);

//         // Makes copies of the bitboards and pieceMap
//         std::vector<bitboard> bitboardsCopy = getBitboards();
//         std::vector<Piece::Type> pieceMapCopy = getPieceMap();
//         std::vector<model::Move> moves = getMoves();

//         for (int i = 0; i < movelist.getMoveIndex(); i++) {
//             moveGenerator.getBoard().makeMove(moves[i], true);
//             moveGenerator.getBoard().unmakeMove(moves[i], true);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getPieceMap(), pieceMapCopy);
//         }
//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_CastlingBlack) {
//         io::Fen::setBoardFromFen(castling, bitboards, stateBitmasks, pieceMap);
//         moveGenerator.genMoves(false, movelist);

//         // Makes copies of the bitboards and pieceMap
//         std::vector<bitboard> bitboardsCopy = getBitboards();
//         std::vector<Piece::Type> pieceMapCopy = getPieceMap();
//         std::vector<model::Move> moves = getMoves();

//         for (int i = 0; i < movelist.getMoveIndex(); i++) {
//             moveGenerator.getBoard().makeMove(moves[i], false);
//             moveGenerator.getBoard().unmakeMove(moves[i], false);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getPieceMap(), pieceMapCopy);
//         }
//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_NormalPromotionWhite) {
//         io::Fen::setBoardFromFen(normalPromotion, bitboards, stateBitmasks, pieceMap);
//         moveGenerator.genMoves(true, movelist);

//         // Makes copies of the bitboards and pieceMap
//         std::vector<bitboard> bitboardsCopy = getBitboards();
//         std::vector<Piece::Type> pieceMapCopy = getPieceMap();
//         std::vector<model::Move> moves = getMoves();

//         for (int i = 0; i < movelist.getMoveIndex(); i++) {
//             moveGenerator.getBoard().makeMove(moves[i], true);
//             moveGenerator.getBoard().unmakeMove(moves[i], true);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getPieceMap(), pieceMapCopy);
//         }
//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_NormalPromotionBlack) {
//         io::Fen::setBoardFromFen(normalPromotion, bitboards, stateBitmasks, pieceMap);
//         moveGenerator.genMoves(false, movelist);

//         // Makes copies of the bitboards and pieceMap
//         std::vector<bitboard> bitboardsCopy = getBitboards();
//         std::vector<Piece::Type> pieceMapCopy = getPieceMap();
//         std::vector<model::Move> moves = getMoves();

//         for (int i = 0; i < movelist.getMoveIndex(); i++) {
//             moveGenerator.getBoard().makeMove(moves[i], false);
//             moveGenerator.getBoard().unmakeMove(moves[i], false);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getPieceMap(), pieceMapCopy);
//         }
//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_CapturePromotionWhite) {
//         io::Fen::setBoardFromFen(capturePromotion, bitboards, stateBitmasks, pieceMap);
//         moveGenerator.genMoves(true, movelist);

//         // Makes copies of the bitboards and pieceMap
//         std::vector<bitboard> bitboardsCopy = getBitboards();
//         std::vector<Piece::Type> pieceMapCopy = getPieceMap();
//         std::vector<model::Move> moves = getMoves();

//         for (int i = 0; i < movelist.getMoveIndex(); i++) {
//             moveGenerator.getBoard().makeMove(moves[i], true);
//             moveGenerator.getBoard().unmakeMove(moves[i], true);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getPieceMap(), pieceMapCopy);
//         }
//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_CapturePromotionBlack) {
//         io::Fen::setBoardFromFen(capturePromotion, bitboards, stateBitmasks, pieceMap);
//         moveGenerator.genMoves(false, movelist);

//         // Makes copies of the bitboards and pieceMap
//         std::vector<bitboard> bitboardsCopy = getBitboards();
//         std::vector<Piece::Type> pieceMapCopy = getPieceMap();
//         std::vector<model::Move> moves = getMoves();

//         for (int i = 0; i < movelist.getMoveIndex(); i++) {
//             moveGenerator.getBoard().makeMove(moves[i], false);
//             moveGenerator.getBoard().unmakeMove(moves[i], false);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getPieceMap(), pieceMapCopy);
//         }
//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_EnPessantWhite) {
//         io::Fen::setBoardFromFen(enPessant, bitboards, stateBitmasks, pieceMap);

//         // Make a double pawn push to enable en pessant
//         moveGenerator.getBoard().makeMove(Move(51, 35, model::Move::DOUBLE_PAWN_PUSH_FLAG), false);

//         // Makes copies of the bitboards and pieceMap
//         std::vector<bitboard> bitboardsCopy = getBitboards();
//         std::vector<Piece::Type> pieceMapCopy = getPieceMap();
        
//         moveGenerator.genMoves(true, movelist);
//         std::vector<model::Move> moves = getMoves();

//         for (int i = 0; i < movelist.getMoveIndex(); i++) {
//             moveGenerator.getBoard().makeMove(moves[i], true);
//             moveGenerator.getBoard().unmakeMove(moves[i], true);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getPieceMap(), pieceMapCopy);
//         }
//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_EnPessantBlack) {
//         io::Fen::setBoardFromFen(enPessant, bitboards, stateBitmasks, pieceMap);

//         // Make a double pawn push to enable en pessant
//         moveGenerator.getBoard().makeMove(Move(9, 17, model::Move::DOUBLE_PAWN_PUSH_FLAG), true);

//         // Makes copies of the bitboards and pieceMap
//         std::vector<bitboard> bitboardsCopy = getBitboards();
//         std::vector<Piece::Type> pieceMapCopy = getPieceMap();
        
//         moveGenerator.genMoves(false, movelist);
//         std::vector<model::Move> moves = getMoves();

//         for (int i = 0; i < movelist.getMoveIndex(); i++) {
//             moveGenerator.getBoard().makeMove(moves[i], false);
//             moveGenerator.getBoard().unmakeMove(moves[i], false);
//             ASSERT_EQ(getBitboards(), bitboardsCopy);
//             ASSERT_EQ(getPieceMap(), pieceMapCopy);
//         }
//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_CastlingRightsWhite) {
//         io::Fen::setBoardFromFen(castling, bitboards, stateBitmasks, pieceMap);

//         moveGenerator.genCastlingMoves(true, movelist);
//         std::vector<model::Move> castlingMoves = getMoves();

//         Move whiteHRookMove = model::Move(0, 8, 0);
//         Move whiteARookMove = model::Move(7, 15, 0);
//         Move whiteKingMove = model::Move(3, 2, 0);

//         moveGenerator.getBoard().makeMove(whiteHRookMove, true);
//         moveGenerator.getBoard().unmakeMove(whiteHRookMove, true);
//         moveGenerator.getBoard().makeMove(whiteARookMove, true);
//         moveGenerator.getBoard().unmakeMove(whiteARookMove, true);
//         moveGenerator.getBoard().makeMove(whiteKingMove, true);
//         moveGenerator.getBoard().unmakeMove(whiteKingMove, true);

//         moveGenerator.resetMoves(movelist);
//         moveGenerator.genCastlingMoves(true, movelist);
//         std::vector<model::Move> castlingMovesAfterUnmake = getMoves();

//         ASSERT_EQ(castlingMoves, castlingMovesAfterUnmake);
//     }

//     TEST_F(MakeUnmakeMoveTest, MakeUnmakeMove_CastlingRightsBlack) {
//         io::Fen::setBoardFromFen(castling, bitboards, stateBitmasks, pieceMap);

//         moveGenerator.genCastlingMoves(false, movelist);
//         std::vector<model::Move> castlingMoves = getMoves();

//         Move blackHRookMove = model::Move(56, 48, 0);
//         Move blackARookMove = model::Move(63, 55, 0);
//         Move blackKingMove = model::Move(59, 58, 0);

//         moveGenerator.getBoard().makeMove(blackHRookMove, false);
//         moveGenerator.getBoard().unmakeMove(blackHRookMove, false);
//         moveGenerator.getBoard().makeMove(blackARookMove, false);
//         moveGenerator.getBoard().unmakeMove(blackARookMove, false);
//         moveGenerator.getBoard().makeMove(blackKingMove, false);
//         moveGenerator.getBoard().unmakeMove(blackKingMove, false);

//         moveGenerator.resetMoves(movelist);
//         moveGenerator.genCastlingMoves(false, movelist);
//         std::vector<model::Move> castlingMovesAfterUnmake = getMoves();

//         ASSERT_EQ(castlingMoves, castlingMovesAfterUnmake);
//     }
// }

```


--------------------

# FILE: tests/engine/CMakeLists.txt

```
# add_subdirectory(search)
add_subdirectory(perft)
```


--------------------

# FILE: tests/engine/perft/CMakeLists.txt

```
add_executable(perft_test
    perftStartPos_test.cpp
    perftPosTwo_test.cpp
    perftPosThree_test.cpp
    perftPosFive_test.cpp
)

target_link_libraries(perft_test PRIVATE perft board GTest::gtest_main)
gtest_discover_tests(perft_test)
```


--------------------

# FILE: tests/engine/perft/perftBase_test.h

```cpp
#pragma once

#include "engine/pickmove/MovePicker.h"

#include "io/Fen.h"

#include "model/board/Board.h"

#include <cstdint>
#include <gtest/gtest.h>
#include <unordered_map>

namespace engine {

class perftBase : public ::testing::Test 
{

struct fromToBitIndices {
    int from;
    int to;
};

protected:
    bool longRuns = true;
    bool megaRuns = false;

    // Long run should be depth 6, mega run should be depth 7
    bool enableStartPosTest = true;
    int startPosMaxDepth = 5;

    // Long run should be depth 5
    bool enablePos2Test = true;
    int posTwoMaxDepth = 4;

    // Long run should be depth 7
    bool enablePos3Test = true;
    int posThreeMaxDepth = 6;

    // Long run should be depth 5
    bool enablePos5Test = true;
    int posFiveMaxDepth = 4;

    std::string startPos;
    std::string posTwo;
    std::string posThree;
    std::string posFive;

    MovePicker movePicker;

    perftBase() : movePicker(20) {}

    virtual void SetUp() override 
    {
        startPos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
        posTwo = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -";
        posThree = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -";
        posFive = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8";
    }

    char colToChar(int col) {
        switch (col) {
            case 0:
                return 'h';
            case 1:
                return 'g';
            case 2:
                return 'f';
            case 3:
                return 'e';
            case 4:
                return 'd';
            case 5:
                return 'c';
            case 6:
                return 'b';
            case 7:
                return 'a';
            default:
                return 'x';
        }
    }

    std::string translateMoveToStr(model::Move move, bool whiteStarted) 
    {
        if (move.isAnyCastle()) {
            return move.isKingCastle() ? (whiteStarted ? "e1g1" : "e8g8") 
                                        : (whiteStarted ? "e1c1" : "e8c8");
        }
        
        int from = move.getBitIndexFrom();
        int to = move.getBitIndexTo();
        
        int fromRow = from / 8;
        int fromCol = from % 8;

        char fromColChar = colToChar(fromCol);

        int toRow = to / 8;
        int toCol = to % 8;
        char toColChar = colToChar(toCol);

        std::string moveStr = "";
        moveStr += fromColChar;
        moveStr += std::to_string(fromRow + 1);
        moveStr += toColChar;
        moveStr += std::to_string(toRow + 1);

        if (move.isAnyPromo()) {
            switch (move.getFlag()) {
                case model::Move::KNIGHT_PROMO_FLAG:
                    moveStr += (whiteStarted) ? "n" : "N";
                    break;
                case model::Move::BISHOP_PROMO_FLAG:
                    moveStr += (whiteStarted) ? "b" : "B";
                    break;
                case model::Move::ROOK_PROMO_FLAG:
                    moveStr += (whiteStarted) ? "r" : "R";
                    break;
                case model::Move::QUEEN_PROMO_FLAG:
                    moveStr += (whiteStarted) ? "q" : "Q";
                    break;
                default:
                    break;
                case model::Move::KNIGHT_PROMO_CAPTURE_FLAG:
                    moveStr += (whiteStarted) ? "n" : "N";
                    break;
                case model::Move::BISHOP_PROMO_CAPTURE_FLAG:
                    moveStr += (whiteStarted) ? "b" : "B";
                    break;
                case model::Move::ROOK_PROMO_CAPTURE_FLAG:
                    moveStr += (whiteStarted) ? "r" : "R";
                    break;
                case model::Move::QUEEN_PROMO_CAPTURE_FLAG:
                    moveStr += (whiteStarted) ? "q" : "Q";
                    break;
            }
        }

        return moveStr;
    }

    fromToBitIndices translateStrToFromTo(std::string moveStr) 
    {
        fromToBitIndices fromTo;

        int fromRow = moveStr[1] - '1';
        int fromCol = 8 - (moveStr[0] - 'a') - 1;
        int toRow = moveStr[3] - '1';
        int toCol = 8 - (moveStr[2] - 'a') - 1;

        fromTo.from = fromRow * 8 + fromCol;
        fromTo.to = toRow * 8 + toCol;

        return fromTo;
    }

    model::Move moveFromStrAndFlag(std::string moveStr, int flag) 
    {
        fromToBitIndices fromTo = translateStrToFromTo(moveStr);
        return model::Move(fromTo.from, fromTo.to, flag);
    }

    std::unordered_map<std::string, uint64_t> nodeCountPerFirstMoveAsMap(bool whiteStarted) 
    {
        std::unordered_map<std::string, uint64_t> nodeCountPerFirstMoveMap{};
        int sum = 0;

        for (size_t i = 0; i < movePicker._nodeCountPerFirstMove.size(); i++) {
            if (movePicker._firstMoves[i].getMove() != 0) {
                std::string moveStr = translateMoveToStr(movePicker._firstMoves[i], whiteStarted);
                std::string nodeCountStr = std::to_string(movePicker._nodeCountPerFirstMove[i]);
                std::string moveNodeCountStr = moveStr + ": " + nodeCountStr;
                nodeCountPerFirstMoveMap[moveStr] = movePicker._nodeCountPerFirstMove[i];

                sum += movePicker._nodeCountPerFirstMove[i];
            }
        }

        nodeCountPerFirstMoveMap["searched"] = sum;

        return nodeCountPerFirstMoveMap;
    }

    void compareFirstMoveCountsToStockfish(
        const std::unordered_map<std::string, uint64_t> firstMoveCounts,
        const std::unordered_map<std::string, uint64_t> stockfishResults) 
    {
        std::ostringstream errors;
        bool hasErrors = false;

        for (const auto& moveCountPair : firstMoveCounts) {
            const std::string& move = moveCountPair.first;
            uint64_t count = moveCountPair.second;

            auto foundIt = stockfishResults.find(move);
            if (foundIt == stockfishResults.end()) {
                errors << "Move: " << move << " not found in stockfish results.\n";
                hasErrors = true;
            } else {
                uint64_t stockfishCount = foundIt->second;
                if (count != stockfishCount) {
                    errors << "Move: " << move << " failed. Expected: " << stockfishCount << ", Got: " << count << ".\n";
                    hasErrors = true;
                }
            }
        }

        // If there were any errors, print them and fail the test
        if (hasErrors) {
            std::cerr << errors.str();
            ASSERT_TRUE(false);
        }
    }
};

} // namespace engine
```


--------------------

# FILE: tests/engine/perft/perftPosFive_test.cpp

```cpp
#include "perftBase_test.h"

#include "io/StockfishPerftRetriever.h"
namespace engine {

class perftPosFive : public perftBase 
{
protected:
    void SetUp() override {
        perftBase::SetUp();
    }

    // Nodes
    std::unordered_map<int, int> expectedNodes = {
        {0, 1},
        {1, 44},
        {2, 1486},
        {3, 62379},
        {4, 2103487},
        {5, 89941194}
    };
};

TEST_F(perftPosFive, perft_pos5) 
{
    if (enablePos5Test) {
        movePicker.setBoardFromFen(posFive);

        // Make dubug move
        // model::Move move = moveFromStrAndFlag("a2a3", 0);
        // movePicker.makeMove(move, true);
        int nDebugMoves = 0;

        int depth = longRuns ? posFiveMaxDepth + 1 : posFiveMaxDepth;
        depth -= nDebugMoves;
        bool whiteToStart = nDebugMoves % 2 == 0;

        std::string debugFen;
        if (nDebugMoves > 0) {
            debugFen = movePicker.getFenFromBoard();
            debugFen += whiteToStart ? " w" : " b";
            debugFen += " KQ - 1 8";
        }

        std::unordered_map<std::string, uint64_t> stockfishResults = io::stockfish::getPerftResults(nDebugMoves > 0 ? debugFen : posFive, depth);

        movePicker.setMaxDepth(depth);
        movePicker.minimax(0, whiteToStart, 0);

        std::unordered_map<std::string, uint64_t> firstMoveCounts = nodeCountPerFirstMoveAsMap(whiteToStart);
        compareFirstMoveCountsToStockfish(firstMoveCounts, stockfishResults);

        if (nDebugMoves == 0) {
            for (uint64_t i = 1; i <= movePicker.getMaxDepth(); i++) {
                ASSERT_EQ(movePicker._nodeCount[i], expectedNodes[i]);
            }
        }
    }
}

} // namespace engine
```


--------------------

# FILE: tests/engine/perft/perftPosThree_test.cpp

```cpp
#include "perftBase_test.h"

#include "io/StockfishPerftRetriever.h"

namespace engine {

class perftPosThree : public perftBase 
{
protected:
    void SetUp() override {
        perftBase::SetUp();
    }

    // Nodes, captures, epCaptures, castling, promotion, check, checkmate
    std::unordered_map<int, std::vector<long>> expectedResults {
        {0, {1,         0,          0,          0,      0,          0,          0}},
        {1, {14,        1,          0,          0,      0,          2,          0}},
        {2, {191,       14,         0,          0,      0,          10,         0}},
        {3, {2812,      209,        2,          0,      0,          267,        0}},
        {4, {43238,     3348,       123,        0,      0,          1680,       17}},
        {5, {674624,    52051,      1165,       0,      0,          52950,      0}},
        {6, {11030083,  940350,     33325,      0,      7552,       452473,     2733}},
        {7, {178633661, 14519036,   294874,     0,      140024,     12797406,   87}}
    };
};

TEST_F(perftPosThree, perft_pos3) 
{
    if (enablePos3Test) {
        movePicker.setBoardFromFen(posThree);

        // model::Move move = moveFromStrAndFlag("a5a4", 0);
        // movePicker.makeMove(move, true);
        // move = moveFromStrAndFlag("h4g4", 0);
        // movePicker.makeMove(move, false);
        // move = moveFromStrAndFlag("a4b3", 0);
        // movePicker.makeMove(move, true);
        // move = moveFromStrAndFlag("g4f5", 0);
        // movePicker.makeMove(move, false);
        // move = moveFromStrAndFlag("b3c4", 0);
        // movePicker.makeMove(move, true);
        // move = moveFromStrAndFlag("f5e4", 0);
        // movePicker.makeMove(move, false);

        int nDebugMoves = 0;

        int depth = longRuns ? posThreeMaxDepth + 1 : posThreeMaxDepth;
        depth -= nDebugMoves;
        bool whiteToStart = nDebugMoves % 2 == 0;

        std::string debugFen;
        if (nDebugMoves > 0) {
            debugFen = movePicker.getFenFromBoard();
            debugFen += whiteToStart ? " w" : " b";
            debugFen += " - -";
        }

        std::unordered_map<std::string, uint64_t> stockfishResults = io::stockfish::getPerftResults(nDebugMoves > 0 ? debugFen : posThree, depth);

        movePicker.setMaxDepth(depth);
        movePicker.minimax(0, whiteToStart, 0);

        std::unordered_map<std::string, uint64_t> firstMoveCounts = nodeCountPerFirstMoveAsMap(whiteToStart);
        compareFirstMoveCountsToStockfish(firstMoveCounts, stockfishResults);

        if (nDebugMoves == 0) {
            for (long i = 1; i <= movePicker.getMaxDepth(); i++) {
                ASSERT_EQ(movePicker._nodeCount[i], expectedResults[i][0]);
                ASSERT_EQ(movePicker._captureCount[i], expectedResults[i][1]);
                ASSERT_EQ(movePicker._epCaptureCount[i], expectedResults[i][2]);
                ASSERT_EQ(movePicker._castlingCount[i], expectedResults[i][3]);
                ASSERT_EQ(movePicker._promotionCount[i], expectedResults[i][4]);
                ASSERT_EQ(movePicker._checkCount[i], expectedResults[i][5]);
                
                if (i < movePicker.getMaxDepth()) {
                    ASSERT_EQ(movePicker._checkmateCount[i], expectedResults[i][6]);
                }       
            }
        }
    }
}

} // namespace engine
```


--------------------

# FILE: tests/engine/perft/perftPosTwo_test.cpp

```cpp
#include "perftBase_test.h"

#include "io/StockfishPerftRetriever.h"

namespace engine {

class perftPosTwo : public perftBase 
{
protected:
    void SetUp() override {
        perftBase::SetUp();
    }

    // Nodes, captures, epCaptures, castling, promotion, check, checkmate
    std::unordered_map<int, std::vector<uint64_t>> expectedResults {
        {0, {1,         0,          0,          0,          0,          0,          0}},
        {1, {48,        8,          0,          2,          0,          0,          0}},
        {2, {2039,      351,        1,          91,         0,          3,          0}},
        {3, {97862,     17102,      45,         3162,       0,          993,        1}},
        {4, {4085603,   757163,     1929,       128013,     15172,      25523,      43}},
        {5, {193690690, 35043416,   73365,      4993637,    8392,       3309887,    30171}},
        {6, {0,         0,          0,          0,          0,          0,          0}}
    };
};

TEST_F(perftPosTwo, perft_pos2) 
{
    if (enablePos2Test) {
        movePicker.setBoardFromFen(posTwo);

        // Make dubug move
        // model::Move move = moveFromStrAndFlag("a2a3", 0);
        // movePicker.makeMove(move, true);
        // move = moveFromStrAndFlag("c7c6", 0);
        // movePicker.makeMove(model::Move(0, 0, model::Move::QUEEN_CASTLE_FLAG), false);

        int nDebugMoves = 0;

        int depth = longRuns ? posTwoMaxDepth + 1 : posTwoMaxDepth;
        depth -= nDebugMoves;
        bool whiteToStart = nDebugMoves % 2 == 0;

        std::string debugFen;
        if (nDebugMoves > 0) {
            debugFen = movePicker.getFenFromBoard();
            debugFen += whiteToStart ? " w" : " b";
            debugFen += " KQkq -";
        }

        std::unordered_map<std::string, uint64_t> stockfishResults = io::stockfish::getPerftResults(nDebugMoves > 0 ? debugFen : posTwo, depth);

        movePicker.setMaxDepth(depth);
        movePicker.minimax(0, whiteToStart, 0);
        
        std::unordered_map<std::string, uint64_t> firstMoveCounts = nodeCountPerFirstMoveAsMap(whiteToStart);
        compareFirstMoveCountsToStockfish(firstMoveCounts, stockfishResults);
        
        if (nDebugMoves == 0) {
            for (uint64_t i = 1; i <= movePicker.getMaxDepth(); i++) {
                ASSERT_EQ(movePicker._nodeCount[i], expectedResults[i][0]);
                ASSERT_EQ(movePicker._captureCount[i], expectedResults[i][1]);
                ASSERT_EQ(movePicker._epCaptureCount[i], expectedResults[i][2]);
                ASSERT_EQ(movePicker._castlingCount[i], expectedResults[i][3]);
                ASSERT_EQ(movePicker._promotionCount[i], expectedResults[i][4]);
                ASSERT_EQ(movePicker._checkCount[i], expectedResults[i][5]);
                
                if (i < movePicker.getMaxDepth()) {
                    ASSERT_EQ(movePicker._checkmateCount[i], expectedResults[i][6]);
                }       
            }
        }
    }
}

} // namespace engine
```


--------------------

# FILE: tests/engine/perft/perftStartPos_test.cpp

```cpp
#include "perftBase_test.h"

#include "io/StockfishPerftRetriever.h"

namespace engine {

class perftStartPos : public perftBase
{
protected:
    void SetUp() override {
        perftBase::SetUp();
    }
};


TEST_F(perftStartPos, perft_starting_pos) 
{
    if (enableStartPosTest) {
        int depth = longRuns ? startPosMaxDepth + 1 : startPosMaxDepth;
        if (megaRuns) {
            depth = 7;
            std::cout << "Running mega run for starting position" << std::endl;
        }

        std::unordered_map<std::string, uint64_t> stockfishResults = io::stockfish::getPerftResults(startPos, depth);

        movePicker.setMaxDepth(depth);
        bool whiteToStart = true;

        movePicker.minimax(0, whiteToStart, 0);
        std::unordered_map<std::string, uint64_t> firstMoveCounts = nodeCountPerFirstMoveAsMap(whiteToStart);
        compareFirstMoveCountsToStockfish(firstMoveCounts, stockfishResults);

        std::unordered_map<int, std::vector<uint64_t>> expectedResults {
            {0, {1,          0,         0,      0,      0, 0,        0    }},
            {1, {20,         0,         0,      0,      0, 0,        0    }},
            {2, {400,        0,         0,      0,      0, 0,        0    }},
            {3, {8902,       34,        0,      0,      0, 12,       0    }},
            {4, {197281,     1576,      0,      0,      0, 469,      8    }},
            {5, {4865609,    82719,     258,    0,      0, 27351,    347  }},
            {6, {119060324,  2812008,   5248,   0,      0, 809099,   10828}},
            {7, {3195901860, 108329926, 319617, 883453, 0, 33103848, 0    }}
        };

        for (uint64_t i = 1; i <= movePicker.getMaxDepth(); i++) {
            ASSERT_EQ(movePicker._nodeCount[i], expectedResults[i][0]);
            ASSERT_EQ(movePicker._captureCount[i], expectedResults[i][1]);
            ASSERT_EQ(movePicker._epCaptureCount[i], expectedResults[i][2]);
            ASSERT_EQ(movePicker._castlingCount[i], expectedResults[i][3]);
            ASSERT_EQ(movePicker._promotionCount[i], expectedResults[i][4]);
            ASSERT_EQ(movePicker._checkCount[i], expectedResults[i][5]);
            
            if (i < movePicker.getMaxDepth()) {
                ASSERT_EQ(movePicker._checkmateCount[i], expectedResults[i][6]);
            }       
        }
    }
}

} // namespace engine
```


--------------------

# FILE: tests/model/CMakeLists.txt

```
add_subdirectory(board)
# add_subdirectory(move)
```


--------------------

# FILE: tests/model/board/Bitboards_test.cpp

```cpp
#include <gtest/gtest.h>

#include "model/board/Board.h"

#include "io/Fen.h"

namespace model {

class BitboardsTest : public ::testing::Test 
{
protected:
    Bitboards bitboards;
    StateBitmasks stateBitmasks;
    PieceMap pieceMap;

    std::string fenOne;

    BitboardsTest()
        : bitboards(), 
        stateBitmasks(bitboards), 
        pieceMap(bitboards), 
        fenOne("8/8/8/8/3Pp3/8/8/8") 
    {
        // Constructor body if needed
    }

    void SetUp() override 
    {
        // Additional setup if needed, but member variables are already initialized
    }
};


TEST_F(BitboardsTest, ConstructorInit_WhitePawn) 
{
    bitboard expected = 0x000000000000FF00ULL;
    bitboard actual = bitboards.getBitboardFromPieceType(Piece::Type::W_PAWN);

    ASSERT_EQ(expected, actual);
}

TEST_F(BitboardsTest, ConstructorInit_WhiteKnight) 
{
    bitboard expected = 0x0000000000000042ULL;
    bitboard actual = bitboards.getBitboardFromPieceType(Piece::Type::W_KNIGHT);

    ASSERT_EQ(expected, actual);
}

TEST_F(BitboardsTest, ConstructorInit_WhiteBishop) 
{
    bitboard expected = 0x0000000000000024ULL;
    bitboard actual = bitboards.getBitboardFromPieceType(Piece::Type::W_BISHOP);

    ASSERT_EQ(expected, actual);
}

TEST_F(BitboardsTest, ConstructorInit_WhiteRook) 
{
    bitboard expected = 0x0000000000000081ULL;
    bitboard actual = bitboards.getBitboardFromPieceType(Piece::Type::W_ROOK);

    ASSERT_EQ(expected, actual);
}

TEST_F(BitboardsTest, ConstructorInit_WhiteQueen) 
{
    bitboard expected = 0x0000000000000010ULL;
    bitboard actual = bitboards.getBitboardFromPieceType(Piece::Type::W_QUEEN);

    ASSERT_EQ(expected, actual);
}

TEST_F(BitboardsTest, ConstructorInit_WhiteKing) 
{
    bitboard expected = 0x0000000000000008ULL;
    bitboard actual = bitboards.getBitboardFromPieceType(Piece::Type::W_KING);

    ASSERT_EQ(expected, actual);
}

TEST_F(BitboardsTest, ConstructorInit_BlackPawn) 
{
    bitboard expected = 0x00FF000000000000ULL;
    bitboard actual = bitboards.getBitboardFromPieceType(Piece::Type::B_PAWN);

    ASSERT_EQ(expected, actual);
}

TEST_F(BitboardsTest, ConstructorInit_BlackKnight) 
{
    bitboard expected = 0x4200000000000000ULL;
    bitboard actual = bitboards.getBitboardFromPieceType(Piece::Type::B_KNIGHT);

    ASSERT_EQ(expected, actual);
}

TEST_F(BitboardsTest, ConstructorInit_BlackBishop) 
{
    bitboard expected = 0x2400000000000000ULL;
    bitboard actual = bitboards.getBitboardFromPieceType(Piece::Type::B_BISHOP);

    ASSERT_EQ(expected, actual);
}

TEST_F(BitboardsTest, ConstructorInit_BlackRook) 
{
    bitboard expected = 0x8100000000000000ULL;
    bitboard actual = bitboards.getBitboardFromPieceType(Piece::Type::B_ROOK);

    ASSERT_EQ(expected, actual);
}

TEST_F(BitboardsTest, ConstructorInit_BlackQueen) 
{
    bitboard expected = 0x1000000000000000ULL;
    bitboard actual = bitboards.getBitboardFromPieceType(Piece::Type::B_QUEEN);

    ASSERT_EQ(expected, actual);
}

TEST_F(BitboardsTest, ConstructorInit_BlackKing) 
{
    bitboard expected = 0x0800000000000000ULL;
    bitboard actual = bitboards.getBitboardFromPieceType(Piece::Type::B_KING);

    ASSERT_EQ(expected, actual);
}

TEST_F(BitboardsTest, getWhitePiecesbitboard_ShouldReturn0x000000000000FFFF) 
{
    bitboard expected = 0x000000000000FFFFULL;
    bitboard actual = stateBitmasks.getWhitePiecesBitmask();

    ASSERT_EQ(expected, actual);
}

TEST_F(BitboardsTest, getBlackPiecesbitboard_ShouldReturn0xFFFF000000000000) 
{
    bitboard expected = 0xFFFF000000000000ULL;
    bitboard actual = stateBitmasks.getBlackPiecesBitmask();

    ASSERT_EQ(expected, actual);
}

TEST_F(BitboardsTest, fillPieceMapTest_ShouldReturnStartingPos) 
{
    ASSERT_EQ(pieceMap.getPieceTypeAtIndex(0), Piece::Type::W_ROOK);
    ASSERT_EQ(pieceMap.getPieceTypeAtIndex(1), Piece::Type::W_KNIGHT);
    ASSERT_EQ(pieceMap.getPieceTypeAtIndex(2), Piece::Type::W_BISHOP);
    ASSERT_EQ(pieceMap.getPieceTypeAtIndex(3), Piece::Type::W_KING);
    ASSERT_EQ(pieceMap.getPieceTypeAtIndex(4), Piece::Type::W_QUEEN);
    ASSERT_EQ(pieceMap.getPieceTypeAtIndex(5), Piece::Type::W_BISHOP);
    ASSERT_EQ(pieceMap.getPieceTypeAtIndex(6), Piece::Type::W_KNIGHT);
    ASSERT_EQ(pieceMap.getPieceTypeAtIndex(7), Piece::Type::W_ROOK);

    for (int i = 8; i < 16; i++) {
        ASSERT_EQ(pieceMap.getPieceTypeAtIndex(i), Piece::Type::W_PAWN);
    }

    for (int i = 16; i < 48; i++) {
        ASSERT_EQ(pieceMap.getPieceTypeAtIndex(i), Piece::Type::EMPTY);
    }

    for (int i = 48; i < 56; i++) {
        ASSERT_EQ(pieceMap.getPieceTypeAtIndex(i), Piece::Type::B_PAWN);
    }

    ASSERT_EQ(pieceMap.getPieceTypeAtIndex(56), Piece::Type::B_ROOK);
    ASSERT_EQ(pieceMap.getPieceTypeAtIndex(57), Piece::Type::B_KNIGHT);
    ASSERT_EQ(pieceMap.getPieceTypeAtIndex(58), Piece::Type::B_BISHOP);
    ASSERT_EQ(pieceMap.getPieceTypeAtIndex(59), Piece::Type::B_KING);
    ASSERT_EQ(pieceMap.getPieceTypeAtIndex(60), Piece::Type::B_QUEEN);
    ASSERT_EQ(pieceMap.getPieceTypeAtIndex(61), Piece::Type::B_BISHOP);
    ASSERT_EQ(pieceMap.getPieceTypeAtIndex(62), Piece::Type::B_KNIGHT);
    ASSERT_EQ(pieceMap.getPieceTypeAtIndex(63), Piece::Type::B_ROOK);
}

TEST_F(BitboardsTest, fillPieceMapFenOneTest_ShouldReturnTwoPieces) 
{
    io::Fen::setBoardFromFen(fenOne, bitboards, stateBitmasks, pieceMap);

    for (int i = 0; i < 64; i++) {
        if (i == 27) {
            ASSERT_EQ(pieceMap.getPieceTypeAtIndex(i), Piece::Type::B_PAWN);
        } else if (i == 28) {
            ASSERT_EQ(pieceMap.getPieceTypeAtIndex(i), Piece::Type::W_PAWN);
        } else {
            ASSERT_EQ(pieceMap.getPieceTypeAtIndex(i), Piece::Type::EMPTY);
        }
    }
}

} // namespace model
```


--------------------

# FILE: tests/model/board/CMakeLists.txt

```
add_executable(board_tests
    Bitboards_test.cpp
)

target_link_libraries(board_tests PRIVATE board io GTest::gtest_main)
gtest_discover_tests(board_tests)
```
