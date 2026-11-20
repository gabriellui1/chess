#include "Board.h"

struct Board::UndoRecord {
    Move move;
    std::unique_ptr<Piece> captured;
    Position capturedAt;
    std::unique_ptr<Piece> replacedPawn;
    bool previouslyMoved = false;
};

Board::Board() = default;
Board::~Board() = default;

void Board::attach(BoardObserver* observer) {
    observers_.push_back(observer);
}

void Board::notifyObservers() {
    for (BoardObserver* observer : observers_) {
        observer->onBoardChanged(*this);
    }
}

void Board::loadStandardPosition() {
    // TODO: actually load the position
    notifyObservers();
}
