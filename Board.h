#ifndef BOARD_H
#define BOARD_H

#include "Types.h"
#include "BoardObserver.h"
#include <vector>
#include <memory>
#include "Piece.h"
#include "PieceFactory.h"

class Board
{
private:
    static inline const std::unique_ptr<Piece> outOfBoundsPtr_{};
    struct UndoRecord;
    std::vector<std::vector<std::unique_ptr<Piece>>> squares_;
    std::vector<UndoRecord> history_;
    std::vector<std::unique_ptr<BoardObserver>> observers_; // non-owning
    void notifyObservers();
    void applyMove(const Move &move);
    void undoMove();
    void emptySquares();

public:
    Board();
    ~Board();

    bool isOnBoard(Position pos) const; // funcction chekcing whether a square is on board given its position
    const std::unique_ptr<Piece> &at(Position pos) const;
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
    void attach(std::unique_ptr<BoardObserver> ob);
    void detach(std::unique_ptr<BoardObserver> ob);
};

#endif
