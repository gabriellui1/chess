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

GameController::GameController(std::shared_ptr<TextView> view) : view_(std::move(view)), state_(GameState::Menu), rules_({}), messages_({})
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
    view_->showStartMenu();
    std::string input;
    Command cmd;
    while (std::getline(std::cin, input))
    {
        cmd = parser_.parse(input, state_);
        execute(cmd);
    }

    view_->showMessage("End program");
}

void GameController::execute(const Command &cmd)
{
    view_->showErrorMessages(cmd.errMessages);
    messages_.clear();
    switch (cmd.type)
    {
    case CommandType::Game:
        view_->showMessage("New game");
        startGame(cmd.whiteSpec, cmd.blackSpec);
        break;
    case CommandType::Move:
        view_->showMessage("Move");
        playMove(cmd);
        break;
    case CommandType::Setup:
        view_->showMessage("Setup");
        board_.clear();
        turn_ = Color::White;
        customStart_ = false;
        state_ = GameState::Setup;
        break;
    case CommandType::Place:
        view_->showMessage("Place");
        setupCommand(cmd);
        break;
    case CommandType::Remove:
        view_->showMessage("Remove");
        board_.remove(cmd.square);
        break;
    case CommandType::SetTurn:
        view_->showMessage("Set turn");
        turn_ = cmd.color;
        break;
    case CommandType::Done:
        view_->showMessage("Done");
        finishSetup();
        break;
    default:
        break;
    }
    view_->showMessages(messages_);
}

void GameController::playMove(const Command &cmd)
{

    std::unique_ptr<Player> &currentPlayer = (turn_ == Color::White) ? whitePlayer_ : blackPlayer_;
    std::vector<Move> chosen = currentPlayer->choose(cmd, board_, legal_);
    if (chosen.empty())
    {
        messages_.clear();
        messages_.push_back("Illegal Move!");
        return;
    }
    board_.apply(chosen[0]);
    turn_ = (turn_ == Color::White) ? Color::Black : Color::White;
    legal_ = rules_.legalMoves(board_, turn_); // i dont know what these do
    switch (rules_.status(board_, turn_, legal_))
    {
    case GameStatus::Check:
        messages_.push_back((turn_ == Color::Black) ? "White is in check." : "Black is in check.");
        break;
    case GameStatus::Checkmate:
        messages_.push_back((turn_ == Color::Black) ? "Checkmate! White wins!" : "Checkmate! Black wins!");
        scores_.addWin((turn_ == Color::White) ? Color::Black : Color::White);
        state_ = GameState::Menu;
        break;
    case GameStatus::Stalemate:
        messages_.push_back("Stalemate!");
        scores_.addDraw();
        state_ = GameState::Menu;
        break;
    }
    view_->onBoardChanged(board_);
}

void GameController::startGame(PlayerType whiteSpec, PlayerType blackSpec)
{
    if (!customStart_)
    {
        board_.loadStandardPosition();
        turn_ = Color::White;
    }
    else
        customStart_ = false;

    legal_ = rules_.legalMoves(board_, turn_);

    if (rules_.status(board_, turn_, legal_) == GameStatus::Stalemate)
    {
        messages_.clear();
        messages_.push_back("Stalemate!");
        state_ = GameState::Menu;
        return;
    }
    state_ = GameState::Game;
    whitePlayer_ = makePlayer(whiteSpec, rules_);
    blackPlayer_ = makePlayer(blackSpec, rules_);
    view_->onBoardChanged(board_);
}

void GameController::setupCommand(const Command &cmd)
{
    std::unique_ptr<Piece> piece = PieceFactory::create(cmd.piece, cmd.color);
    board_.place(cmd.square, piece);
}

void GameController::finishSetup()
{
    std::vector<std::string> messages = rules_.validateSetup(board_);
    for (std::string message : messages)
    {
        view_->showError(message);
    }
    if (messages.empty())
    {
        state_ = GameState::Menu;
        customStart_ = true;
    }
}

void GameController::resign()
{
    Color winner = (turn_ == Color::White) ? Color::Black : Color::White;
    scores_.addWin(winner);
    state_ = GameState::Menu;
}

void GameController::printFinalScore()
{
    view_->showMessage("Final Score:");
    view_->showMessage("White: " + std::to_string(scores_.whiteScore()));
    view_->showMessage("Black: " + std::to_string(scores_.blackScore()));
}