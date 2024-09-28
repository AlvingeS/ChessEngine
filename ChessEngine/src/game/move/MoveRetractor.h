#pragma once

#include "ChessEngine/src/game/move/Move.h"

#include "ChessEngine/src/game/board/PieceType.h"
#include "ChessEngine/src/game/board/Bitboards.h"
#include "ChessEngine/src/game/board/GameStateBitmasks.h"
#include "ChessEngine/src/game/board/SquaresLookup.h"
#include "ChessEngine/src/game/board/ZHasher.h"
#include "ChessEngine/src/game/board/PieceType.h"

// FIXME: We cant have a dependency to perft
#include "ChessEngine/src/ponder/perft/SearchMemory.h"


namespace move {

class MoveRetractor {

public:
    // Constructor
    MoveRetractor(
        board::Bitboards& bitBoards, 
        board::GameStateBitmasks& gameStateBitmasks, 
        board::SquaresLookup& squaresLookup, 
        perft::SearchMemory& searchMemory, 
        board::ZHasher& zHasher
    );
    
    void unmakeMove(
        const Move& move, 
        bool wasWhite, 
        int currentDepth
    );

    void unmakeTemporaryKingMove(bool wasWhite, bool isKingSide); 

    perft::SearchMemory& getSearchMemory() const
    {
        return _searchMemoryRef;
    }

private:
    /// @brief Unmakes a castle move
    /// @param wasWhite if the move was made by white
    /// @param isKingSide if the castle was king side
    void unmakeCastleMove(bool wasWhite, bool isKingSide);

    board::PieceType determineMovedPieceType(const Move& move, bool wasWhite,int toIndex) const;
    
    void removePreviouslyMovedPieceFromBoard(const Move& move, int toIndex, board::PieceType previouslyMovedPieceType, bool wasWhite);
    void placeBackCapturedPieceOnBoard(bool isEP, int captureIndex, int toIndex, bool wasWhite, board::PieceType previouslyCapturedPieceType);
    void placeBackMovedPieceOnBoard(bool wasWhite, int fromIndex, board::PieceType movedPieceType);

    board::Bitboards& _bitboardsRef;
    board::GameStateBitmasks& _gameStateBitmasksRef;
    board::SquaresLookup& _squaresLookupRef;
    perft::SearchMemory& _searchMemoryRef;
    const board::ZHasher& _zHasherRef;
};
}