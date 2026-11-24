#include "Computer.h"
#include "Board.h"

bool Computer::isCapture(const Board &board, const Move &m) const
{
    if (board.at(m.to) != nullptr || m.kind == MoveKind::EnPassant)
    {
        return true;
    }
    return false;
}

bool Computer::isHuman() const
{
    return false;
}
