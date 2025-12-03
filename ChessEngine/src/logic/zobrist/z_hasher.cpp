#include "logic/zobrist/z_hasher.h"

#include "model/position/piece_map.h"

#include "logic/masks.h"
#include "logic/utils.h"

#include <random>

namespace logic {

ZHasher::ZHasher(const model::Position& pos) 
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<uint64_t> dis(0, std::numeric_limits<uint64_t>::max());

    // Generate random numbers for board/piece table
    for (sq_idx sq = 0; sq < 64; sq++) {
        for (size_t j = 0; j < 12; j++) {
            square_piece_type_keys[sq][j] = dis(gen);
        }
    }

    // Generate random numbers for castle rights table
    for (size_t i = 0; i < 4; i++) {
        c_right_keys[i] = dis(gen);
    }

    // Generate random numbers for en passant file table
    for (size_t i = 0; i < 8; i++) {
        ep_file_keys[i] = dis(gen);
    }

    // Generate random number for is white table
    side_to_move_key = dis(gen);
    
    compute_initial_hash(pos);
}

void ZHasher::compute_initial_hash(const model::Position& pos)
{
    z_hash_ = 0ULL;

    // XOR each square with a piece on it from square_piece_type_keys
    for (sq_idx sq = 0; sq < 64; sq++) {
        model::Piece::Type piece_type = pos.piece_map.get_piece_type_at(sq);

        if (piece_type != model::Piece::Type::EMPTY)
            xor_piece_type_at(sq, piece_type);
    }

    // Go through each castle right and XOR it using castle_right_keys
    if ((pos.c_rights & masks::W_KSIDE_CASTLE_RIGHTS_MASK) != 0)
        xor_castle_right(W_KSIDE);
        
    if ((pos.c_rights & masks::W_QSIDE_CASTLE_RIGHTS_MASK) != 0)
        xor_castle_right(W_QSIDE);

    if ((pos.c_rights & masks::B_KSIDE_CASTLE_RIGHTS_MASK) != 0)
        xor_castle_right(B_KSIDE);
    
    if ((pos.c_rights & masks::B_QSIDE_CASTLE_RIGHTS_MASK) != 0)
        xor_castle_right(B_QSIDE);

    // If ep target mask is not 0, determine the file and XOR using ep_file_keys
    if (pos.ep_target_mask != 0ULL)
        xor_ep_file_at(utils::file_from_sq(utils::lsb_idx(pos.ep_target_mask)));

    // Base hash is meant as it is whites turn to move, so if black, toggle
    if (!pos.is_w)
        toggle_side_to_move();
}

void ZHasher::update_castle_rights(castle_rights old_rights, castle_rights new_rights)
{
    castle_rights lost = old_rights & ~new_rights;

    if (lost & masks::W_KSIDE_CASTLE_RIGHTS_MASK) xor_castle_right(W_KSIDE);
    if (lost & masks::W_QSIDE_CASTLE_RIGHTS_MASK) xor_castle_right(W_QSIDE);
    if (lost & masks::B_KSIDE_CASTLE_RIGHTS_MASK) xor_castle_right(B_KSIDE);
    if (lost & masks::B_QSIDE_CASTLE_RIGHTS_MASK) xor_castle_right(B_QSIDE);
}


} // namespace logic