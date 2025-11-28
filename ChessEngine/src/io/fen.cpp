#include "io/fen.h"

#include "model/position/piece_type.h"

namespace io::fen {

void set_board_from_fen(
    const std::string& fen, 
    model::Position& pos) 
{
    pos.bbs.reset_bitboards();

    int rank = 7;
    int file = 0;
    for (char c : fen) {
        if (c == ' ') {
            break;
        } else if (c == '/') {
            rank--;
            file = 0;
        } else if (std::isdigit(c)) {
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
}

std::string get_fen_from_board(const model::PieceMap& piece_map)
{
    std::string fen = "";
    int empty_count = 0;

    for (sq_idx sq = 63; sq >= 0; sq--) {
        if ((sq + 1) % 8 == 0 && sq != 63) {
            if (empty_count != 0) {
                fen += std::to_string(empty_count);
                empty_count = 0;
            }
            fen += "/";
        }

        model::Piece::Type type = piece_map.get_piece_type_at(sq);
        if (type == model::Piece::Type::EMPTY) {
            empty_count++;
        } else {
            if (empty_count != 0) {
                fen += std::to_string(empty_count);
                empty_count = 0;
            }
            fen += model::Piece::get_char_from_type(type);
        }
    }

    return fen;
}

} // namespace io::fen