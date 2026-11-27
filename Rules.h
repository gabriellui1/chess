#ifndef RULES_H
#define RULES_H
#include "Types.h"

class Board;

class Rules
{
    bool castlinghelper(Position rook_square_king, const Board &board, Color opponent, std::vector<Position> square_to_check, std::vector<Position> king_crossed_squares) const;
public:
    std::vector<Move> legalMoves(Board &board, Color side) const;
    bool isInCheck(const Board &board, Color side) const;
    bool isAttacked(const Board &board, Position square, Color byColor) const;
    GameStatus status(const Board &board, Color toMove, const std::vector<Move> &legal) const;
    bool givesCheck(Board &board, const Move &move, Color mover) const;
    std::vector<std::string> validateSetup(const Board &board) const;
    std::vector<Move> castlingMoves(const Board &board, Color color) const;
    bool isThreefoldRepetition(const Board &board) const;
    bool isFiftyMoves(const Board &board) const;
};

#endif
