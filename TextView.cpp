#include "TextView.h"
#include "Board.h"
#include "Helper.h"

TextView::TextView(std::ostream &out) : out_(out), messages_({}) {}

void TextView::drawBoard(const Board &board)
{
    out_ << std::endl;
    for (int row = 7; row >= 0; row--)
    {
        out_ << row + 1 << " ";
        for (int col = 0; col <= 7; col++)
        {
            Position pos{row, col};
            out_ << ((board.at(pos) == nullptr) ? ((row + col) % 2 == 0 ? '_' : ' ') : symbolFor(*board.at(pos)));
        }
        out_ << std::endl;
    }
    out_ << std::endl
         << "  abcdefgh" << std::endl;
}

void TextView::drawBetterBoard(const Board &board)
{
    out_ << std::endl;
    for (int row = 7; row >= 0; row--)
    {
        out_ << row + 1;
        for (int col = 0; col <= 7; col++)
        {
            Position pos{row, col};
            out_ << ((board.at(pos) == nullptr) ? "□" : chessSymbolFor(*board.at(pos)));
        }
        out_ << std::endl;
    }
    out_ << " abcdefgh" << std::endl;
}

void TextView::onBoardChanged(const Board &board)
{
    drawBetterBoard(board);
}

void TextView::show(const std::string &message)
{
    out_ << message << std::endl;
}

void TextView::show(const std::vector<std::string> &messages)
{
    for (std::string message : messages)
        show(message);
}

void TextView::showMessages()
{
    for (std::string message : messages_)
        show(message);
}

void TextView::clearMessages()
{
    messages_.clear();
}

void TextView::pushMessages(const std::string message)
{
    messages_.push_back(message);
}

void TextView::showStartMenu()
{
    show("Welcome to the chess menu!\nChoose one of the commands:\n\ngame <white-player> <black-player>\n\tstarts a new game with <white-player> and <black-player>\n\t<white-player>: human | computer1 | computer2 | computer3 | computer4\n\t\tchoose the type of player playing with the white pieces\n\t<black-player>: human | computer1 | computer2 | computer3 | computer4\n\t\tchoose the type of player playing with the black pieces\n\nsetup\n\tenters setup mode to set up your own initial board configurations\n\nCTRL+D\n\texits chess");
}