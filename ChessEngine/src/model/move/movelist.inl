namespace model {

inline void Movelist::add_null_move() {
    movelist_[move_idx_] = Move();
}

inline void Movelist::reset() {
    move_idx_ = 0;
}

inline const Move& Movelist::get_move_at(int idx) const {
    return movelist_[idx];
}

inline int Movelist::get_move_idx() const {
    return move_idx_;
}

}