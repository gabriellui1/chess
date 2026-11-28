#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include "Types.h"

class ScoreBoard
{
private:
    double white_ = 0.0; // draw gives half points
    double black_ = 0.0;

public:
    void addWin(Color color);
    void addDraw();
    double whiteScore() const;
    double blackScore() const;
};

#endif