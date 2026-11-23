#include "Level1.h"

std::vector<Move> Level1::choose(const Command& cmd, const Board& board, const std::vector<Move>& legal) {
    (void)cmd; // silence complier
    (void)board;
    if (legal.empty()) return {};
    return { legal[0] };
}
