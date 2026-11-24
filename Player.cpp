#include "Player.h"
#include "Helper.h"
bool Human::isHuman() const
{
    return true;
}

std::vector<Move> Human::choose(const Command &cmd, const Board &board, const std::vector<Move> &legal)
{
    for (Move move : legal)
    {
        if (move.from == cmd.from && move.to == cmd.to && (move.promoteTo.empty() || move.promoteTo[0] == cmd.piece))
            return std::vector<Move>({move});
    }
    return std::vector<Move>();
}

Color Player::getColor() const
{
    return color_;
}
