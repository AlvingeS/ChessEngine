#include "model/position/z_hasher.h"

#include "model/position/piece_map.h"

#include <iostream>
#include <random>

namespace model {

ZHasher::ZHasher(PieceMap& piece_map) 
    : piece_map_(piece_map)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<uint64_t> dis(0, std::numeric_limits<uint64_t>::max());

    // Generate random numbers for board/piece table
    for (sq_idx sq = 0; sq < 64; sq++) {
        for (size_t j = 0; j < 12; j++) {
            random_board_piece_type_numbers_[sq][j] = dis(gen);
        }
    }

    // Generate random numbers for castle rights table
    for (size_t i = 0; i < 4; i++) {
        random_castle_rights_numbers_[i] = dis(gen);
    }

    // Generate random numbers for en passant file table
    for (size_t i = 0; i < 8; i++) {
        random_ep_file_numbers_[i] = dis(gen);
    }

    // Generate random number for is white table
    random_is_w_number_ = dis(gen);
    
    compute_initial_hash();
}

void ZHasher::compute_initial_hash()
{
    hash_ = 0;

    for (sq_idx sq = 0; sq < 64; sq++) {
        Piece::Type piece_type = piece_map_.get_piece_type_at(sq);

        if (piece_type != Piece::Type::EMPTY)
            hash_piece_type_at(sq, piece_type);
    }

    // We know that we have castle rights, so we can just add them
    for (size_t i = 0; i < 4; i++) {
        hash_castle_rights(i);
    }

    // There can be no en passant file

    // We know that it is white's turn
    hash_is_w(true);
}

} // namespace model