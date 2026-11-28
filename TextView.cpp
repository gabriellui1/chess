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
    show("Welcome to the chess menu!\nChoose one of the commands:\n\ngame <white-player> <black-player> <undo>\n    starts a new game with <white-player> and <black-player>\n\n    <white-player>: human | computer1 | computer2 | computer3 | computer4\n        choose the type of player playing with the white pieces\n\n    <black-player>: human | computer1 | computer2 | computer3 | computer4\n        choose the type of player playing with the black pieces\n\n    <undo>: undo\n        Add in the word undo if you want to enable the undo command\n\n    ex. \"game human computer3 undo\" or \"game computer3 human\"  \n\nsetup\n    enters setup mode to set up your own initial board configurations. in this mode, you can place and remove your own pieces and play a game with the board you created.\n\nhelp\n    list of commands in your current state\n\nquit\n    exits chess");
}

void TextView::showSetupHelp()
{
    show("List of commands while in setup mode:\n\n+ <piece-type> <square>\n    places piece <piece-type> on square <square>. if there is already a piece on square <square>, replace it.\n\n    <piece-type>: Q | R | B | N | P | K | q | r | b | n | p | k \n        the piece placed on the board, shortened to their starting letter. uppercase will produce white pieces, and lowercase will produce black pieces.\n\n    <square>: [a1-h8]\n        square on the 8x8 board. letter [a-h] specifies file and number [1-8] specifies rank\n    \n    ex. \"+ K h1\"\n\n- <square>\n    if there exists a piece on square <square>, remove it and leave it empty.\n\n    <square>: [a1-h8]\n        square on the 8x8 board. letter [a-h] specifies file and number [1-8] specifies rank\n    \n    ex. \"- c7\"\n\n= <color>\n    sets the starting turn of the board to color <color>\n\n    <color>: white | black\n        when a game is started with this board, <color> will be the color that moves first\n\n    ex. \"= black\"\n\ndone\n    checks if the current board is ready for a game. if valid, saves the board for the start of the next game and exits setup mode. if invalid, displays an error message and does nothing. a valid board must have the following: exactly one white king and exactly one black king which are both not in check, and no pawns on the first or last row of the board.\n\nclear\n    removes every piece off the board\n\nreset\n    clears and resets the board to the standard position\n\nhelp\n    list of commands in your current state\n\nquit\n    exits chess\n\n");
}

void TextView::showGameHelp()
{
    show("List of available commands while playing a game:\n\nmove\n    on a computer player, calculates and performs the next move  \n\nmove <source> <destination>\n    on a human player, performs a move on one of your pieces. moves your piece from the square <source> to the square <destination>\n\n    <source>: [a1-h8]\n        source square on the 8x8 board. letter [a-h] specifies file and number [1-8] specifies rank\n\n    <destination>: [a1-h8]\n        destination square on the 8x8 board. letter [a-h] specifies file and number [1-8] specifies rank\n\n    ex. \"move e2 e4\"\n\nmove <source> <destination> <promoting-piece>\n    on a human player, performs a promoting pawn move on your pawn. moves your pawn from the square <source> to the square <destination> and promotes to the piece type <promoting-piece>\n\n    <promoting-piece>: Q | R | B | N\n        the piece that the pawn promotes to, shortened to their starting letter\n\n    ex. \"move a7 b8 B\"\n\nundo(if enabled)\n    reverts the last move, allowing a player to try a different line\n\nresign\n    forfeits the game as the current player and hands the win over to the opponent\n\nhelp\n    list of commands in your current state\n\nquit\n    exits chess\n\n\n");
}