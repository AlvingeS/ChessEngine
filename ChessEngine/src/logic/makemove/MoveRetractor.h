#pragma once

#include "ChessEngine/src/model/move/Move.h"
#include "ChessEngine/src/model/board/PieceType.h"
#include "ChessEngine/src/model/board/Bitboards.h"
#include "ChessEngine/src/model/board/GameStateBitmasks.h"
#include "ChessEngine/src/model/board/SquaresLookup.h"
#include "ChessEngine/src/model/board/ZHasher.h"
#include "ChessEngine/src/model/board/PieceType.h"

// FIXME: We cant have a dependency to perft
#include "ChessEngine/src/engine/search/SearchMemory.h"

namespace logic {

class MoveRetractor {

public:
    // Constructor
    MoveRetractor(
        model::Bitboards& bitBoards, 
        model::GameStateBitmasks& gameStateBitmasks, 
        model::SquaresLookup& squaresLookup, 
        model::ZHasher& zHasher,
        engine::SearchMemory& searchMemory 
    );
    
    void unmakeMove(
        const model::Move& move, 
        bool wasWhite, 
        int currentDepth
    );

    void unmakeTemporaryKingMove(bool wasWhite, bool isKingSide); 

    engine::SearchMemory& getSearchMemory() const
    {
        return _searchMemoryRef;
    }

private:
    /// @brief Unmakes a castle move
    /// @param wasWhite if the move was made by white
    /// @param isKingSide if the castle was king side
    void unmakeCastleMove(bool wasWhite, bool isKingSide);

   model::PieceType  determineMovedPieceType(const model::Move& move, bool wasWhite,int toIndex) const;
    
    void removePreviouslyMovedPieceFromBoard(const model::Move& move, int toIndex,model::PieceType  previouslyMovedPieceType, bool wasWhite);
    void placeBackCapturedPieceOnBoard(bool isEP, int captureIndex, int toIndex, bool wasWhite,model::PieceType  previouslyCapturedPieceType);
    void placeBackMovedPieceOnBoard(bool wasWhite, int fromIndex,model::PieceType  movedPieceType);

    model::Bitboards& _bitboardsRef;
    model::GameStateBitmasks& _gameStateBitmasksRef;
    model::SquaresLookup& _squaresLookupRef;
    const model::ZHasher& _zHasherRef;
    engine::SearchMemory& _searchMemoryRef;
};

} // namespace logic