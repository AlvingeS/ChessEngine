#pragma once

#include "ChessEngine/common.h"

#include "BitBoards.h"

namespace game {
    class GameStateBitMasks {
        public:
            // ** Constructor **
            GameStateBitMasks(BitBoards& bitboards);

            // ** Getters & Setters **
            U64& getWhitePiecesBitmask() {
                return _whitePiecesBitmask;
            }

            U64& getBlackPiecesBitmask() {
                return _blackPiecesBitmask;
            }

            U64& getOccupiedPiecesBitmask() {
                return _occupiedPiecesBitmask;
            }

            U64& getEmptySquaresBitmask() {
                return _emptySquaresBitmask;
            }
            
            void fillWhitePiecesBitmask(BitBoards& bitboards);
            void fillBlackPiecesBitmask(BitBoards& bitboards);
            
            void fillOccupiedPiecesBitmask() {
                _occupiedPiecesBitmask = _whitePiecesBitmask | _blackPiecesBitmask;
            }

            void fillEmptySquaresBitmask() {
                _emptySquaresBitmask = ~_occupiedPiecesBitmask;
            }
        private:
            

            // ** Data **
            U64 _whitePiecesBitmask;
            U64 _blackPiecesBitmask;
            U64 _occupiedPiecesBitmask;
            U64 _emptySquaresBitmask;
        };
}