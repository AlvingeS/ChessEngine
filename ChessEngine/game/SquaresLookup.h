#pragma once

#include "ChessEngine/common.h"

#include "PieceType.h"
#include "BitBoards.h"

namespace game {
    class SquaresLookup {
        public:
            // ** Constructor **
            SquaresLookup(BitBoards& bitboards);

            // ** Getters & Setters **
            PieceType getPieceTypeAtIndex(int index) {
                return _squaresLookup[index];
            }

            void setPieceTypeAtIndex(int index, PieceType pieceType) {
                _squaresLookup[index] = pieceType;
            }

            // TODO: Should this really by a thing?
            std::vector<PieceType>& getSquaresLookup() {
                return _squaresLookup;
            }

            // ** Functions **
            void fillSquaresLookup(BitBoards& bitboards);
            
        private:
            // ** Data **
            std::vector<PieceType> _squaresLookup;
        };
}