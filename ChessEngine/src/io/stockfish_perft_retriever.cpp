#include "io/stockfish_perft_retriever.h"

#include "io/utils.h"
#include "model/position/piece_map.h"

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

    bool is_kside_castle = (from_sq == 3  && to_sq == 1  && piece_type_from == model::Piece::Type::W_KING)
                        || (from_sq == 59 && to_sq == 57 && piece_type_from == model::Piece::Type::B_KING);
    if (is_kside_castle)
        return model::Move(from_sq, to_sq, model::Move::KING_CASTLE_FLAG);

    bool is_qside_castle = (from_sq == 3  && to_sq == 5  && piece_type_from == model::Piece::Type::W_KING)
                        || (from_sq == 59 && to_sq == 61 && piece_type_from == model::Piece::Type::B_KING);
    if (is_qside_castle)
        return model::Move(from_sq, to_sq, model::Move::QUEEN_CASTLE_FLAG);

    bool is_ep_capture = (piece_type_from == model::Piece::Type::W_PAWN || piece_type_from == model::Piece::Type::B_PAWN)
                      && (std::abs(from_sq - to_sq) % 8 != 0)
                      && piece_type_to == model::Piece::Type::EMPTY;
    if (is_ep_capture)
        return model::Move(from_sq, to_sq, model::Move::EP_CAPTURE_FLAG);


    bool is_double_pawn_push = (piece_type_from == model::Piece::Type::W_PAWN || piece_type_from == model::Piece::Type::B_PAWN)
                            && (std::abs(from_sq - to_sq) == 16);
    if (is_double_pawn_push)
        return model::Move(from_sq, to_sq, model::Move::DOUBLE_PAWN_PUSH_FLAG);

    if (is_capture)
        return model::Move(from_sq, to_sq, model::Move::CAPTURE_FLAG);
    
    return model::Move(from_sq, to_sq, model::Move::QUITE_FLAG);
}

std::unordered_map<model::Move, uint64_t> parse_output(std::string stockfishOutput, const model::PieceMap pm)
{
    std::unordered_map<model::Move, uint64_t> results{};
    std::regex pattern(R"((\w+): (\d+))");              // Finds <alphanumeric>: <digits> pattern
    std::smatch match;

    while (std::regex_search(stockfishOutput, match, pattern)) {
        if (match.size() == 3) {                        // Full match + 2 subgroups
            std::string move_str = match[1].str();      // <alphanumeric>
            long count = std::stol(match[2].str());     // <digits>
            results[notation_to_move(move_str, pm)] = count;
        }
        stockfishOutput = match.suffix().str();         // Replace output with everything
                                                        // after the matched row
    }

    return results; 
}

} // namespace


std::unordered_map<model::Move, uint64_t> get_perft_results(const std::string& FEN, int depth, const model::PieceMap pm) 
{
    const auto stockfishOutput = get_stockfish_output(FEN, depth);
    return parse_output(stockfishOutput, pm);
}

} // namespace io::stockfish