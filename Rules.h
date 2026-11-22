#ifndef RULES_H
#define RULES_H
#include "Types.h"

class Board;

class Rules
{
public:
    std::vector<Move> legalMoves(Board &board, Color side) const;
    bool isInCheck(const Board &board, Color side) const;
    bool isAttacked(const Board &board, Position square, Color byColor) const;
    GameStatus status(const Board &board, Color toMove, const std::vector<Move> &legal) const;
    bool givesCheck(Board &board, const Move &move, Color mover) const;
    std::vector<std::string> validateSetup(const Board &board) const;
    std::vector<Move> castlingMoves(const Board &board, Color color) const;
};

#endif