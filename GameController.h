#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include <memory>
#include "Board.h"
#include "Rules.h"
#include "CommandParser.h"
#include "Player.h"
#include "ScoreBoard.h"
#include "CommandParser.h"
#include "TextView.h"
#include "Player.h"
#include "Types.h"

class GameController {
private:
    Board board;
    Rules rule;
    ScoreBoard scores;
    TextView& view;
    CommandParser parsers;
    std::vector<std::unique_ptr<Player>> players;
    std::vector<Move> legal;

    void execute(const Command& command);
    void runStartMenu();
    void startGame(std::string whiteSpec, std::string blackSpec);
    void playMove(const Command&);
    void resign();
    void setupCommand(const Command&);
    void finishSetup();
    void printFinalScore();

public:
    GameController(TextView& view);
    void run();
};

#endif
