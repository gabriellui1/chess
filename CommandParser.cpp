// TODO: need to split helper methods into different files and include these files or make them static

#include "CommandParser.h"
#include <sstream>
#include <algorithm>
#include <cctype>

// finds index of element in vector, -1 if not found
template <typename T>
int findIndex(const std::vector<T> &vec, const T &target)
{
    auto it = std::find(vec.begin(), vec.end(), target);

    if (it != vec.end())
    {
        return std::distance(vec.begin(), it);
    }

    return -1;
}

// assumes you have already validated
PieceType charToPiece(const char c)
{
    std::string files = "qrbnpk";
    std::vector<PieceType> pieces({PieceType::Queen, PieceType::Rook, PieceType::Bishop, PieceType::Knight, PieceType::Pawn, PieceType::King});
    int index = files.find(toLower(c));
    return pieces[index];
}

// assumes you have already validated
PlayerType stringToPlayer(const std::string str)
{
    std::vector<PlayerType> players({PlayerType::Human, PlayerType::Computer1, PlayerType::Computer2, PlayerType::Computer3, PlayerType::Computer4});
    std::vector<std::string> playerStrings({"human", "computer1", "computer2", "computer3", "computer4"});
    int index = findIndex(playerStrings, toLower(str));
    return players[index];
}

// turns every char in string to lowercase
std::string toLower(const std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c)
                   { return std::tolower(c); });
    return str;
}

// turns every char in string to lowercase
char toLower(const char c)
{
    return static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
}

// does not modify command except for type
bool isWordValid(std::istringstream &iss, std::string &word, Command &cmd, const std::vector<std::string> validWords = {}, bool readWord = true)
{
    if (readWord)
        iss >> word;

    if (iss.eof() || iss.fail() || (validWords.size() != 0 && findIndex(validWords, toLower(word)) == -1))
    {
        cmd.type = CommandType::Invalid;
        return false;
    }
    return true;
}

// modifies square, does not take cmd
bool parseSquare(const std::string word, Position &square)
{
    std::string files = "abcdefgh";
    int index = files.find(toLower(word[0]));
    if (index == std::string::npos || word[1] - '1' < 0 || word[1] - '1' > 7)
        return false;
    square.col = index;
    square.row = word[1] - '1';
    return true;
}

bool isSquareValid(std::istringstream &iss, std::string &word, Position &square)
{
    iss >> word;
    parseSquare(word, square);
    return iss.eof() || square.row == -1 || square.col == -1;
}

Command CommandParser::parse(const std::string &command) const
{
    Command cmd{};

    std::istringstream iss(command);
    std::string word;

    if (!isWordValid(iss, word, cmd))
        return cmd;

    if (word == "game")
    {

        std::vector<std::string> validWords({"human", "computer1", "computer2", "computer3", "computer4"});

        if (!isWordValid(iss, word, cmd, validWords))
            return cmd;
        cmd.whiteSpec = stringToPlayer(word);

        if (!isWordValid(iss, word, cmd, validWords))
            return cmd;
        cmd.blackSpec = stringToPlayer(word);

        cmd.type = CommandType::Game;
        return cmd;
    }

    if (word == "move")
    {
        if (!isSquareValid(iss, word, cmd.from))
            return cmd;

        if (!isSquareValid(iss, word, cmd.to))
        {
            cmd.type = CommandType::Invalid;
            return cmd;
        }

        std::vector<std::string> validWords({"q",
                                             "r",
                                             "b",
                                             "n"});

        iss >> word;
        if (iss.eof() || iss.fail())
            return cmd;

        // do not merge these if statements!
        if (!isWordValid(iss, word, cmd, validWords, false))
            return cmd;
        cmd.piece = charToPiece(word[0]);
        cmd.type = CommandType::Move;
        return cmd;
    }

    if (word == "+")
    {
        std::vector<std::string> validWords({"q",
                                             "r",
                                             "b",
                                             "n",
                                             "k",
                                             "p"});

        iss >> word;
        if (!isWordValid(iss, word, cmd, validWords))
            return cmd;
        cmd.piece = charToPiece(word[0]);
        cmd.color = toLower(word[0]) == word[0] ? Color::Black : Color::White;

        if (!isSquareValid(iss, word, cmd.square))
        {
            cmd.type = CommandType::Invalid;
            return cmd;
        }

        cmd.type = CommandType::Place;
        return cmd;
    }

    if (word == "-")
    {
        if (!isSquareValid(iss, word, cmd.square))
        {
            cmd.type = CommandType::Invalid;
            return cmd;
        }

        cmd.type = CommandType::Remove;
        return cmd;
    }

    if (word == "=")
    {
        std::vector<std::string> validWords({"white", "black"});

        if (!isWordValid(iss, word, cmd, validWords))
            return cmd;
        cmd.color = word == "white" ? Color::White : Color::Black;

        cmd.type = CommandType::SetTurn;
        return cmd;
    }

    if (word == "resign")
    {
        cmd.type = CommandType::Resign;
        return cmd;
    }
    else if (word == "setup")
    {
        cmd.type = CommandType::Setup;
        return cmd;
    }
    else if (word == "done")
    {
        cmd.type = CommandType::Done;
        return cmd;
    }

    cmd.type = CommandType::Invalid;
    return cmd;
}
