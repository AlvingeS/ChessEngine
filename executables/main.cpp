#include "logic/movegen/move_gen.h"
#include "engine/perft/perft.h"
#include "io/board_printer.h"
#include "logic/utils.h"

#include <iostream>
#include <chrono>
#include <locale>

namespace {

struct space_separator : std::numpunct<char> {
    char do_thousands_sep() const override { return ' '; }
    std::string do_grouping() const override { return "\003"; }
};

} // namespace

int main() 
{
    std::cout.imbue(std::locale(std::cout.getloc(), new space_separator()));

    /*
    Total number of nodes for depth 4: 6605806

    Changelog:
        - 2024-04-08: 
            First unoptimized runs: 
            3.803s ~ 1.736M nodes/s ~ 0.7% SF

            Added option to disable perft stats recording, will not use from now on
            2.937s ~ 2,249M nodes/s ~ 0.9% SF

            Removed unnecessary copying of bitboard in make/unmake move
            2.881s ~ 2,345M nodes/s ~ 0.9% SF

            Now handle white/black bitmasks directly in make/unmake move
            2.603s ~ 2,567M nodes/s ~ 1.0% SF
        - 2024-04-10:
            Removed unnecessary vector copying in pawn-move generation
            2.469s ~ 2,678M nodes/s ~ 1.1% SF

            Changed the ordering of the move flags to check for captures,
            promotions, or castles in groups
            2.139s ~ 3,118M nodes/s ~ 1.2% SF

            Made getBitBoard inline
            2.122s ~ 3,121M nodes/s ~ 1.2% SF

        - 2024-04-14
            Completed refactoring of movegeneration, don't know what caused slight speedup
            2.063s ~ 3.300M nodes/s ~ 1.3% SF

        - 2024-05-08
            Refactored acessing bbs to use pointers directly instead of lookup
            1.940s ~ 3.471M nodes/s ~ 1.4% SF

            Removed castling logic that I had forgotten about, it is all handled by the perft
            1.783s ~ 3.686M nodes/s ~ 1.5% SF

        - 2024-05-12
            !!! SWITCHED TO RELEASE MODE FOR PERFORMANCE TESTING !!!
            Previous commit had the following stats in release mode
            0.341s ~ 19.291M nodes/s ~ 7.8% SF

            Refactored Chessboard into an additional movemaker class, however
            performance has dropped so this will be worked on in the future
            0.421s ~ 16.102M nodes/s ~ 6.4% SF

            Moved castling flags to last so I only have to check if flag is
            greater than 14 to see if it is a castle
            0.346s ~ 19.145M nodes/s ~ 7.8% SF

            Refactored perft so that castling rights and last captured pieces info
            is now in its on class, will add everything else as well
            0.337s ~ 20.009M nodes/s ~ 8.0% SF

            Refactored Chessboard intro three separate classes, now a little bit slower
            but for sure worth it
            0.364s ~ 19.1239M nodes/s ~ 7.7% SF

            Lot's of refactoring, killed performance a bit but a lot cleaner
            0.474s 14.86M nodes/s ~ 5.9% SF

            Done with major refactoring into model/logic/engine/io
            0.426s ~ 16.21 nodes/s ~ 6.5% SF

            Using work computer, bunch of shit just picking this up again now
            0.358 ~ 18.14M nodes/s ~ 7.2% SF

            Reworked movegen, now only produce legal moves but test castle and ep instead of all for check
            0.245s ~ 26.38M nodes/s ~ 10.5% SF (0.308s on pc)
    */

    // Start clock
    auto start = std::chrono::high_resolution_clock::now();

    logic::attack_tables::init_attack_tables();

    std::string start_pos  = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    std::string pos2  = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -";
    std::string pos3  = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1";
    std::string pos4  = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1";
    std::string pos5  = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8";
    std::string pos6  = "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10";
    
    engine::Perft perft_start_pos = engine::Perft(5);
    perft_start_pos.set_pos_from_fen(start_pos);
    perft_start_pos.minimax(0, 0, false);

    engine::Perft perft_pos2 = engine::Perft(4);
    perft_pos2.set_pos_from_fen(pos2);
    perft_pos2.minimax(0, 0, false);
    
    engine::Perft perft_pos3 = engine::Perft(6);
    perft_pos3.set_pos_from_fen(pos3);
    perft_pos3.minimax(0, 0, false);

    engine::Perft perft_pos4 = engine::Perft(5);
    perft_pos4.set_pos_from_fen(pos4);
    perft_pos4.minimax(0, 0, false);

    engine::Perft perft_pos5 = engine::Perft(4);
    perft_pos5.set_pos_from_fen(pos5);
    perft_pos5.minimax(0, 0, false);

    engine::Perft perft_pos6 = engine::Perft(4);
    perft_pos6.set_pos_from_fen(pos6);
    perft_pos6.minimax(0, 0, false);

    long long sum = 0LL;
    sum += perft_start_pos.sum_nodes_to_depth(5);
    sum += perft_pos2.sum_nodes_to_depth(4);
    sum += perft_pos3.sum_nodes_to_depth(6);
    sum += perft_pos4.sum_nodes_to_depth(5);
    sum += perft_pos5.sum_nodes_to_depth(4);
    sum += perft_pos6.sum_nodes_to_depth(4);
    std::cout << "Total nodes: " << sum << std::endl;

    // End clock
    auto end = std::chrono::high_resolution_clock::now();

    // Calculate duration
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Elapsed time: " << elapsed_seconds.count() << "s" << std::endl;
    int nodes_per_s = sum / elapsed_seconds.count();
    float nodes_per_s_float = sum / elapsed_seconds.count();
    std::cout << "Nodes per second: " << nodes_per_s << std::endl;
    std::cout << "SF%: " << (nodes_per_s_float / 250000000) * 100 << std::endl;

    return 0;
}
