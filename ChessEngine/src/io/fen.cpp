#include "io/fen.h"

#include "io/utils.h"
#include "model/position/piece_type.h"
#include "logic/masks.h"
#include <sstream>

namespace io::fen {

namespace 
{
    


castle_rights c_rights_from_notation(const std::string& c_rights_notation)
{
    castle_rights c_rights = 0b0000;

    if (c_rights_notation == "-") {
        return c_rights;
    } else {
        if (c_rights_notation.find('K') != std::string::npos)
            c_rights |= logic::masks::W_KSIDE_CASTLE_RIGHTS_MASK;
        
        if (c_rights_notation.find('Q') != std::string::npos)
            c_rights |= logic::masks::W_QSIDE_CASTLE_RIGHTS_MASK;

        if (c_rights_notation.find('k') != std::string::npos)
            c_rights |= logic::masks::B_KSIDE_CASTLE_RIGHTS_MASK;

        if (c_rights_notation.find('q') != std::string::npos)
            c_rights |= logic::masks::B_QSIDE_CASTLE_RIGHTS_MASK;
    }

    return c_rights;
}

} // namespace

void set_pos_from_fen(const std::string& fen, model::Position& pos) 
{
    pos.bbs.reset_bitboards();

    pos.is_w = true;
    pos.c_rights = 0b1111;
    pos.ep_target_mask = 0ULL;
    pos.halfmove_clock = 0;

    std::stringstream ss(fen);
    std::vector<std::string> tokens;
    std::string temp;

    while (ss >> temp)
        tokens.push_back(temp);

    if (tokens.empty())
        return;

    const std::string& board_str = tokens[0];

    int rank = 7;
    int file = 0;
    for (char c : board_str) {
        if (c == '/') {
            rank--;
            file = 0;
        } else if (std::isdigit(static_cast<unsigned char>(c))) {
            file += c - '0';
        } else {
            model::Piece::Type type = model::Piece::get_type_from_char(c);
            pos.bbs.set_piece_type_bit(rank * 8 + 7 - file, type);
            file++;
        }
    }

    pos.piece_map.fill_piece_map_from_bitboards(pos.bbs);
    pos.occ_masks.fill_b_pieces_mask_from_bbs(pos.bbs);
    pos.occ_masks.fill_w_pieces_mask_from_bbs(pos.bbs);
    pos.occ_masks.update_occupancy_masks();

    if (tokens.size() > 1) {
        const std::string& color_str = tokens[1];
        if (color_str == "w") pos.is_w = true;
        else if (color_str == "b") pos.is_w = false;
    }

    if (tokens.size() > 2) {
        pos.c_rights = c_rights_from_notation(tokens[2]);
    }

    if (tokens.size() > 3) {
        const std::string& ep = tokens[3];
        if (ep != "-" && ep.size() == 2 &&
            ep[0] >= 'a' && ep[0] <= 'h' &&
            ep[1] >= '1' && ep[1] <= '8') {
            pos.ep_target_mask = (1ULL << io::utils::sq_from_notation(ep));
        } else {
            pos.ep_target_mask = 0ULL;
        }
    }

    if (tokens.size() > 4) {
        try {
            pos.halfmove_clock = std::stoi(tokens[4]);
        } catch (...) {
            pos.halfmove_clock = 0;
        }
    }
}


} // namespace io::fen