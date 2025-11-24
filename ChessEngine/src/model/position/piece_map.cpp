#include "model/position/piece_map.h"

namespace model {

PieceMap::PieceMap(const Bitboards& bbs) 
{
    fill_piece_map_from_bitboards(bbs);
}

void PieceMap::fill_piece_map_from_bitboards(const Bitboards& bbs) 
{
    piece_map_ = std::vector<Piece::Type>(64, Piece::Type::EMPTY);

    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 12; j++) {
            if (bbs.get_bb_from_idx(j) & (1ULL << i)) {
                piece_map_[i] = Piece::get_type_from_int(j);
                break;
            }
        }
    }
}

} // namespace model