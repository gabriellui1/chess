#include "Level2.h"
#include "Board.h"
#include "Rules.h"

std::vector<Move> Level2::choose(const Command& cmd, const Board& board, const std::vector<Move>& legal) {
    (void)cmd;
    if (legal.empty()) return {};

    Color opponent = (getColor() == Color::White) ? Color::Black : Color::White;
    Rules rules;
    Board& mutableBoard = const_cast<Board&>(board);

    int bestScore = -1;
    Move bestMove = legal[0];

    for (const Move& m : legal) {
        int score = 0;
        bool isCapture = isCapture(mutableBoard, m);

        mutableBoard.apply(m);
        bool givesCheck = rules.inCheck(mutableBoard, opponent);
        mutableBoard.undo();

        if (isCapture) {
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
