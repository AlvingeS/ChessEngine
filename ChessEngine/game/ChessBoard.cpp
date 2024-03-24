#include "ChessBoard.h"

namespace game {

    ChessBoard::ChessBoard() {
        _bitboards = std::vector<bits::U64>(12, 0ULL);
        initPieceBitboards();
    }

    void ChessBoard::initPieceBitboards() {
        _bitboards[pieceTypeToInt(PieceType::W_PAWN)] = 0x000000000000FF00ULL;
        _bitboards[pieceTypeToInt(PieceType::W_KNIGHT)] = 0x0000000000000042ULL;
        _bitboards[pieceTypeToInt(PieceType::W_BISHOP)] = 0x0000000000000024ULL;
        _bitboards[pieceTypeToInt(PieceType::W_ROOK)] = 0x0000000000000081ULL;
        _bitboards[pieceTypeToInt(PieceType::W_QUEEN)] = 0x0000000000000010ULL;
        _bitboards[pieceTypeToInt(PieceType::W_KING)] = 0x0000000000000008ULL;

        _bitboards[pieceTypeToInt(PieceType::B_PAWN)] = 0x00FF000000000000ULL;
        _bitboards[pieceTypeToInt(PieceType::B_KNIGHT)] = 0x4200000000000000ULL;
        _bitboards[pieceTypeToInt(PieceType::B_BISHOP)] = 0x2400000000000000ULL;
        _bitboards[pieceTypeToInt(PieceType::B_ROOK)] = 0x8100000000000000ULL;
        _bitboards[pieceTypeToInt(PieceType::B_QUEEN)] = 0x1000000000000000ULL;
        _bitboards[pieceTypeToInt(PieceType::B_KING)] = 0x0800000000000000ULL;
        
        _enPessantTarget = 0ULL;
        _whiteRookAMoved = false;
        _whiteRookHMoved = false;
        _blackRookAMoved = false;
        _blackRookHMoved = false;
        _whiteKingMoved = false;
        _blackKingMoved = false;
        _whiteHasCastled = false;
        _blackHasCastled = false;

        // Sets piecetype lookup to zero vector 64 squares
        _squaresLookup = std::vector<PieceType>(64, PieceType::EMPTY);
        fillSquaresLookup();
    }

    void ChessBoard::makeCastleMove(bool isWhite, bool isKingSide) {
        if (isWhite) {
            if (isKingSide) {
                _bitboards[pieceTypeToInt(PieceType::W_KING)] &= ~(1ULL << 3);
                _bitboards[pieceTypeToInt(PieceType::W_KING)] |= (1ULL << 1);
                _bitboards[pieceTypeToInt(PieceType::W_ROOK)] &= ~(1ULL);
                _bitboards[pieceTypeToInt(PieceType::W_ROOK)] |= (1ULL << 2);
                _squaresLookup[3] = PieceType::EMPTY;
                _squaresLookup[1] = PieceType::W_KING;
                _squaresLookup[0] = PieceType::EMPTY;
                _squaresLookup[2] = PieceType::W_ROOK;
            } else {
                _bitboards[pieceTypeToInt(PieceType::W_KING)] &= ~(1ULL << 3);
                _bitboards[pieceTypeToInt(PieceType::W_KING)] |= (1ULL << 5);
                _bitboards[pieceTypeToInt(PieceType::W_ROOK)] &= ~(1ULL << 7);
                _bitboards[pieceTypeToInt(PieceType::W_ROOK)] |= (1ULL << 4);
                _squaresLookup[3] = PieceType::EMPTY;
                _squaresLookup[5] = PieceType::W_KING;
                _squaresLookup[7] = PieceType::EMPTY;
                _squaresLookup[4] = PieceType::W_ROOK;
            }
        } else {
            if (isKingSide) {
                _bitboards[pieceTypeToInt(PieceType::B_KING)] &= ~(1ULL << 59);
                _bitboards[pieceTypeToInt(PieceType::B_KING)] |= (1ULL << 57);
                _bitboards[pieceTypeToInt(PieceType::B_ROOK)] &= ~(1ULL << 56);
                _bitboards[pieceTypeToInt(PieceType::B_ROOK)] |= (1ULL << 58);
                _squaresLookup[59] = PieceType::EMPTY;
                _squaresLookup[57] = PieceType::B_KING;
                _squaresLookup[56] = PieceType::EMPTY;
                _squaresLookup[58] = PieceType::B_ROOK;
            } else {
                _bitboards[pieceTypeToInt(PieceType::B_KING)] &= ~(1ULL << 59);
                _bitboards[pieceTypeToInt(PieceType::B_KING)] |= (1ULL << 61);
                _bitboards[pieceTypeToInt(PieceType::B_ROOK)] &= ~(1ULL << 63);
                _bitboards[pieceTypeToInt(PieceType::B_ROOK)] |= (1ULL << 60);
                _squaresLookup[59] = PieceType::EMPTY;
                _squaresLookup[61] = PieceType::B_KING;
                _squaresLookup[63] = PieceType::EMPTY;
                _squaresLookup[60] = PieceType::B_ROOK;
            }
        }
    }

    void ChessBoard::unmakeCastleMove(bool wasWhite, bool wasKingSide) {
        if (wasWhite) {
            if (wasKingSide) {
                _bitboards[pieceTypeToInt(PieceType::W_KING)] &= ~(1ULL << 1);
                _bitboards[pieceTypeToInt(PieceType::W_KING)] |= (1ULL << 3);
                _bitboards[pieceTypeToInt(PieceType::W_ROOK)] &= ~(1ULL << 2);
                _bitboards[pieceTypeToInt(PieceType::W_ROOK)] |= (1ULL);
                _squaresLookup[1] = PieceType::EMPTY;
                _squaresLookup[3] = PieceType::W_KING;
                _squaresLookup[2] = PieceType::EMPTY;
                _squaresLookup[0] = PieceType::W_ROOK;
            } else {
                _bitboards[pieceTypeToInt(PieceType::W_KING)] &= ~(1ULL << 5);
                _bitboards[pieceTypeToInt(PieceType::W_KING)] |= (1ULL << 3);
                _bitboards[pieceTypeToInt(PieceType::W_ROOK)] &= ~(1ULL << 4);
                _bitboards[pieceTypeToInt(PieceType::W_ROOK)] |= (1ULL << 7);
                _squaresLookup[5] = PieceType::EMPTY;
                _squaresLookup[3] = PieceType::W_KING;
                _squaresLookup[4] = PieceType::EMPTY;
                _squaresLookup[7] = PieceType::W_ROOK;
            }
        } else {
            if (wasKingSide) {
                _bitboards[pieceTypeToInt(PieceType::B_KING)] &= ~(1ULL << 57);
                _bitboards[pieceTypeToInt(PieceType::B_KING)] |= (1ULL << 59);
                _bitboards[pieceTypeToInt(PieceType::B_ROOK)] &= ~(1ULL << 58);
                _bitboards[pieceTypeToInt(PieceType::B_ROOK)] |= (1ULL << 56);
                _squaresLookup[57] = PieceType::EMPTY;
                _squaresLookup[59] = PieceType::B_KING;
                _squaresLookup[58] = PieceType::EMPTY;
                _squaresLookup[56] = PieceType::B_ROOK;
            } else {
                _bitboards[pieceTypeToInt(PieceType::B_KING)] &= ~(1ULL << 61);
                _bitboards[pieceTypeToInt(PieceType::B_KING)] |= (1ULL << 59);
                _bitboards[pieceTypeToInt(PieceType::B_ROOK)] &= ~(1ULL << 60);
                _bitboards[pieceTypeToInt(PieceType::B_ROOK)] |= (1ULL << 63);
                _squaresLookup[61] = PieceType::EMPTY;
                _squaresLookup[59] = PieceType::B_KING;
                _squaresLookup[60] = PieceType::EMPTY;
                _squaresLookup[63] = PieceType::B_ROOK;
            }
        }
    }

    void ChessBoard::setCastlingFlags(PieceType pieceType, int from) {
        switch (pieceType) {
            case PieceType::W_KING:
                setKingMoved(true, true);
                break;
            case PieceType::W_ROOK:
                if (from == 0) {
                    setRookHMoved(true, true);
                } else if (from == 7) {
                    setRookAMoved(true, true);
                }
                break;
            case PieceType::B_KING:
                setKingMoved(false, true);
                break;
            case PieceType::B_ROOK:
                if (from == 56) {
                    setRookHMoved(false, true);
                } else if (from == 63) {
                    setRookAMoved(false, true);
                }
                break;
            default:
                break;
        }
    }

    void ChessBoard::unsetCastlingFlags(PieceType pieceType, int from) {
        switch (pieceType) {
            case PieceType::W_KING:
                setKingMoved(true, false);
                break;
            case PieceType::W_ROOK:
                if (from == 0) {
                    setRookHMoved(true, false);
                } else if (from == 7) {
                    setRookAMoved(true, false);
                }
                break;
            case PieceType::B_KING:
                setKingMoved(false, false);
                break;
            case PieceType::B_ROOK:
                if (from == 56) {
                    setRookHMoved(false, false);
                } else if (from == 63) {
                    setRookAMoved(false, false);
                }
                break;
            default:
                break;
        }
    }

    PieceType ChessBoard::getPromotionPieceType(int promotionFlag, bool isWhite) {
        switch(promotionFlag) {
            case Move::KNIGHT_PROMO_FLAG:
                return isWhite ? PieceType::W_KNIGHT : PieceType::B_KNIGHT;
                break;
            case Move::BISHOP_PROMO_FLAG:
                return isWhite ? PieceType::W_BISHOP : PieceType::B_BISHOP;
                break;
            case Move::ROOK_PROMO_FLAG:
                return isWhite ? PieceType::W_ROOK : PieceType::B_ROOK;
                break;
            case Move::QUEEN_PROMO_FLAG:
                return isWhite ? PieceType::W_QUEEN : PieceType::B_QUEEN;
                break;
            case Move::KNIGHT_PROMO_CAPTURE_FLAG:
                return isWhite ? PieceType::W_KNIGHT : PieceType::B_KNIGHT;
                break;
            case Move::BISHOP_PROMO_CAPTURE_FLAG:
                return isWhite ? PieceType::W_BISHOP : PieceType::B_BISHOP;
                break;
            case Move::ROOK_PROMO_CAPTURE_FLAG:
                return isWhite ? PieceType::W_ROOK : PieceType::B_ROOK;
                break;
            case Move::QUEEN_PROMO_CAPTURE_FLAG:
                return isWhite ? PieceType::W_QUEEN : PieceType::B_QUEEN;
                break;
        }

        return PieceType::EMPTY;
    }

    void ChessBoard::makeMove(Move move, bool isWhite) {
        // If the move is a castle, update the bitboards and return
        if (move.isAnyCastle()) {
            makeCastleMove(isWhite, move.isKingCastle());
            setHasCastled(isWhite, true);
            return;
        }

        // Get the from and to indices
        int from = move.getBitIndexFrom();
        int to = move.getBitIndexTo();
        
        // Piece type of piece being moved
        PieceType movedPieceType = _squaresLookup[from];
        bits::U64 movedPieceBitboard = _bitboards[pieceTypeToInt(movedPieceType)];
        movedPieceBitboard &= ~(1ULL << from);
        _squaresLookup[from] = PieceType::EMPTY;

        if (!move.isAnyCapture() && (movedPieceType != PieceType::W_PAWN && movedPieceType != PieceType::B_PAWN)) {
            _noCaptureOrPawnMoveCount++;
        } else {
            _noCaptureOrPawnMoveCount = 0;
        }

        // If the move is a capture, update the last captured piece and its bitboard
        if (move.isAnyCapture()) {
            int captureIndex = move.isEpCapture() ? (isWhite ? to - 8 : to + 8) : to;
            PieceType capturedPieceType = _squaresLookup[captureIndex];
            _lastCapturedPiece = capturedPieceType;

            _bitboards[pieceTypeToInt(capturedPieceType)] &= ~(1ULL << captureIndex);

            if (move.isEpCapture()) {
                _squaresLookup[captureIndex] = PieceType::EMPTY;
            }
        }

        if (move.isAnyPromo()) {
            PieceType promotionPieceType = getPromotionPieceType(move.getFlag(), isWhite);
            _bitboards[pieceTypeToInt(promotionPieceType)] |= (1ULL << to);
            _squaresLookup[to] = promotionPieceType;
        } else {
            movedPieceBitboard |= (1ULL << to);
            _squaresLookup[to] = movedPieceType;
        }

        // Update the moved piece bitboard
        _bitboards[pieceTypeToInt(movedPieceType)] = movedPieceBitboard;

        // If the player has not castled, update castling flags
        if (not getHasCastled(isWhite)) {
            setCastlingFlags(movedPieceType, from);
        }

        if (move.isDoublePawnPush()) {
            _enPessantTarget = isWhite ? (1ULL << (to - 8)) : (1ULL << (to + 8));
        } else {
            _enPessantTarget = 0ULL;
        }
    }

    void ChessBoard::makeTemporaryKingMove(bool isWhite, bool isKingSide) {
        int pieceTypeInt = isWhite ? pieceTypeToInt(PieceType::W_KING) : pieceTypeToInt(PieceType::B_KING);
        bits::U64 kingBitboard = _bitboards[pieceTypeInt];
        int from = isWhite ? 3 : 59;
        int to = isKingSide ? (isWhite ? 2 : 58) : (isWhite ? 4 : 60);

        kingBitboard &= ~(1ULL << from);
        kingBitboard |= (1ULL << to);

        _bitboards[pieceTypeInt] = kingBitboard;
    }

    void ChessBoard::unmakeTemporaryKingMove(bool isWhite, bool isKingSide) {
        int pieceTypeInt = isWhite ? pieceTypeToInt(PieceType::W_KING) : pieceTypeToInt(PieceType::B_KING);
        bits::U64 kingBitboard = _bitboards[pieceTypeInt];
        int from = isKingSide ? (isWhite ? 2 : 58) : (isWhite ? 4 : 60);
        int to = isWhite ? 3 : 59;

        kingBitboard &= ~(1ULL << from);
        kingBitboard |= (1ULL << to);

        _bitboards[pieceTypeInt] = kingBitboard;
    }

    void ChessBoard::unmakeMove(Move move, bool wasWhite) {
        // If the move is a castle, update the bitboards and return
        if (move.isAnyCastle()) {
            unmakeCastleMove(wasWhite, move.isKingCastle());
            setHasCastled(wasWhite, false);
            return;
        }

        // Get the from and to indices
        int from = move.getBitIndexFrom();
        int to = move.getBitIndexTo();
        
        // Piece type of piece being moved
        PieceType movedPieceType;

        // If the move was a promotion, set the moved piece to a pawn of the same color
        // Else, set the moved piece to the piece occupying the to square
        if (move.isAnyPromo()) {
            movedPieceType = wasWhite ? PieceType::W_PAWN : PieceType::B_PAWN;
        } else {
            movedPieceType = _squaresLookup[to];
        }

        // Place back the moved piece
        bits::U64 movedPieceBitboard = _bitboards[pieceTypeToInt(movedPieceType)];
        movedPieceBitboard |= (1ULL << from);
        _squaresLookup[from] = movedPieceType;

        // If the move was not a promotion, remove the piece in the bitboard
        // Else, remove the bit for the promoted piece
        if (not move.isAnyPromo()) {
            movedPieceBitboard &= ~(1ULL << to);
        } else {
            PieceType promotionPieceType = getPromotionPieceType(move.getFlag(), wasWhite);
            _bitboards[pieceTypeToInt(promotionPieceType)] &= ~(1ULL << to);
        }

        // Update the moved piece bitboard
        _bitboards[pieceTypeToInt(movedPieceType)] = movedPieceBitboard;

        // If the move was a capture, place back the captured piece
        // else set the square to empty
        if (move.isAnyCapture()) {
            int captureIndex = move.isEpCapture() ? (wasWhite ? to - 8 : to + 8) : to;
            _bitboards[pieceTypeToInt(_lastCapturedPiece)] |= (1ULL << captureIndex);
            _squaresLookup[captureIndex] = _lastCapturedPiece;

            if (move.isEpCapture()) {
                _squaresLookup[to] = PieceType::EMPTY;
            }
        } else {
            _squaresLookup[to] = PieceType::EMPTY;
        }

        if (not getHasCastled(wasWhite)) {
            unsetCastlingFlags(movedPieceType, from);
        }

        if (move.isDoublePawnPush()) {
            _enPessantTarget = 0ULL;
        }
    }

    void ChessBoard::fillSquaresLookup() {
        _squaresLookup = std::vector<PieceType>(64, PieceType::EMPTY);

        for (int i = 0; i < 64; i++) {
            for (int j = 0; j < 12; j++) {
                if (_bitboards[j] & (1ULL << i)) {
                    _squaresLookup[i] = intToPieceType(j);
                }
            }
        }
    }

    bool ChessBoard::kingSideCastlersHasMoved(bool isWhite) {
        if (isWhite) {
            return _whiteKingMoved || _whiteRookHMoved;
        } else {
            return _blackKingMoved || _blackRookHMoved;
        }
    }

    bool ChessBoard::queenSideCastlersHasMoved(bool isWhite) {
        if (isWhite) {
            return _whiteKingMoved || _whiteRookAMoved;
        } else {
            return _blackKingMoved || _blackRookAMoved;
        }
    }

    // TODO: MIGHT BE WRONG
    bits::U64 ChessBoard::getWhitePiecesBitmask() {
        bits::U64 w_all = 0;
        for (int i = 0; i < 6; i++) {
            w_all |= _bitboards[i];
        }

        return w_all;
    }

    // TODO: MIGHT BE WRONG
    bits::U64 ChessBoard::getBlackPiecesBitmask() {
        bits::U64 b_all = 0;
        for (int i = 6; i < 12; i++) {
            b_all |= _bitboards[i];
        }

        return b_all;
    }

    void ChessBoard::setBoardFromFen(const std::string& fen) {
        for (int i = 0; i < 12; i++) {
            _bitboards[i] = 0ULL;
        }

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
                PieceType type = CharToPieceType(c);
                uint64_t bit = 1ULL << (rank * 8 + (7 - file));
                _bitboards[pieceTypeToInt(type)] |= bit;
                file++;
            }
        }

        fillSquaresLookup();
    }
}