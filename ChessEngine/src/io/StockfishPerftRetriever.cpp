#include "io/StockfishPerftRetriever.h"

#include <sstream>
#include <regex>
#include <iostream>

namespace io::stockfish
{

namespace
{

std::string buildShellCommand(const std::string& FEN, int depth) 
{
    std::ostringstream oss;
    
    oss << "echo \"position fen " << FEN
        << "\\ngo perft " << depth
        << "\\nquit\" | stockfish";

    return oss.str();
} 

std::string getStockfishOutput(const std::string& FEN, int depth)
{
    auto shellCommand = buildShellCommand(FEN, depth);

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

std::unordered_map<std::string, uint64_t> parseOutput(std::string stockfishOutput)
{
    std::unordered_map<std::string, uint64_t> results{};
    std::regex pattern(R"((\w+): (\d+))");              // Finds <alphanumeric>: <digits> pattern
    std::smatch match;

    while (std::regex_search(stockfishOutput, match, pattern)) {
        if (match.size() == 3) {                        // Full match + 2 subgroups
            std::string move = match[1].str();          // <alphanumeric>
            long count = std::stol(match[2].str());     // <digits>
            results[move] = count;
        }
        stockfishOutput = match.suffix().str();         // Replace output with everything
                                                        // after the matched row
    }

    return results; 
}

} // namespace


std::unordered_map<std::string, uint64_t> getPerftResults(const std::string& FEN, int depth) 
{
    const auto stockfishOutput = getStockfishOutput(FEN, depth);
    return parseOutput(stockfishOutput);
}

} // namespace io::stockfish