#ifndef PIECES_H
#define PIECES_H

#include "Piece.h"

class King: public Piece{
public:
    King(Color colour);
    virtual PieceType getType() const override;
    virtual std::vector<Move> pseudoMoves(const Board& board, Position from) const override;
    virtual char getChar() const override;
};

class Queen: public Piece{
public:
    Queen(Color colour);
    virtual PieceType getType() const override;
    virtual std::vector<Move> pseudoMoves(const Board& board, Position from) const override;
    virtual char getChar() const override;
};

class Rook: public Piece{
public:
    Rook(Color colour);
    virtual PieceType getType() const override;
    virtual std::vector<Move> pseudoMoves(const Board& board, Position from) const override;
    virtual char getChar() const override;
};

class Bishop: public Piece{
public:
    Bishop(Color colour);
    virtual PieceType getType() const override;
    virtual std::vector<Move> pseudoMoves(const Board& board, Position from) const override;
    virtual char getChar() const override;
};

class Knight: public Piece{
public:
    Knight(Color colour);
    virtual PieceType getType() const override;
    virtual std::vector<Move> pseudoMoves(const Board& board, Position from) const override;
    virtual char getChar() const override;
};



#endif