#ifndef PLAYER_H
#define PLAYER_H

#include "Types.h"
#include <vector>

class Board;

class Player
{
protected:
    Color color_ = Color::White;

public:
    explicit Player(Color color) : color_(color) {}
    virtual ~Player() = default;

    virtual bool isHuman() const = 0;
    Color getColor() const;
    virtual std::vector<Move> choose(const Command &, const Board &, const std::vector<Move> &legal) = 0;
};

class Human : public Player
{

public:
    using Player::Player;
    std::vector<Move> choose(const Command &, const Board &, const std::vector<Move> &legal) override;
    bool isHuman() const override;
};
#endif
