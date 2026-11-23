// rename this file to controller.cpp
#include "GameController.h"
#include "Computer.h"
#include "TextView.h"
#include "DataManipHelper.h"
#include <iostream>
#include <string>
#include <typeinfo>

using namespace std;

// std::unique_ptr<Player> makePlayer(const PlayerType &spec /*, const Rules &rules*/)
// {
//     switch (spec)
//     {
//     case PlayerType::Human:
//         return std::make_unique<Human>();
//     case PlayerType::Computer1:
//         return std::make_unique<Computer>(); // change these values to real computer levels later
//     case PlayerType::Computer2:
//         return std::make_unique<Computer>();
//     case PlayerType::Computer3:
//         return std::make_unique<Computer>();
//     case PlayerType::Computer4:
//         return std::make_unique<Computer>();
//     }
// }

GameController::GameController(std::shared_ptr<TextView> view) : view_(std::move(view)), state_(GameState::Menu)
{
    board_.attach(view);
}

void GameController::run()
{
    std::string input;
    Command command;
    while (std::getline(std::cin, input))
    {
        switch (state_)
        {
        case GameState::Menu:
            command = parser_.parseMenu(input, std::cout);
            break;
        case GameState::Setup:
            command = parser_.parseSetup(input, std::cout);
            break;
        case GameState::Game:
            command = parser_.parseGame(input, std::cout);
            break;
        }
        execute(command);
    }

    std::cout << "end";
}

void GameController::execute(const Command &cmd)
{

    switch (cmd.type)
    {
    case CommandType::Game:
        std::cout << "new game\n";
        board_.loadStandardPosition();
        // makePlayer(cmd.whiteSpec);
        // makePlayer(cmd.blackSpec);
        break;
    case CommandType::Move:
        std::cout << "move\n";
        // playMove(cmd);
        break;
    case CommandType::Setup:
        std::cout << "Setup\n";
        board_.loadStandardPosition();
        state_ = GameState::Setup;

        break;
    case CommandType::Place:
        std::cout << "place\n";
        {
            std::unique_ptr<Piece> piece = PieceFactory::create(cmd.piece, cmd.color);
            board_.place(cmd.square, piece);
        }
        break;
    case CommandType::Remove:
        std::cout << "remove\n";
        board_.remove(cmd.square);
        break;
    case CommandType::SetTurn:
        std::cout << "set turn\n";
        turn_ = cmd.color;
        break;
    case CommandType::Done:
        std::cout << "done\n";
        state_ = GameState::Menu;
        break;
    default:
        std::cout << "invalid command\n";
        break;
    }
    // temp code
    ostream &out_ = std::cout;
    out_ << std::endl;
    for (int row = 7; row >= 0; row--)
    {
        out_ << row + 1 << " ";
        for (int col = 0; col <= 7; col++)
        {
            Position pos{row, col};
            out_ << ((board_.at(pos) == nullptr) ? ((row + col) % 2 == 0 ? '_' : ' ') : symbolFor(*board_.at(pos)));
        }
        out_ << std::endl;
    }
    out_ << std::endl
         << "  abcdefgh" << std::endl;
}

void playMove(const Command &cmd)
{
}

void startGame(PlayerType whiteSpec, PlayerType blackSpec)
{
}
