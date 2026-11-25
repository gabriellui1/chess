#ifndef COMPUTER_H
#define COMPUTER_H

#include "Player.h"
#include "Rules.h"
#include <vector>

class Computer : public Player
{ // different levels implement this class

protected:
    const Rules &rules_;
    bool isCapture(const Board &board, const Move &m) const;

public:
    Computer(Color color, const Rules &rules) : Player(color), rules_(rules) {}
    virtual ~Computer() = default;

    bool isHuman() const;
    virtual std::vector<Move> choose(const Command &, const Board &, const std::vector<Move> &legal) = 0;
};

#endif
