#include <unordered_map>
#include "ChessEngine/game/piecetype.h"

namespace game {
typedef unsigned long long U64;

class ChessBoard {
    public:
        ChessBoard();

    private:
        std::unordered_map<PieceType, U64> bitboards;

        bool w_to_move;
};
} // namespace game