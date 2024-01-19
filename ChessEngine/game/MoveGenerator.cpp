#include "MoveGenerator.h"
#include "ChessEngine/bits/BitMasks.h"
#include "ChessEngine/bits/BitUtils.h"

/*
IDEA FOR CHECK-CHECKING

1. Imagine that the king is your only piece on the board.
2. Generate all moves for the king, raycast as if it was a rook.
3. If the rays hits sliding pieces, check if the enemy pieces are rooks or queens.
4. If that is the case, then check what what pieces you own along that ray, blocking the attack.
5. Save the ray, and tag the piece or pieces as blockers (perhaps in a bitboard). These pieces can now only move along the ray.
6. Repeat for the king as if it was a bishop.

Now when calculating moves that is not the king, you can check wheter or not a piece is a blocker,
and if it is, you can only move along the ray that it is blocking. Otherwise you do not
have to check for check.

For the king, perform a queens raycast after each move but with your own pieces as well.

I have to figure out what to do if I am actually in check, and there are no blockers.
I realized, that it is the first thing to check actually. If I am in check, I can only
attempt to find moves that could block the check, move the king or eliminate the threat.
*/


namespace game {
    MoveGenerator::MoveGenerator() {
        _moves = std::vector<Move>(MAX_LEGAL_MOVES);
        _board = ChessBoard();
    }

    std::vector<Move>& MoveGenerator::genMoves(bool is_white) {
        genRookMoves(is_white);

        return _moves;
    }

    void MoveGenerator::genRookMoves(bool is_white) {
        std::vector<int> indices;
        bits::U64 iso_rook_bitboard;
        std::vector<bits::U64> ray_bitmasks;
        std::vector<bool> ray_blocked;

        if (is_white) {
            indices = bits::getBitIndices(_board.getBitboard(PieceType::W_ROOK));
        } else {
            indices = bits::getBitIndices(_board.getBitboard(PieceType::B_ROOK));
        }
    }
}