#pragma once
#include "ChessEngine/src/common.h"

#include "ChessEngine/src/model/move/Move.h"

namespace model {

class Movelist {

public:
    Movelist();

    const Move& getMoveAt(int index) const;
    int getMoveIndex() const;
    
    void addMove(const Move& move);
    void addNullMove();
    void reset();
    
private:
    std::vector<Move> _movelist;
    int _moveIndex;
};

} // namespace model

#include "ChessEngine/src/model/move/Movelist.inl"