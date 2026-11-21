#ifndef PAWN_H
#define PAWN_H

#include "Piece.h"

class Pawn: public Piece{
public:
    Pawn(Color colour);
    virtual PieceType getType() const override;
    virtual std::vector<Move> pseudoMoves(const Board& board, Position from) const override;
    virtual std::vector<Position> attackSquare(const Board& board, Position from) const override;
    virtual char getChar() const override;
};



#endif