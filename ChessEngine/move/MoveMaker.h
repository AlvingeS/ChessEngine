#pragma once

#include "ChessEngine/perft/SearchMemory.h"
#include "ChessEngine/board/PieceType.h"
#include "ChessEngine/board/Bitboards.h"
#include "ChessEngine/board/GameStateBitmasks.h"
#include "ChessEngine/board/SquaresLookup.h"
#include "ChessEngine/board/ZHasher.h"
#include "ChessEngine/board/PieceType.h"

#include "Move.h"

namespace move {

class MoveMaker {

public:
    // Constructor
    MoveMaker(
        board::Bitboards& bitBoards, 
        board::GameStateBitmasks& gameStateBitmasks, 
        board::SquaresLookup& squaresLookup, 
        perft::SearchMemory& searchMemory, 
        board::ZHasher& zHasher
    );
    
    // Public member functions
    void makeMove(
        const Move& move,
        bool isWhite, 
        int currentDepth
    );

    board::PieceType removeMovedPieceFromBoard(bool isWhite, int fromIndex);

    void placeMovedPieceOnBoard(
        bool isWhite, 
        int toIndex,
        board::PieceType movedPieceType
    );

    void handleEnPessantMemory(
        const move::Move& move, 
        bool isWhite, 
        int currentDepth, 
        int toIndex
    );

    void handleNoCaptureCount(
        const move::Move& move, 
        int currentDepth,
        board::PieceType movedPieceType 
    );
    
    void unmakeMove(
        const Move& move, 
        bool wasWhite, 
        int currentDepth
    );

    void placeBackMovedPieceOnBoard(
        bool wasWhite,
        int fromIndex, 
        board::PieceType movedPieceType
    );

    
    void makeCastleMove(bool isWhite, bool isKingSide);
    void unmakeCastleMove(bool isWhite, bool isKingSide);
    void makeTemporaryKingMove(bool isWhite, bool isKingSide);
    void unmakeTemporaryKingMove(bool isWhite, bool isKingSide); 

    perft::SearchMemory& getSearchMemory() const
    {
        return _searchMemoryRef;
    }

private:
    board::PieceType determineMovedPieceType(
        const Move& move, 
        bool wasWhite,
        int toIndex
    ) const;

    board::PieceType getPromotionPieceType(int promotionFlag, bool isWhite) const;
    void removeCapturedPieceFromBoard(bool isEP, bool isWhite, int captureIndex, board::PieceType capturedPieceType);
    void placeBackCapturedPieceOnBoard(bool isEP, int captureIndex, int toIndex, bool wasWhite, board::PieceType previouslyCapturedPieceType);
    void removePreviouslyMovedPieceFromBoard(const Move& move, int toIndex, board::PieceType previouslyMovedPieceType, bool wasWhite);

    board::Bitboards& _bitboardsRef;
    board::GameStateBitmasks& _gameStateBitmasksRef;
    board::SquaresLookup& _squaresLookupRef;
    perft::SearchMemory& _searchMemoryRef;
    const board::ZHasher& _zHasherRef;
};
}