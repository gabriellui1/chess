#include "Level2.h"
#include "Board.h"
#include "Rules.h"

std::vector<Move> Level2::choose(const Command& cmd, const Board& board, const std::vector<Move>& legal) {
    (void)cmd;
    if (legal.empty()) return {};

    Color opponent = !getColor();
    Board& mutableBoard = const_cast<Board&>(board);

    int bestScore = -1;
    Move bestMove = legal[0];

    for (const Move& m : legal) {
        int score = 0;
        bool capture = isCapture(board, m);

        mutableBoard.apply(m);
        bool givesCheck = rules_.isInCheck(mutableBoard, opponent);
        mutableBoard.undo();

        if (capture) {
            score += 1;
        }
        if (givesCheck) {
            score += 1;
        }

        if (score > bestScore) {
            bestScore = score;
            bestMove = m;
        }
    }

    return { bestMove };
}
