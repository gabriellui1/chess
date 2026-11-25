#ifndef LEVEL4_H
#define LEVEL4_H

#include "Computer.h"

class Level4 : public Computer {
public:
    using Computer::Computer;
    std::vector<Move> choose(const Command& cmd, const Board& board, const std::vector<Move>& legal) override;
    int evaluateBoard(const Board& board) const;
    int alphaBeta(Board& board, int depth, int alpha, int beta, Color currentColor) const;
};

#endif
