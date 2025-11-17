namespace model {

inline bool StateBitmasks::whitePiecesBitIsSet(int index) const 
{
    return (_whitePiecesBitmask & (1ULL << index)) != 0;
}

inline bool StateBitmasks::blackPiecesBitIsSet(int index) const 
{
    return (_blackPiecesBitmask & (1ULL << index)) != 0;
}

inline void StateBitmasks::setWhitePiecesBit(int index) 
{
    _whitePiecesBitmask |= (1ULL << index);
}

inline void StateBitmasks::clearWhitePiecesBit(int index) 
{
    _whitePiecesBitmask &= ~(1ULL << index);
}

inline const bitmask& StateBitmasks::getWhitePiecesBitmask() const 
{
    return _whitePiecesBitmask;
}

inline void StateBitmasks::setBlackPiecesBit(int index) 
{
    _blackPiecesBitmask |= (1ULL << index);
}

inline void StateBitmasks::clearBlackPiecesBit(int index) 
{
    _blackPiecesBitmask &= ~(1ULL << index);
}

inline const bitmask& StateBitmasks::getBlackPiecesBitmask() const 
{
    return _blackPiecesBitmask;
}

inline const bitmask& StateBitmasks::getOccupiedPiecesBitmask() const 
{
    return _occupiedPiecesBitmask;
}

inline const bitmask& StateBitmasks::getEmptySquaresBitmask() const 
{
    return _emptySquaresBitmask;
}

inline void StateBitmasks::updOccupiedPiecesBitmask() 
{
    _occupiedPiecesBitmask = _whitePiecesBitmask | _blackPiecesBitmask;
}

inline void StateBitmasks::updEmptySquaresBitmask() 
{
    _emptySquaresBitmask = ~_occupiedPiecesBitmask;
}

} // namespace model