#ifndef PLAYER_H
#define PLAYER_H

#include "Types.h"
#include <vector>

class Board;

class Player
{
protected:
    Color color = Color::White;

public:
    explicit Player(Color color) : color_(color) {}
    virtual ~Player() = default;

    Color getColor() const { return color_; }
    virtual bool isHuman() const = 0;
    Color getColor() const;
    virtual std::vector<Move> choose(const Command &, const Board &, const std::vector<Move> &legal) = 0;
};

class Human : public Player
{

public:
    std::vector<Move> choose(const Command &, const Board &, const std::vector<Move> &legal) override;
    bool isHuman() const override;
};
#endif
