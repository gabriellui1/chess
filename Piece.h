#ifndef PIECE_H
#define PIECE_H

#include "Types.h"

class Board;

class Piece{
    Color colour_;
    bool moved_ = false;
protected:
    std::vector<Position> slide(const Board& board, const std::vector<Offset>& directions, Position from, int maxSteps) const;
    std::vector<Position> jump(const Board& board, Position from, const std::vector<Offset>& offsets) const;
    std::vector<Move> toMoves(const Board& board, Position from, const std::vector<Position>& squares) const;

public:
    explicit Piece(Color colour) : colour_(colour) {};
    virtual ~Piece() = default;
    Color getColour() const;
    bool hasMoved() const;
    void setMoved(bool moved);
    virtual PieceType getType() const = 0;
    virtual std::vector<Move> pseudoMoves(const Board& board, Position from) const = 0;
    virtual std::vector<Position> attackSquare(const Board& board, Position from) const;
    virtual char getChar() const = 0;
};

#endif
