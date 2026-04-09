#include "engine/uci/uci.h"

#include "engine/uci/go_params.h"
#include "engine/search/time_manager.h"

#include "io/fen.h"
#include "logic/attack_tables/attack_tables.h"
#include "logic/movegen/move_gen.h"
#include "logic/makemove/move_maker.h"
#include "logic/makemove/move_retractor.h"
#include "logic/zobrist/z_hasher.h"

#include "model/position/position.h"
#include "model/move/movelist.h"
#include "model/move/move.h"

#include <thread>
#include <atomic>
#include <iostream>
#include <sstream>
#include <string>

namespace engine::uci {

namespace {

const std::string START_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

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
    std::thread search_thread;
    engine::Searcher searcher;
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
            std::cout << "option name Move Overhead type spin default 30 min 0 max 10000\n";
            std::cout << "option name Threads type spin default 1 min 1 max 1\n";
            std::cout << "option name Hash type spin default 16 min 1 max 33554432\n";
            std::cout << "uciok" << std::endl;
        }
        else if (cmd == "setoption") {
            std::string token;
            std::string name;
            std::string value;

            // consume "name"
            iss >> token;

            // read the option name (could be multiple words, stops at "value")
            while (iss >> token && token != "value") {
                if (!name.empty()) name += " ";
                name += token;
            }

            // read the value
            iss >> value;

            if (name == "Hash") {
                searcher.resize_tt(std::stoi(value));
            }
        }        
        else if (cmd == "isready") {
            std::cout << "readyok" << std::endl;
        }
        else if (cmd == "ucinewgame") {
            searcher.reset_position();
            searcher.reset_stacks();
            searcher.clear_tt();
            searcher.clear_game_hist();
        }
        else if (cmd == "position") {
            std::string token;
            iss >> token;

            if (token == "startpos") {
                searcher.set_pos_from_fen(START_FEN);
                searcher.z_hash_from_position();

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

                searcher.set_pos_from_fen(fen);
                searcher.z_hash_from_position();

                // Try to consume "moves" token
                iss >> token;
            }

            // Apply moves if present (token should be "moves" at this point,
            // or empty if no moves follow)
            std::string move_str;
            while (iss >> move_str) {
                // Generate legal moves from current position
                auto move_list = searcher.gen_moves();

                // Find matching move and apply it
                model::Move move = parse_uci_move(move_str, move_list);

                if (move.value() != 0) {
                    searcher.make_move(move);
                }
            }
        }
        else if (cmd == "go") {
            std::string token;
            GoParams go_params;

            auto read_int = [&](std::optional<int>& param) {
                int val;
                if (iss >> val) param = val;
            };

            while (iss >> token) {
                if      (token == "wtime")     read_int(go_params.wtime);
                else if (token == "btime")     read_int(go_params.btime);
                else if (token == "winc")      read_int(go_params.winc);
                else if (token == "binc")      read_int(go_params.binc);
                else if (token == "movestogo") read_int(go_params.movestogo);
                else if (token == "movetime")  read_int(go_params.movetime);
                else if (token == "depth")     read_int(go_params.depth);
                else if (token == "infinite")  go_params.infinite = true;
            }

            // If a search is already running, stop it first
            if (search_thread.joinable()) {
                searcher.request_stop();
                search_thread.join();
            }

            auto tm = TimeManager(go_params, searcher.get_is_w());

            search_thread = std::thread([&searcher, tm]() mutable {
                model::Move best = searcher.search(tm);
                auto move_notation = move_to_uci(best);
                std::cout << "bestmove " << move_notation << std::endl;
            });
        }
        else if (cmd == "stop") {
            if (search_thread.joinable()) {
                searcher.request_stop();
                search_thread.join();
            }
        }
        else if (cmd == "bench") {
            if (search_thread.joinable()) {
                searcher.request_stop();
                search_thread.join();
            }
            
            searcher.bench();
            break;
        }
        else if (cmd == "quit") {
            if (search_thread.joinable()) {
                searcher.request_stop();
                search_thread.join();
            }
            break;
        }
    }
}

} // namespace engine::uci