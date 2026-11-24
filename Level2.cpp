#include "Level2.h"
#include "Board.h"
#include "Rules.h"
#include <random>

std::vector<Move> Level2::choose(const Command &cmd, const Board &board, const std::vector<Move> &legal)
{
    (void)cmd;
    if (legal.empty())
        return {};

    Color opponent = (getColor() == Color::White) ? Color::Black : Color::White;
    Rules rules;
    Board &mutableBoard = const_cast<Board &>(board);

    std::vector<Move> preferred;

    for (const Move &m : legal)
    {
        bool isCapture = (board.at(m.to) != nullptr) || (m.kind == MoveKind::EnPassant);

        mutableBoard.apply(m);
        bool givesCheck = rules.isInCheck(mutableBoard, opponent);
        mutableBoard.undo();

        if (isCapture || givesCheck)
        {
            preferred.push_back(m);
        }
    }

    const std::vector<Move> &pool = preferred.empty() ? legal : preferred;

    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<std::size_t> dist(0, pool.size() - 1);

    return {pool[dist(rng)]};
}
