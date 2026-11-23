#include "Player.h"
bool Human::isHuman() const
{
    return true;
}

std::vector<Move> Human::choose(const Command &cmd, const Board &board, const std::vector<Move> &legal)
{
    return legal;
}