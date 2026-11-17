#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "Board.h"
#include "Rules.h"
#include "CommandParser.h"
#include "Player.h"

class GameController {
private:
    Board board;
    Rules rule;

public:
    GameController();
    void run();
};

#endif
