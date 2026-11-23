// rename this file to controller.cpp
#include "GameController.h"
#include "Computer.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Level4.h"
#include "TextView.h"
#include "DataManipHelper.h"
#include <iostream>
#include <string>
#include <typeinfo>

using namespace std;

GameController::GameController(std::shared_ptr<TextView> view) : view_(std::move(view)), state_(GameState::Menu), rules_(Rules{})
{
    board_.attach(view);
}

std::unique_ptr<Player> makePlayer(const PlayerType &spec, const Rules &rules)
{
    switch (spec)
    {
    case PlayerType::Human:
        return std::make_unique<Human>();
    case PlayerType::Computer1:
        return std::make_unique<Level1>(rules);
    case PlayerType::Computer2:
        return std::make_unique<Level2>(rules);
    case PlayerType::Computer3:
        return std::make_unique<Level3>(rules);
    case PlayerType::Computer4:
        return std::make_unique<Level4>(rules);
    }
}

void GameController::run()
{
    std::cout << "hi" << std::endl;

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

    view_->showMessage("end");
}

void GameController::execute(const Command &cmd)
{

    switch (cmd.type)
    {
    case CommandType::Game:
        view_->showMessage("new game\n");
        startGame(cmd.whiteSpec, cmd.blackSpec);
        break;
    case CommandType::Move:
        view_->showMessage("move\n");
        playMove(cmd);
        break;
    case CommandType::Setup:
        view_->showMessage("Setup\n");
        board_.loadStandardPosition();
        state_ = GameState::Setup;

        break;
    case CommandType::Place:
        view_->showMessage("place\n");
        {
            std::unique_ptr<Piece> piece = PieceFactory::create(cmd.piece, cmd.color);
            board_.place(cmd.square, piece);
        }
        break;
    case CommandType::Remove:
        view_->showMessage("remove\n");
        board_.remove(cmd.square);
        break;
    case CommandType::SetTurn:
        view_->showMessage("set turn\n");
        turn_ = cmd.color;
        break;
    case CommandType::Done:
        view_->showMessage("done\n");
        state_ = GameState::Menu;
        break;
    default:
        view_->showMessage("invalid command\n");
        break;
    }
    view_->onBoardChanged(board_);
}

void GameController::playMove(const Command &cmd)
{

    std::unique_ptr<Player> &currentPlayer = (turn_ == Color::White) ? whitePlayer_ : blackPlayer_;
    if (currentPlayer->isHuman())
    {
    }
    std::vector<Move> chosen = currentPlayer->choose(cmd, board_, legal_);
    if (chosen.empty())
    {
        view_->showError("Illegal Move");
        return;
    }
    board_.apply(chosen[0]);
    turn_ = (turn_ == Color::White) ? Color::Black : Color::White;
    legal_ = rules_.legalMoves(board_, turn_); // i dont know what these do
    rules_.status(board_, turn_, legal_);
}

void GameController::startGame(PlayerType whiteSpec, PlayerType blackSpec)
{
    if (!customStart_)
        board_.loadStandardPosition();

    whitePlayer_ = makePlayer(whiteSpec, rules_);
    blackPlayer_ = makePlayer(blackSpec, rules_);
    turn_ = Color::White;
    legal_ = rules_.legalMoves(board_, turn_);
    state_ = GameState::Game;
}
