#include "ScoreBoard.h"

void ScoreBoard::addWin(Color color)
{
    if (color == Color::White)
        white_ += 1.0;
    if (color == Color::Black)
        black_ += 1.0;
}

void ScoreBoard::addDraw()
{
    white_ += 0.5;
    black_ += 0.5;
}

double ScoreBoard::whiteScore() const
{
    return white_;
}

double ScoreBoard::blackScore() const
{
    return black_;
}