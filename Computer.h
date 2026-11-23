#ifndef COMPUTER_H
#define COMPUTER_H

#include "Player.h"
#include "Rules.h"
#include <vector>

class Computer : public Player
{ // different levels implement this class

protected:
    const Rules &rules_;

public:
    explicit Computer(const Rules &rules);
    bool isHuman() const;
    virtual std::vector<Move> choose(const Command &, const Board &, const std::vector<Move> &legal) = 0;
    // more as needed
};

#endif
