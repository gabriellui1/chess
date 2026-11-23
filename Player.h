#ifndef PLAYER_H
#define PLAYER_H

#include "Types.h"
#include <vector>
class Board;

class Player
{
protected:
public:
    virtual ~Player() = default;
    virtual bool isHuman() const = 0;
    virtual std::vector<Move> choose(const Command &, const Board &, const std::vector<Move> &legal) = 0;
};

class Human : public Player
{

public:
    virtual std::vector<Move> choose(const Command &, const Board &, const std::vector<Move> &legal) override;
};
#endif
