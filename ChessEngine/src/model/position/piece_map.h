#pragma once

#include "model/position/bitboards.h"
#include "model/position/piece_type.h"

namespace model {

class Bitboards;

class PieceMap {

public:
    PieceMap(const Bitboards& bitboards);

    Piece::Type get_piece_type_at_index(int index) const;
    
    void set_piece_type_at_index(int index, Piece::Type piece_type);
    void fill_piece_map_from_bitboards(const Bitboards& bitboards);

private:
    std::vector<Piece::Type> piece_map_;
};

} // namespace model

#include "model/position/piece_map.inl"