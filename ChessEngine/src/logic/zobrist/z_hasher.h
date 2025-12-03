#pragma once

#include "model/position/position.h"

namespace logic {

enum CastleRightsType : int {
    W_KSIDE,
    W_QSIDE,
    B_KSIDE,
    B_QSIDE  
};

class ZHasher {

public:
    ZHasher(const model::Position& pos);

    void xor_piece_type_at(sq_idx sq, model::Piece::Type piece_type);
    void xor_castle_right(CastleRightsType c_rights_type);
    void xor_ep_file_at(int file_idx);
    void toggle_side_to_move();

    void set_z_hash(uint64_t z_hash);
    uint64_t value() const;

private:
    void compute_initial_hash(const model::Position& pos);

    uint64_t square_piece_type_keys[64][12];
    
    // In order:
    //      0 - White King side
    //      1 - White Queen side
    //      2 - Black King side	
    //      3 - Black Queen side
    uint64_t c_right_keys[4];
    uint64_t ep_file_keys[8];
    uint64_t is_w_key;

    uint64_t z_hash_;
};

} // namespace logic

#include "logic/zobrist/z_hasher.inl"
