#pragma once
#include "PieceType.h"
#include <functional>

namespace game {
    class Move {
        public:
            // Public member functions
            Move();
            Move(PieceType pieceType, int bitIndexFrom, int bitIndexTo);
            PieceType getPieceType() {
                return _pieceType;
            }

            int getBitIndexFrom() const {
                return _bitIndexFrom;
            }

            int getBitIndexTo() const {
                return _bitIndexTo;
            }

            friend bool operator==(const Move& lhs, const Move& rhs) {
                return lhs._pieceType == rhs._pieceType &&
                       lhs._bitIndexFrom == rhs._bitIndexFrom &&
                       lhs._bitIndexTo == rhs._bitIndexTo;
        }

        private:
            // Private member variables
            PieceType _pieceType;
            int _bitIndexFrom;
            int _bitIndexTo;

            // Private member functions
    };
}

namespace std {
    template <>
    struct hash<game::Move> {
        size_t operator()(const game::Move& move) const {
            // Example hash combination of two integers
            return std::hash<int>()(move.getBitIndexFrom()) ^ std::hash<int>()(move.getBitIndexTo());
        }
    };
}