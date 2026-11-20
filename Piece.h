#ifndef PIECE_H
#define PIECE_H

#include "Types.h"
#include <vector>
#include <string>

class Board;

class Piece{
protected:
    std::string colour_;
    bool moved_ = false;

    std::vector<Position> slide(const Board& board, const std::vector<Offset>& directions, Position from) const;
    std::vector<Position> jump(const Board& board, Position from, const std::vector<Offset>& offsets) const;

public:
    explicit Piece(std::string colour) : colour_(colour) {};
    virtual ~Piece() = default;
    std::string getColour() const;
    bool hasMoved() const;
    void setMoved(bool moved) const;
    virtual PieceType getType() const = 0;
    virtual std::vector<Move> pseudoMoves(const Board& board, Position from) const = 0;
    virtual std::vector<Position> attackSquare(const Board& board, Position from) const = 0;
};

#endif
