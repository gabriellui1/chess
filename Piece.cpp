#include "Piece.h"
#include "Board.h"

std::vector<Position> Piece::slide(const Board &board, const std::vector<Offset> &directions, Position from, int maxSteps) const
{
    std::vector<Position> squares;
    for (const Offset &dir : directions)
    { // read only
        Position cur = from;

        for (int i = 0; i < maxSteps; ++i)
        {
            cur = Position{cur.row + dir.drow, cur.col + dir.dcol};
            if (!board.isOnBoard(cur))
            {
                break; // can't step in that direction anymore
            }

            squares.push_back(cur); // include the square that possibly can have a piece there.
            if (board.at(cur) != Board::outOfBoundsPtr_)
            {
                break; // there is a piece so break
            }
        }
        return squares;
    }
}

std::vector<Position> Piece::jump(const Board &board, Position from, const std::vector<Offset> &offsets) const
{
    std::vector<Position> squares;
    for (const Offset &offset : offsets)
    { // read only
        Position cur = Position{from.row + offset.drow, from.col + offset.dcol};
        if (board.isOnBoard(cur))
        {
            squares.push_back(cur);
        }
    }
    return squares;
}

std::vector<Move> Piece::toMoves(const Board &board, Position from, const std::vector<Position> &squares) const
{
    std::vector<Move> moves;
    for (Position cur_pos : squares)
    {
        if(board.at(cur_pos) != Board::outOfBoundsPtr_){
            if (board.at(cur_pos)->getColor() != getColor())
            { // only allow move that squares is empty or does not hit the same color.
                moves.push_back(Move{from, cur_pos, MoveKind::Normal, {}});
            }
        }
    }
    return moves;
}

Color Piece::getColor() const
{
    return color_;
}
bool Piece::hasMoved() const
{
    return moved_;
}
void Piece::setMoved(bool moved)
{
    moved_ = moved;
}

std::vector<Position> Piece::attackSquare(const Board &board, Position from) const
{
    std::vector<Move> moves = pseudoMoves(board, from);
    std::vector<Position> pos_vec;
    for (Move &move : moves)
    {
        pos_vec.push_back(move.to);
    }
    return pos_vec;
}