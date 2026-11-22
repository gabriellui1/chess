#ifndef BOARD_OBSERVER_H
#define BOARD_OBSERVER_H

class Board;

class BoardObserver {
public:
    virtual ~BoardObserver() = default;
    virtual void onBoardChanged(const Board& board) = 0;
};

#endif
