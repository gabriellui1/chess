#include "Board.h"
#include "Types.h"
#include "Helper.h"

struct Board::UndoRecord
{
    Move move;
    PieceType movedPiece;
    Color movedColor;
    std::unique_ptr<Piece> captured;
    Position capturedAt;
    std::unique_ptr<Piece> replacedPawn;
    bool previouslyMoved = false;
    std::string previousKey;
    std::string san;
};

Board::Board()
    : squares_(8)
{
    for (auto &row : squares_)
    {
        row.resize(8);
    }
}

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

void Board::place(Position pos, std::unique_ptr<Piece> &piece)
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
    if (history_.empty())
        return std::vector<Move>();
    std::vector<Move> v{history_.back().move};
    return v;
}

static std::string ambiguous(const Board &board, const Move &move, PieceType piece, Color color)
{
    bool a = false;
    bool matchCol = false;
    bool matchRow = false;
    for (int r = 0; r < 8; ++r)
    {
        for (int c = 0; c < 8; ++c)
        {
            if (r == move.from.row && c == move.from.col) { continue; }
            const auto &other = board.at(Position{r, c});
            if (other != nullptr && other->getColor() == color && other->getType() == piece)
            {
                for (const Move &m : other->pseudoMoves(board, Position{r, c}))
                {
                    if (m.to.row == move.to.row && m.to.col == move.to.col)
                    {
                        a = true;
                        if (c == move.from.col)
                        {
                            matchCol = true;
                        }
                        if (r == move.from.row)
                        {
                            matchRow = true;
                        }
                        break;
                    }
                }
            }
        }
    }
    if (!a) { return ""; }
    if (!matchCol) { return std::string(1, static_cast<char>('a' + move.from.col)); }
    if (!matchRow) { return std::string(1, static_cast<char>('1' + move.from.row)); }
    return squareToString(move.from);
}

std::string Board::generateMoveSan(const Move &move, PieceType piece, Color color, bool isCapture) const {
    if (move.kind == MoveKind::Castle) {
        return (move.to.col == 6) ? "O-O" : "O-O-O";
    }
    std::string san;
    if (piece == PieceType::Pawn) {
        if (isCapture) {
            san += static_cast<char>('a' + move.from.col);
            san += 'x';
        }
        san += squareToString(move.to);
        if (move.kind == MoveKind::Promotion && !move.promoteTo.empty()) {
            san += '=';
            san += toUpper(PIECE_CHAR.at(move.promoteTo[0]));
        }
        return san;
    }
    else {
        san += toUpper(PIECE_CHAR.at(piece));
        san += ambiguous(*this, move, piece, color);
        if (isCapture) {
            san += 'x';
        }
        san += squareToString(move.to);
        return san;
    }
}

void Board::apply(const Move &move)
{
    PieceType piece = at(move.from)->getType();
    Color color = at(move.from)->getColor();
    bool isCapture = (move.kind == MoveKind::EnPassant) || (squares_[move.to.row][move.to.col] != nullptr);

    std::string san = generateMoveSan(move, piece, color, isCapture);
    applyMove(move);
    history_.back().san = std::move(san); // don't need san anymore

    notifyObservers();
}

void Board::undo()
{
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
            if (at(pos) != nullptr && at(pos) != Board::outOfBoundsPtr_ && at(pos)->getType() == PieceType::King && at(pos)->getColor() == color)
            {
                kingPos.push_back(pos);
            }
        }
    }
    return kingPos;
}

// Fix and understand this code notifyObservers, attach, detach
void Board::notifyObservers()
{
    std::vector<std::shared_ptr<BoardObserver>> liveObservers;
    std::erase_if(observers_, [&liveObservers](const auto &observer)
                  {
        if (auto live = observer.lock())
        {
            liveObservers.push_back(std::move(live));
            return false;
        }
        return true; });

    for (const auto &observer : liveObservers){
        observer->onBoardChanged(*this);
    }
}

void Board::attach(const std::shared_ptr<BoardObserver> &observer)
{
    observers_.push_back(observer);
}

void Board::detach(const std::shared_ptr<BoardObserver> &observer)
{
    std::erase_if(observers_, [&observer](const auto &current) { // hello??????
        auto live = current.lock();
        return !live || live == observer;
    });
}

void Board::applyMove(const Move &move)
{
    UndoRecord undo;
    undo.previousKey = getKey();
    undo.move = move;
    undo.previouslyMoved = at(move.from)->hasMoved();
    undo.movedPiece = at(move.from)->getType();
    undo.movedColor = at(move.from)->getColor();

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
    if (history_.empty())
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

bool Board::canUndo() const {return !history_.empty();}

bool Board::castlingRights(Color color, bool kingside) const
{
    int x = (color == Color::White) ? 0 : 7;
    int rookPos = kingside ? 7 : 0;
    const auto & king = squares_[x][4];
    const auto & rook = squares_[x][rookPos];
    bool kingGood = false;
    if (king != nullptr && king->getType() == PieceType::King
        && king->getColor() == color && !king->hasMoved()) {
        kingGood = true;
    }
    bool rookGood = false;
    if (rook != nullptr && rook->getType() == PieceType::Rook &&
           rook->getColor() == color && !rook->hasMoved()) {
        rookGood = true;
    }
    return kingGood && rookGood;
}

int Board::fiftyMoveCount() const
{
    int count = 0;
    for (int i = static_cast<int>(history_.size()) - 1; i >= 0; --i)
    {
        const UndoRecord &rec = history_[i];
        if (rec.captured != nullptr || rec.movedPiece == PieceType::Pawn)
        {
            break;
        }
        ++count;
    }
    return count;
}

std::string Board::getPGN() const
{
    std::string pgn;
    int count = 1;
    for (int i = 0; i < static_cast<int>(history_.size()); ++i) {
        const UndoRecord &r = history_[i];
        if (i > 0) {
            pgn += ' ';
        }
        if (r.movedColor == Color::White) {
            pgn += std::to_string(count) + ". ";
        }
        // assuming no custom positions
        pgn += r.san;
        if (r.movedColor == Color::Black) {
            ++count;
        }
    }
    return pgn;
}

std::string Board::getKey() const {
    std::string fen;
    for (int i = 7; i >= 0; --i)
    {
        int empty = 0;
        for (int j = 0; j < 8; j++) {
            const auto &p = squares_[i][j];
            if (p == nullptr) {
                ++empty;
            } else {
                if (empty > 0) {
                    fen += std::to_string(empty);
                    empty = 0;
                }
                char c = PIECE_CHAR.at(p->getType());
                if (p->getColor() == Color::Black) {
                    if (c >= 'A' && c <= 'Z') c += ('a' - 'A');
                } else {
                    if (c >= 'a' && c <= 'z') c -= ('a' - 'A');
                }
                fen += c;
            }
        }
        if (empty > 0) {
            fen += std::to_string(empty);
            empty = 0;
        }
        if (i > 0)
        {
            fen += '/';
        }
    }
    fen += (history_.size() % 2 == 0) ? " w " : " b ";

    bool rights = false;
    if (castlingRights(Color::White, true)) { fen += 'K'; rights = true; }
    if (castlingRights(Color::White, false)) { fen += 'Q'; rights = true; }
    if (castlingRights(Color::Black, true)) { fen += 'k'; rights = true; }
    if (castlingRights(Color::Black, false)) { fen += 'q'; rights = true; }
    if (!rights) { fen += '-'; }

    // enpassant
    fen += ' ';
    if (history_.empty()) {
        fen += "-";
    } else {
        const Move &last = history_.back().move;
        const auto &piece = squares_[last.to.row][last.to.col];
        if (piece != nullptr && piece->getType() == PieceType::Pawn) {
            int diff = last.to.row - last.from.row;
            if (diff == 2 || diff == -2) {
                int x = last.to.col;
                int y = (last.from.row + last.to.row) / 2;
                Position square{ y, x };
                std::string s = squareToString(square);
                fen += s;
            } else {
                fen += "-";
            }
        } else {
            fen += "-";
        }
    }
    return fen;
}

std::string Board::boardFEN() const {
    std::string fen = getKey();
    fen += ' ' + std::to_string(fiftyMoveCount());
    fen += ' ' + std::to_string(1 + history_.size() / 2);
    return fen;
}

int Board::repeatCount() const {
    int n = static_cast<int>(history_.size());
    std::string current = getKey();
    int count = 1;
    for (int i = n - 2; i >= 0; i -= 2) {
        if (history_[i].previousKey == current)
        {
            ++count;
        }
    }
    return count;
}

void Board::appendSanSuffix(char suffix)
{
    if (!history_.empty()) {
        history_.back().san += suffix;
    }
}
