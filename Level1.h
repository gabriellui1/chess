#ifndef LEVEL1_H
#define LEVEL1_H

#include "Computer.h"

class Level1 : public Computer {
public:
    using Computer::Computer;
    std::vector<Move> choose(const Command& cmd, const Board& board, const std::vector<Move>& legal) override;
};

#endif
