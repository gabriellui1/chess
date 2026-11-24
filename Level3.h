#ifndef LEVEL3_H
#define LEVEL3_H

#include "Computer.h"

class Level3 : public Computer {
public:
    std::vector<Move> choose(const Command& cmd, const Board& board, const std::vector<Move>& legal) override;
};

#endif
