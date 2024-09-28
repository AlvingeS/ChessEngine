#pragma once

#include "ChessEngine/src/game/board/Bitboards.h"

namespace board {

class GameStateBitmasks {

public:
    /// @brief Constructor
    /// @param bitboards The bitboards to fill the bitmasks from
    GameStateBitmasks(const Bitboards& bitboards);
    
    /// @brief Returns true if the bit at the given index is set 
    /// in the white pieces bitmask
    inline bool whitePiecesBitIsSet(int index) const 
    {
        return (_whitePiecesBitmask & (1ULL << index)) != 0;
    }

    /// @brief Returns true if the bit at the given index is set
    /// in the black pieces bitmask
    inline bool blackPiecesBitIsSet(int index) const 
    {
        return (_blackPiecesBitmask & (1ULL << index)) != 0;
    }

    /// @brief Sets the bit at the given index in the white pieces bitmask
    inline void setWhitePiecesBit(int index) 
    {
        //assert(!whitePiecesBitIsSet(index));
        _whitePiecesBitmask |= (1ULL << index);
    }

    /// @brief Clears the bit at the given index in the white pieces bitmask
    inline void clearWhitePiecesBit(int index) 
    {
        //assert(whitePiecesBitIsSet(index));
        _whitePiecesBitmask &= ~(1ULL << index);
    }

    /// @brief Returns the white pieces bitmask
    inline const bitmask& getWhitePiecesBitmask() const 
    {
        return _whitePiecesBitmask;
    }

    /// @brief Sets the bit at the given index in the black pieces bitmask
    inline void setBlackPiecesBit(int index) 
    {
        //assert(!blackPiecesBitIsSet(index));
        _blackPiecesBitmask |= (1ULL << index);
    }

    /// @brief Clears the bit at the given index in the black pieces bitmask
    inline void clearBlackPiecesBit(int index) 
    {
        //assert(blackPiecesBitIsSet(index));
        _blackPiecesBitmask &= ~(1ULL << index);
    }

    /// @brief Returns the black pieces bitmask
    inline const bitmask& getBlackPiecesBitmask() const 
    {
        return _blackPiecesBitmask;
    }

    /// @brief Updates the occupied and empty squares bitmasks
    inline void updOccupiedAndEmptySquaresBitmasks() 
    {
        updOccupiedPiecesBitmask();
        updEmptySquaresBitmask();
    }

    /// @brief Returns the occupied pieces bitmask 
    inline const bitmask& getOccupiedPiecesBitmask() const 
    {
        return _occupiedPiecesBitmask;
    }

    /// @brief Returns the empty squares bitmask
    inline const bitmask& getEmptySquaresBitmask() const 
    {
        return _emptySquaresBitmask;
    }

    /// @brief Fills the white pieces bitmask from the given bitboards
    void fillWhitePiecesBitmaskFromBitboards(const Bitboards& bitboards);
    
    /// @brief Fills the black pieces bitmask from the given bitboards
    void fillBlackPiecesBitmaskFromBitboards(const Bitboards& bitboards);

private:

    /// @brief Updates the occupied pieces bitmask
    inline void updOccupiedPiecesBitmask() 
    {
        _occupiedPiecesBitmask = _whitePiecesBitmask | _blackPiecesBitmask;
    }

    /// @brief Updates the empty squares bitmask
    inline void updEmptySquaresBitmask() 
    {
        _emptySquaresBitmask = ~_occupiedPiecesBitmask;
    }

    bitmask _whitePiecesBitmask;
    bitmask _blackPiecesBitmask;
    bitmask _occupiedPiecesBitmask;
    bitmask _emptySquaresBitmask;
};

} // namespace board