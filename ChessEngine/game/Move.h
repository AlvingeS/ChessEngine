#pragma once
#include "PieceType.h"
#include <functional>

namespace game {
    class Move {
        public:
            static const int QUITE_FLAG = 0;
            static const int CAPTURE_FLAG = 1;
            static const int DOUBLE_PAWN_PUSH_FLAG = 2;
            static const int KING_CASTLE_FLAG = 3;
            static const int QUEEN_CASTLE_FLAG = 4;
            static const int EP_CAPTURE_FLAG = 5;
            static const int KNIGHT_PROMO_FLAG = 6;
            static const int BISHOP_PROMO_FLAG = 7;
            static const int ROOK_PROMO_FLAG = 8;
            static const int QUEEN_PROMO_FLAG = 9;
            static const int KNIGHT_PROMO_CAPTURE_FLAG = 10;
            static const int BISHOP_PROMO_CAPTURE_FLAG = 11;
            static const int ROOK_PROMO_CAPTURE_FLAG = 12;
            static const int QUEEN_PROMO_CAPTURE_FLAG = 13;
            static const int TBD_ONE_FLAG = 14;
            static const int TBD_TWO_FLAG = 15;
            
            Move() {
                _move = 0;
            }

            // Public member functions            
            Move(int bitIndexFrom, int bitIndexTo, int flag) {
                _move = ((flag & 0xf) << 12 | (bitIndexTo & 0x3f) << 6 | (bitIndexFrom & 0x3f));
            }

            // Copy constructor
            Move(const Move& move) {
                _move = move._move;
            }

            int getMove() const {
                return _move;
            }

            int getFlag() const {
                return (_move >> 12) & 0xf;
            }

            void operator=(const Move& move) {
                _move = move._move;
            }

            bool operator==(const Move& move) const {
                return (_move & 0xffff) == (move._move & 0xffff);
            }

            bool operator!=(const Move& move) const {
                return (_move & 0xffff) != (move._move & 0xffff);
            }

            int getBitIndexFrom() const {
                return _move & 0x3f;
            }
            
            int getBitIndexTo() const {
                return (_move >> 6) & 0x3f;
            }
            
            bool isQuite() const {
                return ((_move >> 12) & 0xf) == QUITE_FLAG;
            }

            bool isDoublePawnPush() const {
                return ((_move >> 12) & 0xf) == DOUBLE_PAWN_PUSH_FLAG;
            }

            bool isKingCastle() const {
                return ((_move >> 12) & 0xf) == KING_CASTLE_FLAG;
            }

            bool isQueenCastle() const {
                return ((_move >> 12) & 0xf) == QUEEN_CASTLE_FLAG;
            }

            bool isNormalCapture() const {
                return ((_move >> 12) & 0xf) == CAPTURE_FLAG;
            }

            bool isEpCapture() const {
                return ((_move >> 12) & 0xf) == EP_CAPTURE_FLAG;
            }

            bool isKnightPromo() const {
                return ((_move >> 12) & 0xf) == KNIGHT_PROMO_FLAG;
            }

            bool isBishopPromo() const {
                return ((_move >> 12) & 0xf) == BISHOP_PROMO_FLAG;
            }

            bool isRookPromo() const {
                return ((_move >> 12) & 0xf) == ROOK_PROMO_FLAG;
            }

            bool isQueenPromo() const {
                return ((_move >> 12) & 0xf) == QUEEN_PROMO_FLAG;
            }

            bool isKnightPromoCapture() const {
                return ((_move >> 12) & 0xf) == KNIGHT_PROMO_CAPTURE_FLAG;
            }

            bool isBishopPromoCapture() const {
                return ((_move >> 12) & 0xf) == BISHOP_PROMO_CAPTURE_FLAG;
            }

            bool isRookPromoCapture() const {
                return ((_move >> 12) & 0xf) == ROOK_PROMO_CAPTURE_FLAG;
            }

            bool isQueenPromoCapture() const {
                return ((_move >> 12) & 0xf) == QUEEN_PROMO_CAPTURE_FLAG;
            }

            bool isAnyCapture() const {
                return isNormalCapture() || isEpCapture() || isKnightPromoCapture() || isBishopPromoCapture() || isRookPromoCapture() || isQueenPromoCapture();
            }


            bool isAnyPromo() const {
                return isKnightPromo() || isBishopPromo() || isRookPromo() || isQueenPromo() || isKnightPromoCapture() || isBishopPromoCapture() || isRookPromoCapture() || isQueenPromoCapture();
            }

            bool isAnyPromoCapture() const {
                return isKnightPromoCapture() || isBishopPromoCapture() || isRookPromoCapture() || isQueenPromoCapture();
            }

            bool isAnyCastle() const {
                return isKingCastle() || isQueenCastle();
            }

        private:
            // Private member variables
            int _move;
    };
}

namespace std {
    template <>
    struct hash<game::Move> {
        size_t operator()(const game::Move& move) const {
            size_t h1 = std::hash<int>()(move.getBitIndexFrom());
            size_t h2 = std::hash<int>()(move.getBitIndexTo());
            size_t h3 = std::hash<int>()(move.getFlag());

            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };
}