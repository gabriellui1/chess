#include "Board.h"

struct Board::UndoRecord
{
    Move move;
    std::unique_ptr<Piece> captured;
    Position capturedAt;
    std::unique_ptr<Piece> replacedPawn;
    bool previouslyMoved = false;
};

Board::Board() = default;
Board::~Board() = default;

bool Board::isOnBoard(Position pos) const
{
    return pos.row >= 0 && pos.col >= 0 && pos.row <= 7 && pos.col <= 7;
}

const Piece *Board::at(Position pos) const
{
    if (!isOnBoard(pos))
        return nullptr;

    return squares_[pos.row][pos.col].get();
}

void Board::place(Position pos, std::unique_ptr<Piece> piece)
{
    remove(pos);
    squares_[pos.row][pos.col] = std::move(piece);
    notifyObservers();
}

void Board::remove(Position pos)
{
    if (!isOnBoard(pos))
        return;

    if (at(pos) != nullptr)
    {
        squares_[pos.row][pos.col].reset();
        notifyObservers();
    }
}

void Board::clear()
{
    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            Position pos{row, col};
            remove(pos);
        }
    }
    history_.clear();
    notifyObservers();
}

void Board::attach(BoardObserver *observer)
{
    observers_.push_back(observer);
}

void Board::notifyObservers()
{
    for (BoardObserver *observer : observers_)
    {
        observer->onBoardChanged(*this);
    }
}

void Board::loadStandardPosition()
{
    // TODO: actually load the position
    notifyObservers();
}

std::vector<Move> Board::lastMove() const
{
    if (history_.size() == 0)
        return std::vector<Move>();
    std::vector<Move> v{history_.back().move};
    return v;
}

std::vector<Position> Board::kingSquares(Color color) const
{
    std::vector<Position> kingPos = std::vector<Position>();
    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            Position pos{row, col};
            if (at(pos)->getType() == PieceType::King && at(pos)->getColour() == color)
            {
                kingPos.push_back(pos);
            }
        }
    }
    return kingPos;
}

void Board::apply(const Move &move)
{
}