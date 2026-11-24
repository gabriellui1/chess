#ifndef LEVEL2_H
#define LEVEL2_H

#include "Computer.h"

class Level2 : public Computer {
public:
    std::vector<Move> choose(const Command& cmd, const Board& board, const std::vector<Move>& legal) override;
};

#endif
