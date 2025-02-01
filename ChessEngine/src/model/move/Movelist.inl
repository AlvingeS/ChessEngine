namespace model {

inline void Movelist::addNullMove() {
    _movelist[_moveIndex] = Move();
}

inline void Movelist::reset() {
    _moveIndex = 0;
}

inline const Move& Movelist::getMoveAt(int index) const {
    return _movelist[index];
}

inline int Movelist::getMoveIndex() const {
    return _moveIndex;
}

}