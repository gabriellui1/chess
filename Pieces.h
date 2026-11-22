#ifndef PIECES_H
#define PIECES_H

#include "Piece.h"

class King : public Piece
{
public:
    King(Color color);
    virtual PieceType getType() const override;
    virtual std::vector<Move> pseudoMoves(const Board &board, Position from) const override;
    virtual char getChar() const override;
};

class Queen : public Piece
{
public:
    Queen(Color color);
    virtual PieceType getType() const override;
    virtual std::vector<Move> pseudoMoves(const Board &board, Position from) const override;
    virtual char getChar() const override;
};

class Rook : public Piece
{
public:
    Rook(Color color);
    virtual PieceType getType() const override;
    virtual std::vector<Move> pseudoMoves(const Board &board, Position from) const override;
    virtual char getChar() const override;
};

class Bishop : public Piece
{
public:
    Bishop(Color color);
    virtual PieceType getType() const override;
    virtual std::vector<Move> pseudoMoves(const Board &board, Position from) const override;
    virtual char getChar() const override;
};

class Knight : public Piece
{
public:
    Knight(Color color);
    virtual PieceType getType() const override;
    virtual std::vector<Move> pseudoMoves(const Board &board, Position from) const override;
    virtual char getChar() const override;
};

#endif