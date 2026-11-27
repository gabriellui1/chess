// rename this file to controller.cpp
#include "GameController.h"
#include "Computer.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Level4.h"
#include "TextView.h"
#include "Helper.h"
#include <iostream>
#include <string>
#include <typeinfo>
#include <format>

using namespace std;

GameController::GameController(std::shared_ptr<TextView> view)
    : view_(std::move(view)),
      state_(GameState::Menu),
      rules_({}),
      turn_(Color::White),
      customStart_(false),
      undoEnabled_(false)
{
    board_.attach(view_);
}

std::unique_ptr<Player> makePlayer(const PlayerType &type, Color color, const Rules &rules)
{
    switch (type)
    {
    case PlayerType::Human:
        return std::make_unique<Human>(color);
    case PlayerType::Computer1:
        return std::make_unique<Level1>(color, rules);
    case PlayerType::Computer2:
        return std::make_unique<Level2>(color, rules);
    case PlayerType::Computer3:
        return std::make_unique<Level3>(color, rules);
    case PlayerType::Computer4:
        return std::make_unique<Level4>(color, rules);
    }
    return nullptr;
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

    printFinalScore();
    view_->show("End program");
}

void GameController::execute(const Command &cmd)
{
    view_->show(cmd.errMessages);
    view_->clearMessages();
    switch (cmd.type)
    {
    case CommandType::Game:
        startGame(cmd.whiteSpec, cmd.blackSpec, cmd.undoEnabled);
        break;
    case CommandType::Move:
        playMove(cmd);
        break;
    case CommandType::Setup:
        enterSetup();
        break;
    case CommandType::Place:
        setupPlace(cmd);
        break;
    case CommandType::Remove:
        setupRemove(cmd);
        break;
    case CommandType::SetTurn:
        view_->show("Set turn");
        turn_ = cmd.color;
        break;
    case CommandType::Resign:
        resign();
        break;
    case CommandType::Done:
        finishSetup();
        break;
    case CommandType::Help:
        switch (state_)
        {
        case GameState::Menu:
            view_->showStartMenu();
            break;
        case GameState::Game:
            view_->showGameHelp();
            break;
        case GameState::Setup:
            view_->showSetupHelp();
            break;
        }
        break;
    case CommandType::Undo:
        undoMove();
        break;
    default:
        break;
    }
    view_->pushMessages(std::format("You are currently in {} state: ", stateToString(state_)));
    view_->showMessages();
}

void GameController::playMove(const Command &cmd)
{

    std::unique_ptr<Player> &currentPlayer = (turn_ == Color::White) ? whitePlayer_ : blackPlayer_;
    if (currentPlayer->isHuman() && (!isSquareValid(cmd.from) || !isSquareValid(cmd.to)))
    {
        view_->clearMessages();
        view_->pushMessages("Please enter valid squares for your move.");
        return;
    }
    if (!currentPlayer->isHuman() && (isSquareValid(cmd.from) || isSquareValid(cmd.to)))
    {
        view_->clearMessages();
        view_->pushMessages("Computer cannot accept move input.");
        return;
    }
    std::vector<Move> chosen = currentPlayer->choose(cmd, board_, legal_);

    if (chosen.empty() || !isSquareValid(chosen[0].from) || !isSquareValid(chosen[0].to))
    {
        view_->clearMessages();
        view_->pushMessages("Illegal Move!");
        return;
    }
    board_.apply(chosen[0]);
    // the code pieceToString(board_.at(chosen[0].to)->getType()) is scuffed, pls fix
    view_->pushMessages(std::format("Moved {} {} from {} to {}", colorToString(turn_), pieceToString(board_.at(chosen[0].to)->getType()), squareToString(chosen[0].from), squareToString(chosen[0].to)));
    turn_ = (turn_ == Color::White) ? Color::Black : Color::White;
    legal_ = rules_.legalMoves(board_, turn_); // i dont know what these do
    switch (rules_.status(board_, turn_, legal_))
    {
    case GameStatus::Check:
        view_->pushMessages((turn_ == Color::White) ? "White is in check." : "Black is in check.");
        break;
    case GameStatus::Checkmate:
        view_->pushMessages((turn_ == Color::Black) ? "Checkmate! White wins!" : "Checkmate! Black wins!");
        scores_.addWin((turn_ == Color::White) ? Color::Black : Color::White);
        state_ = GameState::Menu;
        break;
    case GameStatus::Stalemate:
        view_->pushMessages("Stalemate!");
        scores_.addDraw();
        state_ = GameState::Menu;
        break;
    case GameStatus::ThreeFoldRepetition:
        view_->pushMessages("Threefold repetition.");
        scores_.addDraw();
        state_ = GameState::Menu;
        break;
    case GameStatus::FiftyMove:
        view_->pushMessages("Draw by fifty moves rule");
        scores_.addDraw();
        state_ = GameState::Menu;
        break;
    }
}

void GameController::startGame(PlayerType whiteSpec, PlayerType blackSpec, bool undoEnabled)
{
    view_->pushMessages("New game");
    undoEnabled_ = undoEnabled;
    if (!customStart_)
    {
        board_.loadStandardPosition();
        turn_ = Color::White;
    }
    else
    {
        view_->onBoardChanged(board_);
        customStart_ = false;
    }
    legal_ = rules_.legalMoves(board_, turn_);

    if (rules_.status(board_, turn_, legal_) == GameStatus::Stalemate)
    {
        view_->clearMessages();
        view_->pushMessages("Stalemate!");
        state_ = GameState::Menu;
        return;
    }
    state_ = GameState::Game;
    whitePlayer_ = makePlayer(whiteSpec, Color::White, rules_);
    blackPlayer_ = makePlayer(blackSpec, Color::Black, rules_);
}

void GameController::setupPlace(const Command &cmd)
{
    view_->pushMessages(std::format("Place {} {} at {}", colorToString(cmd.color), pieceToString(cmd.piece), squareToString(cmd.square)));
    std::unique_ptr<Piece> piece = PieceFactory::create(cmd.piece, cmd.color);
    board_.place(cmd.square, piece);
}

void GameController::setupRemove(const Command &cmd)
{
    if (board_.at(cmd.square) == Board::outOfBoundsPtr_)
    {
        view_->pushMessages("No piece at " + squareToString(cmd.square) + ".");
        return;
    }
    // the code pieceToString(board_.at(chosen[0].to)->getType()) is scuffed, pls fix
    view_->pushMessages(std::format("Remove {} {} at square {}", colorToString(cmd.color), pieceToString(board_.at(cmd.square)->getType()), squareToString(cmd.square)));
    board_.remove(cmd.square);
}

void GameController::enterSetup()
{
    view_->pushMessages("Entered setup mode.");
    board_.clear();
    turn_ = Color::White;
    customStart_ = false;
    state_ = GameState::Setup;
}

void GameController::finishSetup()
{
    std::vector<std::string> messages = rules_.validateSetup(board_);
    view_->show(messages);
    if (messages.empty())
    {
        view_->pushMessages("Exited setup mode and saved board!");
        state_ = GameState::Menu;
        customStart_ = true;
    }
}

void GameController::resign()
{
    Color winner = (turn_ == Color::White) ? Color::Black : Color::White;
    view_->pushMessages((winner == Color::White) ? "Black resigns. White wins!" : "White resigns. Black wins!");
    scores_.addWin(winner);
    state_ = GameState::Menu;
}

void GameController::printFinalScore()
{
    view_->show("Final Score:");
    view_->show("White: " + std::to_string(scores_.whiteScore()));
    view_->show("Black: " + std::to_string(scores_.blackScore()));
}

void GameController::undoMove()
{
    if (!undoEnabled_)
    {
        view_->pushMessages("Undo is not enabled for this game.");
        return;
    }
    if (!board_.canUndo())
    {
        view_->pushMessages("There is no move to undo.");
        return;
    }
    board_.undo();
    turn_ = !turn_;
    legal_ = rules_.legalMoves(board_, turn_);
    view_->pushMessages("Undid the last move.");
    if (rules_.status(board_, turn_, legal_) == GameStatus::Check)
    {
        view_->pushMessages((turn_ == Color::White) ? "White is in check." : "Black is in check.");
    }
}
