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
#include "Computer.h"
#include "Types.h"

class GameController
{
private:
    Board board_;
    Rules rules_;
    ScoreBoard scores_;
    std::shared_ptr<TextView> view_;
    CommandParser parser_;
    std::unique_ptr<Player> whitePlayer_;
    std::unique_ptr<Player> blackPlayer_;
    std::vector<Move> legal_;
    Color turn_;
    GameState state_;
    bool customStart_;

    void execute(const Command &cmd);
    // bool runStartMenu(); menu and idle are same thing
    void startGame(PlayerType whiteSpec, PlayerType blackSpec); // change from string to playerType
    void playMove(const Command &cmd);
    void resign();
    void setupCommand(const Command &cmd);
    void finishSetup();
    void printFinalScore();

public:
    GameController(std::shared_ptr<TextView> view);
    void run();
};

#endif
