#include "Pawn.h"
#include "Board.h"

Pawn::Pawn(Color color) : Piece{color} {}

PieceType Pawn::getType() const
{
    return PieceType::Pawn;
}

// helper
std::vector<Move> add(Position from, Position cur, std::vector<Move> moves, int lastRow)
{
    if (cur.row == lastRow)
    {
        for (PieceType p : {PieceType::Queen, PieceType::Rook, PieceType::Bishop, PieceType::Knight})
        {
            moves.push_back(Move{from, cur, MoveKind::Promotion, {p}});
        }
    }
    else
    {
        moves.push_back(Move{from, cur, MoveKind::Normal, {}});
    }
    return moves;
}

std::vector<Move> Pawn::pseudoMoves(const Board &board, Position from) const
{
    int direction = getColor() == Color::White ? 1 : -1;
    int homeRow = getColor() == Color::White ? 1 : 6;
    int lastRow = getColor() == Color::White ? 7 : 0;
    std::vector<Move> moves;

    // one and two step forward
    Position cur = Position{from.row + direction, from.col};
    if (isOnBoard(cur) && board.at(cur) == nullptr)
    {
        moves = add(from, cur, moves, lastRow);
        Position cur_two = Position{from.row + 2 * direction, from.col};
        if (from.row == homeRow && isOnBoard(cur_two) && board.at(cur_two) == nullptr)
        {
            moves.push_back(Move{from, cur_two, MoveKind::DoublePawnStep, {}});
        }
    }

    // Captures

    Position capture_right{from.row + direction, from.col + 1}; // test right capture
    if (isOnBoard(capture_right))
    {
        const Piece *target = board.at(capture_right);
        if (target != nullptr && target->getColor() != getColor())
        {
            moves = add(from, capture_right, moves, lastRow);
        }
    }
    Position capture_left{from.row + direction, from.col - 1}; // test left capture
    if (isOnBoard(capture_left))
    {
        const Piece *target = board.at(capture_left);
        if (target != nullptr && target->getColor() != getColor())
        {
            moves = add(from, capture_left, moves, lastRow);
        }
    }

    // En passant, the last move must be an enemy pawn move with DoublePawnStep and next to our pawn
    std::vector<Move> last = board.lastMove();
    if (!last.empty())
    {
        const Move &lastmove = last.back();
        const Piece *other = board.at(lastmove.to);
        if (lastmove.kind == MoveKind::DoublePawnStep && other != nullptr && other->getType() == PieceType::Pawn && other->getColor() != getColor() && lastmove.to.row == from.row && (lastmove.to.col - from.col == 1 || lastmove.to.col - from.col == -1))
        {
            moves.push_back(Move{from, Position{from.row + direction, lastmove.to.col}, MoveKind::EnPassant, {}});
        }
    }

    return moves;
}

std::vector<Position> Pawn::attackSquare(const Board &board, Position from) const
{
    // Captures
    int direction = getColor() == Color::White ? 1 : -1;
    std::vector<Move> moves;

    Position capture_right{from.row + direction, from.col + 1}; // test right capture
    if (isOnBoard(capture_right))
    {
        moves.push_back(Move{from, capture_right, MoveKind::Normal, {}});
    }

    Position capture_left{from.row + direction, from.col - 1}; // test left capture
    if (isOnBoard(capture_left))
    {
        moves.push_back(Move{from, capture_left, MoveKind::Normal, {}});
    }
}

char Pawn::getChar() const
{
    return getColor() == Color::White ? 'P' : 'p';
}