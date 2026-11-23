// TODO: need to split helper methods into different files and include these files or make them static

#include "CommandParser.h"
#include <sstream>
#include <iostream>
#include "DataManipHelper.h"

// does not modify command except for type
bool isWordValid(std::istringstream &iss, std::string &word, Command &cmd, const std::vector<std::string> validWords = {}, bool readWord = true)
{
    if (readWord)
        iss >> word;

    if (iss.fail() || (!validWords.empty() &&
                       findIndex(validWords, toLower(word)) == -1))
    {
        cmd.type = CommandType::Invalid;
        return false;
    }

    return true;
}

// modifies square, does not take cmd
bool parseSquare(const std::string word, Position &square)
{
    if (word.size() == 2)
        return false;
    int index = findCharIndex("abcdefgh", toLower(word[0]));
    if (index == -1 || word[1] - '1' < 0 || word[1] - '1' > 7)
        return false;
    square.col = index;
    square.row = word[1] - '1';
    return true;
}

bool isSquareValid(std::istringstream &iss, std::string &word, Position &square)
{
    iss >> word;
    return parseSquare(word, square);
}

Command CommandParser::parse(const std::string &command, std::ostream &out) const
{
    Command cmd{};

    std::istringstream iss(command);
    std::string word;

    iss >> word;
    out << word << std::endl;

    // if (!isWordValid(iss, word, cmd))
    //     return cmd;

    if (word == "game")
    {
        out << "hi" << std::endl;

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
