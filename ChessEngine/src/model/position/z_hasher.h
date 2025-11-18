#pragma once

#include "model/position/piece_type.h"

#include <cstdint>

namespace model {

class PieceMap;

class ZHasher {

public:
    ZHasher(PieceMap& piece_map);
    void hash_square_piece_type(int square_index, Piece::Type piece_type);
    void hash_castle_rights(int castle_rights);
    void hash_en_pessant_file(int file);
    void hash_is_w(bool is_w);

private:
    void compute_initial_hash();

    uint64_t random_board_piece_type_numbers_[64][12];
    
    // In order:
    //      0 - White King side
    //      1 - White Queen side
    //      2 - Black King side	
    //      3 - Black Queen side
    uint64_t random_castle_rights_numbers_[4];
    
    uint64_t random_en_pessant_file_numbers_[8];
    uint64_t random_is_w_number_;
    uint64_t hash_;

    PieceMap& piece_map_;
};

} // namespace model

#include "model/position/z_hasher.inl"
