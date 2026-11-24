#include "Level3.h"
#include "Board.h"
#include "Piece.h"
#include "Rules.h"

std::vector<Move> Level3::choose(const Command& cmd, const Board& board, const std::vector<Move>& legal) {
    (void)cmd;
    if (legal.empty()) return {};

    Rules rules;
    Board& mutableBoard = const_cast<Board&>(board);

    int bestScore = -99999;
    Move bestMove = legal[0];
    Color color = getColor();

    for (const Move& m : legal) {
        int score = 0;

        bool isCapture = false;
        bool isCheck = false;

        // check if is capture
        bool isCapture = isCapture(mutableBoard, m);

        mutableBoard.apply(m);

        // check if is check
        if (rules.inCheck(mutableBoard, !color)) {
            score += 100;
        }

        // check if opponent can capture any of our pieces on their next turn
        std::vector<Move> opponentLegal = rules.legalMoves(mutableBoard, !color);
        for (const Move& oppMove : opponentLegal) {
            bool capture = isCapture(mutableBoard, m);
            if (capture) {
                score -= 1;
                break;
            }
        }

        mutableBoard.undo();

        if (!canBeCaptured) {
            score += 1;
        }
        if (isCapture) {
            score += 1;
        }
        if (isCheck) {
            score += 1;
        }

        if (score > bestScore) {
            bestScore = score;
            bestMove = m;
        }
    }

    return { bestMove };
}
