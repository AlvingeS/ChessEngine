#include "engine/uci/uci.h"

#include "io/fen.h"
#include "logic/attack_tables/attack_tables.h"
#include "logic/movegen/move_gen.h"
#include "logic/makemove/move_maker.h"
#include "logic/makemove/move_retractor.h"
#include "logic/zobrist/z_hasher.h"

#include "model/position/position.h"
#include "model/move/movelist.h"
#include "model/move/move.h"

#include <iostream>
#include <sstream>
#include <string>

namespace engine::uci {

namespace {

const std::string START_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
const int MAX_DEPTH = 12;

std::string sq_to_str(sq_t sq) {
    std::string s;
    s += char('a' + (sq % 8));
    s += char('1' + (sq / 8));
    return s;
}

} // namespace

std::string move_to_uci(const model::Move& move) {
    std::string s = sq_to_str(move.from()) + sq_to_str(move.to());

    if (move.is_any_promo()) {
        int f = move.flag();
        if (f == model::Move::KNIGHT_PROMO_FLAG || 
            f == model::Move::KNIGHT_PROMO_CAPTURE_FLAG) s += 'n';
        else if (f == model::Move::BISHOP_PROMO_FLAG || 
                 f == model::Move::BISHOP_PROMO_CAPTURE_FLAG) s += 'b';
        else if (f == model::Move::ROOK_PROMO_FLAG || 
                 f == model::Move::ROOK_PROMO_CAPTURE_FLAG) s += 'r';
        else s += 'q';
    }

    return s;
}

model::Move parse_uci_move(
    const std::string& str, 
    model::Movelist& legal_moves) 
{
    int from_file = str[0] - 'a';
    int from_rank = str[1] - '1';
    int to_file   = str[2] - 'a';
    int to_rank   = str[3] - '1';

    sq_t from_sq = from_rank * 8 + from_file;
    sq_t to_sq   = to_rank   * 8 + to_file;

    char promo = str.length() > 4 ? str[4] : 0;

    for (int i = 0; i < constants::MAX_LEGAL_MOVES; i++) {
        const auto& m = legal_moves.get_move_at(i);

        if (m.value() == 0) {
            break;
        }

        if (m.from() == from_sq && m.to() == to_sq) {
            if (!m.is_any_promo()) return m;

            if (promo == 'q' && (m.flag() == model::Move::QUEEN_PROMO_FLAG || 
                m.flag() == model::Move::QUEEN_PROMO_CAPTURE_FLAG)) return m;
            if (promo == 'r' && (m.flag() == model::Move::ROOK_PROMO_FLAG || 
                m.flag() == model::Move::ROOK_PROMO_CAPTURE_FLAG)) return m;
            if (promo == 'b' && (m.flag() == model::Move::BISHOP_PROMO_FLAG || 
                m.flag() == model::Move::BISHOP_PROMO_CAPTURE_FLAG)) return m;
            if (promo == 'n' && (m.flag() == model::Move::KNIGHT_PROMO_FLAG || 
                m.flag() == model::Move::KNIGHT_PROMO_CAPTURE_FLAG)) return m;
        }
    }

    return model::Move();
}

void uci_loop() {
    engine::MovePicker move_picker(MAX_DEPTH);

    std::string line;

    while (std::getline(std::cin, line)) {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        std::istringstream iss(line);
        std::string cmd;
        iss >> cmd;

        if (cmd == "uci") {
            std::cout << "id name Smorklaren" << std::endl;
            std::cout << "id author Alvinge" << std::endl;
            std::cout << "uciok" << std::endl;
        }
        else if (cmd == "isready") {
            std::cout << "readyok" << std::endl;
        }
        else if (cmd == "ucinewgame") {
            move_picker.reset_position();
            move_picker.reset_stacks();
        }
        else if (cmd == "position") {
            std::string token;
            iss >> token;

            if (token == "startpos") {
                move_picker.set_pos_from_fen(START_FEN);
                move_picker.z_hash_from_position();

                // Try to consume "moves" token
                iss >> token;
            }
            else if (token == "fen") {
                std::string fen;
                std::string part;
                // FEN has 6 space-separated fields
                for (int i = 0; i < 6 && (iss >> part); i++) {
                    if (i > 0) fen += " ";
                    fen += part;
                }

                move_picker.set_pos_from_fen(fen);
                move_picker.z_hash_from_position();

                // Try to consume "moves" token
                iss >> token;
            }

            // Apply moves if present (token should be "moves" at this point,
            // or empty if no moves follow)
            std::string move_str;
            while (iss >> move_str) {
                // Generate legal moves from current position
                auto move_list = move_picker.gen_moves();

                // Find matching move and apply it
                model::Move move = parse_uci_move(move_str, move_list);

                if (move.value() != 0) {
                    move_picker.make_move(move);
                }
            }
        }
        else if (cmd == "go") {
            std::string token;
            int depth = 5;

            while (iss >> token) {
                if (token == "depth") { 
                    iss >> depth; 
                }
                else if (token == "wtime" || token == "btime" || 
                        token == "winc"  || token == "binc"  || 
                        token == "movestogo" || token == "movetime" ||
                        token == "nodes") {
                    // Consume the value that follows these tokens
                    std::string skip;
                    iss >> skip;
                }
                // "infinite", "ponder" etc have no value — just ignore
            }

            model::Move best = move_picker.pick_move(depth);
            std::cout << "bestmove " << move_to_uci(best) << std::endl;
        }
        else if (cmd == "quit") {
            break;
        }
        // Unknown commands are silently ignored per UCI spec
    }
}

} // namespace engine::uci