#pragma once

#include "ChessEngine/src/representation/move/Move.h"
#include "ChessEngine/src/representation/board/PieceType.h"
#include "ChessEngine/src/representation/board/Bitboards.h"
#include "ChessEngine/src/representation/board/GameStateBitmasks.h"
#include "ChessEngine/src/representation/board/SquaresLookup.h"
#include "ChessEngine/src/representation/board/ZHasher.h"
#include "ChessEngine/src/representation/board/PieceType.h"

// FIXME: We cant have a dependency to perft
#include "ChessEngine/src/engine/search/SearchMemory.h"

namespace logic {
namespace makemove {

class MoveRetractor {

public:
    // Constructor
    MoveRetractor(
        representation::board::Bitboards& bitBoards, 
        representation::board::GameStateBitmasks& gameStateBitmasks, 
        representation::board::SquaresLookup& squaresLookup, 
        representation::board::ZHasher& zHasher,
        engine::search::SearchMemory& searchMemory 
    );
    
    void unmakeMove(
        const representation::move::Move& move, 
        bool wasWhite, 
        int currentDepth
    );

    void unmakeTemporaryKingMove(bool wasWhite, bool isKingSide); 

    engine::search::SearchMemory& getSearchMemory() const
    {
        return _searchMemoryRef;
    }

private:
    /// @brief Unmakes a castle move
    /// @param wasWhite if the move was made by white
    /// @param isKingSide if the castle was king side
    void unmakeCastleMove(bool wasWhite, bool isKingSide);

   representation::board::PieceType  determineMovedPieceType(const representation::move::Move& move, bool wasWhite,int toIndex) const;
    
    void removePreviouslyMovedPieceFromBoard(const representation::move::Move& move, int toIndex,representation::board::PieceType  previouslyMovedPieceType, bool wasWhite);
    void placeBackCapturedPieceOnBoard(bool isEP, int captureIndex, int toIndex, bool wasWhite,representation::board::PieceType  previouslyCapturedPieceType);
    void placeBackMovedPieceOnBoard(bool wasWhite, int fromIndex,representation::board::PieceType  movedPieceType);

    representation::board::Bitboards& _bitboardsRef;
    representation::board::GameStateBitmasks& _gameStateBitmasksRef;
    representation::board::SquaresLookup& _squaresLookupRef;
    const representation::board::ZHasher& _zHasherRef;
    engine::search::SearchMemory& _searchMemoryRef;
};

} // namespace makemove
} // namespace logic