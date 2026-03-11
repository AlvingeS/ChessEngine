#include "io/stockfish_perft_retriever.h"

#include "io/utils.h"
#include "model/position/piece_map.h"
#include "model/constants.h"

#include <sstream>
#include <regex>
#include <iostream>

namespace io::stockfish
{

namespace
{

std::string build_shell_command(const std::string& FEN, int depth) 
{
    std::ostringstream oss;
    
    oss << "echo \"position fen " << FEN
        << "\\ngo perft " << depth
        << "\\nquit\" | stockfish";

    return oss.str();
} 

std::string get_stockfish_output(const std::string& FEN, int depth)
{
    auto shellCommand = build_shell_command(FEN, depth);

    FILE* pipe = popen(shellCommand.c_str(), "r");

    if (!pipe) {
        std::cerr << "Couldn't open pipe to stockfish" << std::endl;
        return{}; // Return empty buffer
    }

    char buffer[256];
    std::string output = "";

    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        output += buffer;
    }

    pclose(pipe);

    return output;
}

model::Move notation_to_move(const std::string& notation, const model::PieceMap pm)
{
    std::string from = notation.substr(0, 2);
    std::string to = notation.substr(2, 2);

    char promo_to_piece = ' ';
    if (notation.length() == 5)
        promo_to_piece = notation[4];

    sq_idx from_sq = io::utils::sq_from_notation(from);
    sq_idx to_sq = io::utils::sq_from_notation(to);

    model::Piece::Type piece_type_from = pm.get_piece_type_at(from_sq);
    model::Piece::Type piece_type_to = pm.get_piece_type_at(to_sq);

    bool is_capture = pm.get_piece_type_at(to_sq) != model::Piece::Type::EMPTY;
    bool is_promo = promo_to_piece != ' ';

    if (is_promo) {
        if (is_capture) {
            switch (promo_to_piece) {
                case 'Q':
                    return model::Move(from_sq, to_sq, model::Move::QUEEN_PROMO_CAPTURE_FLAG);
                case 'R':
                    return model::Move(from_sq, to_sq, model::Move::ROOK_PROMO_CAPTURE_FLAG);
                case 'B':
                    return model::Move(from_sq, to_sq, model::Move::BISHOP_PROMO_CAPTURE_FLAG);
                case 'N':
                    return model::Move(from_sq, to_sq, model::Move::KNIGHT_PROMO_CAPTURE_FLAG);
                case 'q':
                    return model::Move(from_sq, to_sq, model::Move::QUEEN_PROMO_CAPTURE_FLAG);
                case 'r':
                    return model::Move(from_sq, to_sq, model::Move::ROOK_PROMO_CAPTURE_FLAG);
                case 'b':
                    return model::Move(from_sq, to_sq, model::Move::BISHOP_PROMO_CAPTURE_FLAG);
                case 'n':
                    return model::Move(from_sq, to_sq, model::Move::KNIGHT_PROMO_CAPTURE_FLAG);
            }
        } else {
            switch (promo_to_piece) {
                case 'Q':
                    return model::Move(from_sq, to_sq, model::Move::QUEEN_PROMO_FLAG);
                case 'R':
                    return model::Move(from_sq, to_sq, model::Move::ROOK_PROMO_FLAG);
                case 'B':
                    return model::Move(from_sq, to_sq, model::Move::BISHOP_PROMO_FLAG);
                case 'N':
                    return model::Move(from_sq, to_sq, model::Move::KNIGHT_PROMO_FLAG);
                case 'q':
                    return model::Move(from_sq, to_sq, model::Move::QUEEN_PROMO_FLAG);
                case 'r':
                    return model::Move(from_sq, to_sq, model::Move::ROOK_PROMO_FLAG);
                case 'b':
                    return model::Move(from_sq, to_sq, model::Move::BISHOP_PROMO_FLAG);
                case 'n':
                    return model::Move(from_sq, to_sq, model::Move::KNIGHT_PROMO_FLAG);
            }
        }
    }

    bool is_kside_castle = (from_sq         == constants::W_KING_START_SQ           &&
                            to_sq           == constants::W_KSIDE_KING_CASTLE_TO_SQ &&
                            piece_type_from == model::Piece::Type::W_KING)          || 
                           (from_sq         == constants::B_KING_START_SQ           &&
                            to_sq           == constants::B_KSIDE_KING_CASTLE_TO_SQ &&
                            piece_type_from == model::Piece::Type::B_KING);

    if (is_kside_castle)
        return model::Move(from_sq, to_sq, model::Move::KING_CASTLE_FLAG);

    bool is_qside_castle = (from_sq         == constants::W_KING_START_SQ           &&
                            to_sq           == constants::W_QSIDE_KING_CASTLE_TO_SQ &&
                            piece_type_from == model::Piece::Type::W_KING)          || 
                           (from_sq         == constants::B_KING_START_SQ           &&
                            to_sq           == constants::B_QSIDE_KING_CASTLE_TO_SQ &&
                            piece_type_from == model::Piece::Type::B_KING);

    if (is_qside_castle)
        return model::Move(from_sq, to_sq, model::Move::QUEEN_CASTLE_FLAG);

    bool is_ep_capture = (piece_type_from == model::Piece::Type::W_PAWN  ||
                          piece_type_from == model::Piece::Type::B_PAWN) &&
                         (std::abs(from_sq - to_sq) % 8 != 0)            &&
                          piece_type_to == model::Piece::Type::EMPTY;

    if (is_ep_capture)
        return model::Move(from_sq, to_sq, model::Move::EP_CAPTURE_FLAG);


    bool is_double_pawn_push = (piece_type_from == model::Piece::Type::W_PAWN  ||
                                piece_type_from == model::Piece::Type::B_PAWN) &&
                               (std::abs(from_sq - to_sq) == 16);

    if (is_double_pawn_push)
        return model::Move(from_sq, to_sq, model::Move::DOUBLE_PAWN_PUSH_FLAG);

    if (is_capture)
        return model::Move(from_sq, to_sq, model::Move::CAPTURE_FLAG);
    
    return model::Move(from_sq, to_sq, model::Move::QUITE_FLAG);
}

std::unordered_map<model::Move, uint64_t> parse_output_into_map(std::string stockfish_output, const model::PieceMap pm)
{
    std::unordered_map<model::Move, uint64_t> results{};
    std::regex pattern(R"((\w+): (\d+))");              // Finds <alphanumeric>: <digits> pattern
    std::smatch match;

    while (std::regex_search(stockfish_output, match, pattern)) {
        if (match.size() == 3) {                        // Full match + 2 subgroups
            std::string move_str = match[1].str();      // <alphanumeric>
            if (move_str.size() > 5)
                break;

            long count = std::stol(match[2].str());     // <digits>
            results[notation_to_move(move_str, pm)] = count;
        }
        stockfish_output = match.suffix().str();         // Replace output with everything
                                                        // after the matched row
    }

    return results; 
}

std::vector<model::Move> parse_output_into_vector(std::string stockfish_output, const model::PieceMap pm)
{
    std::vector<model::Move> results{};
    std::regex pattern(R"((\w+): (\d+))");              // Finds <alphanumeric>: <digits> pattern
    std::smatch match;

    while (std::regex_search(stockfish_output, match, pattern)) {
        if (match.size() == 3) {                        // Full match + 2 subgroups
            std::string move_str = match[1].str();      // <alphanumeric>
            if (move_str.size() > 5)
                break;

            results.push_back(notation_to_move(move_str, pm));
        }
        stockfish_output = match.suffix().str();         // Replace output with everything
                                                         // after the matched row
    }

    return results; 
}

char col_to_char(int col) {
    switch (col) {
        case 0:
            return 'a';
        case 1:
            return 'b';
        case 2:
            return 'c';
        case 3:
            return 'd';
        case 4:
            return 'e';
        case 5:
            return 'f';
        case 6:
            return 'g';
        case 7:
            return 'h';
        default:
            return 'x';
    }
}

} // namespace


std::unordered_map<model::Move, uint64_t> get_perft_results(const std::string& FEN, int depth, const model::PieceMap pm) 
{
    const auto stockfish_output = get_stockfish_output(FEN, depth);
    return parse_output_into_map(stockfish_output, pm);
}

std::vector<model::Move> get_legal_moves_from_fen(const std::string& FEN, const model::PieceMap pm)
{
    const auto stockfish_output = get_stockfish_output(FEN, 1);
    return parse_output_into_vector(stockfish_output, pm);
}

std::string move_to_str(model::Move move, bool w_started) 
{
    if (move.is_any_castle()) {
        return move.is_king_castle() ? (w_started ? "e1g1" : "e8g8") 
                                    : (w_started ? "e1c1" : "e8c8");
    }
    
    int from = move.get_from_sq();
    int to = move.get_to_sq();
    
    int from_row = from / 8;
    int from_col = from % 8;

    char from_col_char = col_to_char(from_col);

    int to_row = to / 8;
    int to_col = to % 8;
    char to_col_char = col_to_char(to_col);

    std::string move_str = "";
    move_str += from_col_char;
    move_str += std::to_string(from_row + 1);
    move_str += to_col_char;
    move_str += std::to_string(to_row + 1);

    if (move.is_any_promo()) {
        switch (move.get_flag()) {
            case model::Move::KNIGHT_PROMO_FLAG:
                move_str += (w_started) ? "n" : "N";
                break;
            case model::Move::BISHOP_PROMO_FLAG:
                move_str += (w_started) ? "b" : "B";
                break;
            case model::Move::ROOK_PROMO_FLAG:
                move_str += (w_started) ? "r" : "R";
                break;
            case model::Move::QUEEN_PROMO_FLAG:
                move_str += (w_started) ? "q" : "Q";
                break;
            default:
                break;
            case model::Move::KNIGHT_PROMO_CAPTURE_FLAG:
                move_str += (w_started) ? "n" : "N";
                break;
            case model::Move::BISHOP_PROMO_CAPTURE_FLAG:
                move_str += (w_started) ? "b" : "B";
                break;
            case model::Move::ROOK_PROMO_CAPTURE_FLAG:
                move_str += (w_started) ? "r" : "R";
                break;
            case model::Move::QUEEN_PROMO_CAPTURE_FLAG:
                move_str += (w_started) ? "q" : "Q";
                break;
        }
    }

    return move_str;
}

std::pair<bool, std::string> compare_first_move_counts_to_stockfish(
    const std::unordered_map<model::Move, uint64_t>& first_move_counts,
    const std::unordered_map<model::Move, uint64_t>& stockfish_results,
    bool w_started)
{
    std::ostringstream errors;
    bool has_errors = false;

    for (const auto& move_count_pair : first_move_counts) {
        const model::Move& move = move_count_pair.first;
        uint64_t count = move_count_pair.second;

        auto fount_it = stockfish_results.find(move);
        if (fount_it == stockfish_results.end()) {
            errors << "Move: " << io::stockfish::move_to_str(move, w_started) << " not found in stockfish results.\n";
            has_errors = true;
        } else {
            uint64_t stockfishCount = fount_it->second;
            if (count != stockfishCount) {
                errors << "Move: " << io::stockfish::move_to_str(move, w_started) << " failed. Expected: " << stockfishCount << ", Got: " << count << ".\n";
                has_errors = true;
            }
        }
    }

    return {has_errors, errors.str()};
}

} // namespace io::stockfish