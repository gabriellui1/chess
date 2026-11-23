#include "Level2.h"

std::vector<Move> Level2::choose(const Command& cmd, const Board& board, const std::vector<Move>& legal) {
    (void)cmd; // silence complier
    (void)board;
    if (legal.empty()) return {};
    return { legal[0] };
}
