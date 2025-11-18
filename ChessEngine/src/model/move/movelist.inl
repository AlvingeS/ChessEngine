namespace model {

inline void Movelist::add_null_move() {
    movelist_[move_index_] = Move();
}

inline void Movelist::reset() {
    move_index_ = 0;
}

inline const Move& Movelist::get_move_at(int index) const {
    return movelist_[index];
}

inline int Movelist::get_move_index() const {
    return move_index_;
}

}