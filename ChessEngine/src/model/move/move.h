#pragma once

#include "model/types.h"
#include <functional>

namespace model {

struct Move {

public:
    enum Flag : std::uint8_t {
        QUIET_FLAG                = 0,
        CAPTURE_FLAG              = 1,
        EP_CAPTURE_FLAG           = 2,
        KNIGHT_PROMO_CAPTURE_FLAG = 3,
        BISHOP_PROMO_CAPTURE_FLAG = 4,
        ROOK_PROMO_CAPTURE_FLAG   = 5,
        QUEEN_PROMO_CAPTURE_FLAG  = 6,
        KNIGHT_PROMO_FLAG         = 7,
        BISHOP_PROMO_FLAG         = 8,
        ROOK_PROMO_FLAG           = 9,
        QUEEN_PROMO_FLAG          = 10,
        TBD_ONE_FLAG              = 11,
        TBD_TWO_FLAG              = 12,
        DOUBLE_PAWN_PUSH_FLAG     = 13,
        KING_CASTLE_FLAG          = 14,
        QUEEN_CASTLE_FLAG         = 15
    };
    
    using val_t = std::uint16_t;

    static constexpr int FROM_SHIFT = 0;
    static constexpr int TO_SHIFT   = 6;
    static constexpr int FLAG_SHIFT = 12;
    
    static constexpr val_t FROM_MASK = 0x3F;
    static constexpr val_t TO_MASK   = 0x3F;
    static constexpr val_t FLAG_MASK = 0x0F;
    
    val_t val_ = 0;

    Move() noexcept = default;

    Move(sq_idx from, sq_idx to, int flag) noexcept
        : val_(
            ((from & FROM_MASK) << FROM_SHIFT) |
            ((to   & TO_MASK)   << TO_SHIFT)   |
            ((flag & FLAG_MASK) << FLAG_SHIFT))
        {}

    [[nodiscard]] constexpr bool operator==(const Move& other) const noexcept
    {
        return val_ == other.val_;
    }

    [[nodiscard]] constexpr bool operator!=(const Move& other) const noexcept
    {
        return val_ != other.val_;
    }

    [[nodiscard]] constexpr val_t value() const noexcept
    {
        return val_;
    }

    [[nodiscard]] constexpr int flag() const noexcept
    {
        return (val_ >> FLAG_SHIFT) & FLAG_MASK;
    }

    [[nodiscard]] constexpr sq_idx from() const noexcept
    {
        return (val_ >> FROM_SHIFT) & FROM_MASK;
    }

    [[nodiscard]] constexpr sq_idx to() const noexcept
    {
        return (val_ >> TO_SHIFT) & TO_MASK;
    }

    [[nodiscard]] constexpr bool is_quiet() const noexcept
    {
        return flag() == QUIET_FLAG;
    }

    [[nodiscard]] constexpr bool is_double_pawn_push() const noexcept
    {
        return flag() == DOUBLE_PAWN_PUSH_FLAG;
    }

    [[nodiscard]] constexpr bool is_king_castle() const noexcept
    {
        return flag() == KING_CASTLE_FLAG;
    }

    [[nodiscard]] constexpr bool is_queen_castle() const noexcept
    {
        return flag() == QUEEN_CASTLE_FLAG;
    }

    [[nodiscard]] constexpr bool is_normal_capture() const noexcept
    {
        return flag() == CAPTURE_FLAG;
    }

    [[nodiscard]] constexpr bool is_ep_capture() const noexcept
    {
        return flag() == EP_CAPTURE_FLAG;
    }

    [[nodiscard]] constexpr bool is_knight_promo() const noexcept
    {
        return flag() == KNIGHT_PROMO_FLAG;
    }

    [[nodiscard]] constexpr bool is_bishop_promo() const noexcept
    {
        return flag() == BISHOP_PROMO_FLAG;
    }

    [[nodiscard]] constexpr bool is_rook_promo() const noexcept
    {
        return flag() == ROOK_PROMO_FLAG;
    }

    [[nodiscard]] constexpr bool is_queen_promo() const noexcept
    {
        return flag() == QUEEN_PROMO_FLAG;
    }

    [[nodiscard]] constexpr bool is_knight_promo_capture() const noexcept
    {
        return flag() == KNIGHT_PROMO_CAPTURE_FLAG;
    }

    [[nodiscard]] constexpr bool is_bishop_promo_capture() const noexcept
    {
        return flag() == BISHOP_PROMO_CAPTURE_FLAG;
    }

    [[nodiscard]] constexpr bool is_rook_promo_capture() const noexcept
    {
        return flag() == ROOK_PROMO_CAPTURE_FLAG;
    }

    [[nodiscard]] constexpr bool is_queen_promo_capture() const noexcept
    {
        return flag() == QUEEN_PROMO_CAPTURE_FLAG;
    }

    [[nodiscard]] constexpr bool is_any_capture() const noexcept
    {
        const int f = flag();
        return f >= CAPTURE_FLAG && f <= QUEEN_PROMO_CAPTURE_FLAG;
    }

    [[nodiscard]] constexpr bool is_any_promo() const noexcept
    {
        const int f = flag();
        return f >= KNIGHT_PROMO_CAPTURE_FLAG && f <= QUEEN_PROMO_FLAG;
    }

    [[nodiscard]] constexpr bool is_any_promo_capture() const noexcept
    {
        const int f = flag();
        return f >= KNIGHT_PROMO_CAPTURE_FLAG && f <= QUEEN_PROMO_CAPTURE_FLAG;
    }

    [[nodiscard]] constexpr bool is_any_castle() const noexcept
    {
        return flag() >= KING_CASTLE_FLAG;
    }
};

} // namespace model

namespace std
{
    template <>
    struct hash<model::Move>
    {
        size_t operator()(const model::Move& m) const noexcept
        {
            return static_cast<size_t>(m.value());
        }
    };
}