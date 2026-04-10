#pragma once

#include "model/types.h"

#include <functional>

namespace model {
struct Move {

public:

    // 0010 - Capture bit
    // 0001 - Promo bit
    // 1100 - Subtype bits
    // 
    // Subtypes:
    // 00 - Knight / Quiet
    // 10 - Bishop / Double pawn push
    // 01 - Rook   / King castle
    // 11 - Queen  / Queen castle
    enum Flag : std::uint8_t {
        QUIET_FLAG                = 0b0000,
        DOUBLE_PAWN_PUSH_FLAG     = 0b1000,
        KING_CASTLE_FLAG          = 0b0100,
        QUEEN_CASTLE_FLAG         = 0b1100,
        CAPTURE_FLAG              = 0b0010,
        EP_CAPTURE_FLAG           = 0b1010,
        KNIGHT_PROMO_FLAG         = 0b0001,
        BISHOP_PROMO_FLAG         = 0b1001,
        ROOK_PROMO_FLAG           = 0b0101,
        QUEEN_PROMO_FLAG          = 0b1101,
        KNIGHT_PROMO_CAPTURE_FLAG = 0b0011,
        BISHOP_PROMO_CAPTURE_FLAG = 0b1011,
        ROOK_PROMO_CAPTURE_FLAG   = 0b0111,
        QUEEN_PROMO_CAPTURE_FLAG  = 0b1111,
    };

    using val_t = std::uint16_t;
        
    Move() noexcept = default;

    Move(sq_t from, sq_t to, int flag) noexcept
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

    [[nodiscard]] constexpr sq_t from() const noexcept
    {
        return (val_ >> FROM_SHIFT) & FROM_MASK;
    }

    [[nodiscard]] constexpr sq_t to() const noexcept
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

    [[nodiscard]] constexpr bool is_any_castle() const noexcept
    {
        return flag() == KING_CASTLE_FLAG || flag() == QUEEN_CASTLE_FLAG;
    }

    [[nodiscard]] constexpr bool is_king_castle() const noexcept
    {
        return flag() == KING_CASTLE_FLAG;
    }

    [[nodiscard]] constexpr bool is_ep_capture() const noexcept
    {
        return flag() == EP_CAPTURE_FLAG;
    }

    [[nodiscard]] constexpr bool is_any_capture() const noexcept
    {
        return CAPTURE_BIT_MASK & flag();
    }

    [[nodiscard]] constexpr bool is_any_promo() const noexcept
    {
        return PROMO_BIT_MASK & flag();
    }

private:
    val_t val_{0};

    static constexpr int FROM_SHIFT = 0;
    static constexpr int TO_SHIFT   = 6;
    static constexpr int FLAG_SHIFT = 12;

    static constexpr val_t FROM_MASK = 0x3F;
    static constexpr val_t TO_MASK   = 0x3F;
    static constexpr val_t FLAG_MASK = 0x0F;

    static constexpr int8_t CAPTURE_BIT_MASK = 0b0010;
    static constexpr int8_t PROMO_BIT_MASK   = 0b0001;
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