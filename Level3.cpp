#include "Level3.h"
#include "Board.h"
#include "Piece.h"
#include "Rules.h"

std::vector<Move> Level3::choose(const Command& cmd, const Board& board, const std::vector<Move>& legal) {
    (void)cmd;
    if (legal.empty()) return {};

    Board& mutableBoard = const_cast<Board&>(board);

    int bestScore = -99999;
    Move bestMove = legal[0];
    Color color = getColor();

    for (const Move& m : legal) {
        int score = 0;

        // check if is capture
        if (isCapture(mutableBoard, m)) {
            score += 1;
        }

        mutableBoard.applyTrial(m);

        // check if is check
        if (rules_.isInCheck(mutableBoard, !color)) {
            score += 1;
        }

        // check if opponent can capture any of our pieces on their next turn
        std::vector<Move> opponentLegal = rules_.legalMoves(mutableBoard, !color);
        for (const Move& oppMove : opponentLegal) {
            if (isCapture(mutableBoard, oppMove)) {
                score -= 2;
                break;
            }
        }

        mutableBoard.undoTrial();

        if (score > bestScore) {
            bestScore = score;
            bestMove = m;
        }
    }

    return { bestMove };
}
