#include "TextView.h"
#include "Board.h"

TextView::TextView(std::ostream &out) : out_(out) {}

void TextView::drawBoard(const Board &board)
{
    for (int row = 7; row >= 0; row--)
    {
        out_ << row + 1 << " ";
        for (int col = 0; col <= 7; col++)
        {
            Position pos{row, col};
            out_ << (board.at(pos) == nullptr) ? ((row + col) % 2 == 0 ? '_' : ' ') : board.at(pos)->getChar();
        }
        out_ << std::endl;
    }
    out_ << std::endl
         << "  abcdefgh";
}

void TextView::onBoardChanged(const Board &board)
{
    drawBoard(board);
}

void TextView::showMessage(const std::string &message)
{
    out_ << message << std::endl;
}

void TextView::showError(const std::string &error)
{
    out_ << error << std::endl;
}

void TextView::showStartMenu()
{
    out_ << "Welcome to the chess menu!\nChoose one of the commands:\n\ngame <white-player> <black-player>\n\tstarts a new game with <white-player> and <black-player>\n\t<white-player>: human | computer1 | computer2 | computer3 | computer4\n\t\tchoose the type of player playing with the white pieces\n\t<black-player>: human | computer1 | computer2 | computer3 | computer4\n\t\tchoose the type of player playing with the black pieces\n\nsetup\n\tenters setup mode to set up your own initial board configurations\n\nCTRL+D\n\texits chess";
}