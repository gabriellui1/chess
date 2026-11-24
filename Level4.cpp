#include "Level4.h"
#include "Board.h"
#include "Piece.h"
#include "Rules.h"
#include <algorithm>

int Level4::evaluateBoard(const Board& board) const {
    int eval = 0;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            const std::unique_ptr<Piece> &piece = board.at(Position{i, j});
            if (piece != Board::outOfBoundsPtr_)
            {
                Color color = piece->getColor();
                if (color == Color::White) {
                    eval += PIECE_VALUES.at(piece->getType());
                } else {
                    eval -= PIECE_VALUES.at(piece->getType());
                }
            }
        }
    }
    return eval;
}

int Level4::alphaBeta(Board& board, int depth, int alpha, int beta, Color currentColor) const {
    if (depth == 0) {
        return evaluateBoard(board);
    }

    std::vector<Move> legals = rules_.legalMoves(board, currentColor);
    if (legals.empty()) {
        if (rules_.isInCheck(board, currentColor)) {
            if (currentColor == Color::White) {
                return -9999;
            } else {
                return 9999;
            }
        }
        return 0;
    }

    if (currentColor == Color::White) {
        int bestScore = -99999;
        for (const Move& m : legals) {
            board.apply(m);
            int score = alphaBeta(board, depth - 1, alpha, beta, !currentColor);
            board.undo();

            bestScore = std::max(bestScore, score);
            alpha = std::max(alpha, bestScore);
            if (beta <= alpha) {
                break;
            }
        }
        return bestScore;
    } else {
        int bestScore = 99999;
        for (const Move& m : legals) {
            board.apply(m);
            int score = alphaBeta(board, depth - 1, alpha, beta, !currentColor);
            board.undo();

            bestScore = std::min(bestScore, score);
            beta = std::min(beta, bestScore);
            if (beta <= alpha) {
                break;
            }
        }
        return bestScore;
    }
}

std::vector<Move> Level4::choose(const Command& cmd, const Board& board, const std::vector<Move>& legal) {
    (void)cmd;
    if (legal.empty()) return {};

    Board& mutableBoard = const_cast<Board&>(board);
    Color color = getColor();

    int depth = 3;
    int alpha = -99999;
    int beta = 99999;
    Move bestMove = legal[0];

    if (color == Color::White) {
        int bestScore = -99999;
        for (const Move& m : legal) {
            mutableBoard.apply(m);
            int score = alphaBeta(mutableBoard, depth - 1, alpha, beta, !color);
            mutableBoard.undo();

            if (score > bestScore) {
                bestScore = score;
                alpha = std::max(alpha, score);
                bestMove = m;
            }
        }
    } else {
        int bestScore = 99999;
        for (const Move& m : legal) {
            mutableBoard.apply(m);
            int score = alphaBeta(mutableBoard, depth - 1, alpha, beta, !color);
            mutableBoard.undo();

            if (score < bestScore) {
                bestScore = score;
                beta = std::min(beta, score);
                bestMove = m;
            }
        }
    }

    return { bestMove };
}
