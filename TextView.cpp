#include "TextView.h"

TextView::TextView(std::ostream& out) : out_(out) {}

void TextView::onBoardChanged(const Board& board) {
    (void)board;
    out_ << "TextView Board changed\n";
}
