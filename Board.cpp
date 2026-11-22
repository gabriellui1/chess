// Status: Finished?? but not tested

#include "Board.h"
#include "Types.h"
struct Board::UndoRecord
{
    Move move;
    std::unique_ptr<Piece> captured;
    Position capturedAt;
    std::unique_ptr<Piece> replacedPawn;
    bool previouslyMoved = false;
};

Board::Board()
{
    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            squares_[row][col] = nullptr;
        }
    }
};

Board::~Board() = default;

bool Board::isOnBoard(Position pos) const
{
    return pos.row >= 0 && pos.col >= 0 && pos.row <= 7 && pos.col <= 7;
}

const std::unique_ptr<Piece> &Board::at(Position pos) const
{
    if (!isOnBoard(pos))
        return Board::outOfBoundsPtr_;

    return squares_[pos.row][pos.col];
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
    emptySquares();
    notifyObservers();
}

void Board::loadStandardPosition()
{
    emptySquares();

    for (int col = 0; col < 8; col++)
    {
        squares_[1][col] = PieceFactory::create(PieceType::Pawn, Color::White);
    }

    for (int col = 0; col < 8; col++)
    {
        squares_[6][col] = PieceFactory::create(PieceType::Pawn, Color::Black);
    }

    std::vector<PieceType> pieceTypes = {PieceType::Rook, PieceType::Knight, PieceType::Bishop, PieceType::Queen, PieceType::King, PieceType::Bishop, PieceType::Knight, PieceType::Rook};
    for (int row : {0, 7})
    {
        for (int col = 0; col < 8; col++)
        {
            squares_[row][col] = PieceFactory::create(pieceTypes[col], row == 0 ? Color::White : Color::Black);
        }
    }

    notifyObservers();
}

std::vector<Move> Board::lastMove() const
{
    if (history_.size() == 0)
        return std::vector<Move>();
    std::vector<Move> v{history_.back().move};
    return v;
}

void Board::apply(const Move &move)
{
    applyMove(move);
    notifyObservers();
}

void Board::undo()
{
    if (history_.size() == 0)
        return;
    undoMove();
    notifyObservers();
}

void Board::applyTrial(const Move &move)
{
    applyMove(move);
} // without notifying observer
void Board::undoTrial()
{
    undoMove();
}

std::vector<Position> Board::kingSquares(Color color) const
{
    std::vector<Position> kingPos = std::vector<Position>();
    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            Position pos{row, col};
            if (at(pos)->getType() == PieceType::King && at(pos)->getColor() == color)
            {
                kingPos.push_back(pos);
            }
        }
    }
    return kingPos;
}

void Board::notifyObservers()
{
    for (const std::unique_ptr<BoardObserver> &ob : observers_)
    {
        ob->onBoardChanged(*this);
    }
}

void Board::attach(std::unique_ptr<BoardObserver> observer)
{
    observers_.push_back(std::move(observer));
}

void Board::detach(std::unique_ptr<BoardObserver> observer)
{
    for (const std::unique_ptr<BoardObserver> &ob : observers_)
    {
        if (ob == observer)
        {
            observer.reset();
            observers_.pop_back();
            break;
        }
    }
}

void Board::applyMove(const Move &move)
{
    UndoRecord undo;
    undo.previouslyMoved = at(move.from)->hasMoved();

    if (move.kind == MoveKind::EnPassant)
    {
        undo.captured = std::move(squares_[move.from.row][move.to.col]);
        undo.capturedAt = Position{move.from.row, move.to.col};
    }
    else
    {
        undo.captured = std::move(squares_[move.to.row][move.to.col]);
        undo.capturedAt = Position{move.to.row, move.to.col};
    }

    if (move.kind == MoveKind::Castle)
    {
        if (move.to.col > move.from.col)
        {
            squares_[move.to.row][5] = std::move(squares_[move.to.row][7]);
            squares_[move.to.row][5]->setMoved(true);
        }
        else
        {
            squares_[move.to.row][3] = std::move(squares_[move.to.row][0]);
            squares_[move.to.row][3]->setMoved(true);
        }
    }

    if (move.kind == MoveKind::Promotion)
    {
        undo.replacedPawn = std::move(squares_[move.from.row][move.from.col]);
        squares_[move.to.row][move.to.col] = PieceFactory::create(move.promoteTo[0], undo.replacedPawn->getColor());
    }
    else
        squares_[move.to.row][move.to.col] = std::move(squares_[move.from.row][move.from.col]);

    squares_[move.to.row][move.to.col]->setMoved(true);
    history_.push_back(std::move(undo));
}

void Board::undoMove()
{
    if (history_.size() == 0)
        return;

    UndoRecord undo = std::move(history_.back());
    history_.pop_back();

    if (undo.move.kind == MoveKind::Promotion)
    {
        squares_[undo.move.to.row][undo.move.to.col].reset();
        squares_[undo.move.from.row][undo.move.from.col] = std::move(undo.replacedPawn);
    }
    else
        squares_[undo.move.from.row][undo.move.from.col] = std::move(squares_[undo.move.to.row][undo.move.to.col]);

    if (undo.move.kind == MoveKind::Castle)
    {
        if (undo.move.to.col > undo.move.from.col)
        {
            squares_[undo.move.to.row][7] = std::move(squares_[undo.move.to.row][5]);
            squares_[undo.move.to.row][7]->setMoved(false);
        }
        else
        {
            squares_[undo.move.to.row][0] = std::move(squares_[undo.move.to.row][3]);
            squares_[undo.move.to.row][0]->setMoved(false);
        }
    }

    squares_[undo.capturedAt.row][undo.capturedAt.col] = std::move(undo.captured);
    squares_[undo.move.from.row][undo.move.from.col]->setMoved(undo.previouslyMoved);
}

void Board::emptySquares()
{
    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            Position pos{row, col};
            remove(pos);
            squares_[row][col] = nullptr;
        }
    }
    history_.clear();
}
