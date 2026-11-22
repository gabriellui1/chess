#include "GameController.h"
#include <iostream>
#include <string>
#include <typeinfo>

using namespace std;

GameController::GameController(TextView &view_) : view(view_)
{
    board.attach(std::unique_ptr<BoardObserver>(&view));
}

void GameController::run()
{
    std::cout << "not world\n";

    std::string input;
    while (std::getline(std::cin, input))
    {
        Command command = parsers.parse(input, std::cout);

        if (command.type == CommandType::Game)
        {
            std::cout << "new game\n";
            board.loadStandardPosition();
        }
        else if (command.type == CommandType::Move)
        {
            std::cout << "move\n";
        }
        else if (command.type == CommandType::Resign)
        {
            std::cout << "resign\n";
        }
        else if (command.type == CommandType::Setup)
        {
            std::cout << "setup\n";
        }
        else if (command.type == CommandType::Done)
        {
            std::cout << "done\n";
        }
        else
        {
            std::cout << "invalid command\n";
        }
    }

    std::cout << "end";
}