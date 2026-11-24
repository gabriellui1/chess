#include "Level1.h"
#include <random>

std::vector<Move> Level1::choose(const Command& cmd, const Board& board, const std::vector<Move>& legal) {
    (void)cmd; // silence complier
    (void)board;
    if (legal.empty()) return {};

    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<std::size_t> dist(0, legal.size() - 1);

    return { legal[dist(rng)] };
}
