#include "Level4.h"
#include "Board.h"
#include "Piece.h"
#include "Rules.h"
#include <algorithm>
#include <vector>

int Level4::evaluateBoard(const Board& board) const {
    int eval = 0;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            Position pos = Position{i, j};
            const std::unique_ptr<Piece>& piece = board.at(pos);

            if (piece != nullptr && piece != Board::outOfBoundsPtr_) {
                int c = 1;
                if (piece->getColor() == Color::Black) {
                    c = -1;
                }

                if ((i == 3 || i == 4) && (j == 3 || j == 4)) {
                    eval += 3 * c;
                }

                std::vector<Position> attackSquares = piece->attackSquare(board, pos);
                for (const Position& p : attackSquares) {
                    if ((p.row == 3 || p.row == 4) && (p.col == 3 || p.col == 4)) {
                        eval += 2 * c;
                    }
                    const std::unique_ptr<Piece>& attackPiece = board.at(p);
                    if (attackPiece != nullptr && attackPiece != Board::outOfBoundsPtr_) {
                        if (attackPiece->getColor() != piece->getColor()) {
                            eval += 5 * c;
                        }
                    }
                    eval += 1 * c;
                }
                eval += PIECE_VALUES.at(piece->getType()) * 20 * c;
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
            return (currentColor == Color::White) ? (-90000 - depth) : (90000 + depth);
        }
        return 0;
    }
    if (rules_.isThreefoldRepetition(board)) {
        return 0;
    }
    if (rules_.isFiftyMoves(board)) {
        return 0;
    }

    if (currentColor == Color::White) {
        int bestScore = -99999;
        for (const Move& m : legals) {
            board.applyTrial(m);
            int score = alphaBeta(board, depth - 1, alpha, beta, !currentColor);
            board.undoTrial();

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
            board.applyTrial(m);
            int score = alphaBeta(board, depth - 1, alpha, beta, !currentColor);
            board.undoTrial();

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
            mutableBoard.applyTrial(m);
            int score = alphaBeta(mutableBoard, depth - 1, alpha, beta, !color);
            mutableBoard.undoTrial();

            if (score > bestScore) {
                bestScore = score;
                alpha = std::max(alpha, score);
                bestMove = m;
            }
        }
    } else {
        int bestScore = 99999;
        for (const Move& m : legal) {
            mutableBoard.applyTrial(m);
            int score = alphaBeta(mutableBoard, depth - 1, alpha, beta, !color);
            mutableBoard.undoTrial();

            if (score < bestScore) {
                bestScore = score;
                beta = std::min(beta, score);
                bestMove = m;
            }
        }
    }

    return { bestMove };
}
