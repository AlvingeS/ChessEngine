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
    Move(int bit_idx_from, int bit_idx_to, int flag);
    Move(const Move& move);

    int get_move() const;
    int get_flag() const;

    void operator=(const Move& move);
    bool operator==(const Move& move) const;
    bool operator!=(const Move& move) const;

    int get_bit_idx_from() const;
    int get_bit_idx_to() const;
    bool is_quite() const;
    bool is_double_pawn_push() const;
    bool is_king_castle() const;
    bool is_queen_castle() const;
    bool is_normal_capture() const;
    bool is_ep_capture() const;
    bool is_knight_promo() const;
    bool is_bishop_promo() const;
    bool is_rook_promo() const;
    bool is_queen_promo() const;
    bool is_knight_promo_capture() const;
    bool is_bishop_promo_capture() const;
    bool is_rook_promo_capture() const;
    bool is_queen_promo_capture() const;
    bool is_any_capture() const;
    bool is_any_promo() const;
    bool is_any_promo_capture() const;
    bool is_any_castle() const;

private:
    int move_;
};

} // namespace model

#include "model/move/move.inl"

namespace std {
    template <>
    struct hash<model::Move> {
        size_t operator()(const model::Move& move) const {
            size_t h1 = std::hash<int>()(move.get_bit_idx_from());
            size_t h2 = std::hash<int>()(move.get_bit_idx_to());
            size_t h3 = std::hash<int>()(move.get_flag());

            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };
}