#pragma once

#include "model/position/bitboards.h"
#include "model/position/piece_type.h"

namespace model {

class Bitboards;

class PieceMap {

public:
    PieceMap(const Bitboards& bbs);

    Piece::Type get_piece_type_at(sq_idx sq) const;
    
    void set_piece_type_at(sq_idx sq, Piece::Type piece_type);
    void fill_piece_map_from_bitboards(const Bitboards& bbs);

private:
    std::vector<Piece::Type> piece_map_;
};

} // namespace model

#include "model/position/piece_map.inl"