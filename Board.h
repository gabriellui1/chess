#ifndef BOARD_H
#define BOARD_H

#include "Types.h"
#include "BoardObserver.h"
#include <vector>
#include <memory>
#include "Piece.h"

class Board
{
private:
    struct UndoRecord;
    std::vector<std::vector<std::unique_ptr<Piece>>> squares_;
    std::vector<UndoRecord> history_;
    std::vector<BoardObserver *> observers_; // non-owning
    void notifyObservers();

public:
    Board();
    ~Board();

    bool isOnBoard(Position pos) const; // funcction chekcing whether a square is on board given its position
    const Piece *at(Position pos) const;
    void place(Position pos, std::unique_ptr<Piece> piece);
    void remove(Position pos);
    void clear();
    void loadStandardPosition();
    std::vector<Move> lastMove() const;
    std::vector<Position> kingSquares(Color color) const; // extacly 1 king per side
    void apply(const Move &move);
    void undo();                       // notify observer
    void applyTrial(const Move &move); // without notifying observer
    void undoTrial();
    void attach(BoardObserver *ob);
    void detach(BoardObserver *ob);
};

#endif
