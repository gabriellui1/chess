// TODO: need to split helper methods into different files and include these files or make them static

#include "CommandParser.h"
#include <sstream>
#include <iostream>
#include "DataManipHelper.h"

// does not modify command except for type
bool isWordValid(std::istringstream &iss, std::string &word, Command &cmd, const std::vector<std::string> validWords = {}, bool readWord = true)
{
    bool wordExists = true;
    if (readWord)
        wordExists = static_cast<bool>(iss >> word);

    if (!wordExists || iss.fail() || (!validWords.empty() && findIndex(validWords, toLower(word)) == -1))
    {
        cmd.type = CommandType::Invalid;
        return false;
    }

    return true;
}

// modifies square, does not take cmd
bool parseSquare(const std::string word, Position &square)
{
    if (word.size() != 2)
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

Command CommandParser::parseMenu(const std::string &command) const
{
    Command cmd;
    std::istringstream iss(command);
    std::string word;

    if (!isWordValid(iss, word, cmd))
        return cmd;

    if (word == "game" || word == "g")
    {
        bool wordExists = static_cast<bool>(iss >> word);
        if (!wordExists || iss.fail())
        {
            cmd.type = CommandType::Game;
            cmd.whiteSpec = PlayerType::Human;
            cmd.blackSpec = PlayerType::Human;
            return cmd;
        }
        std::vector<std::string> validWords({"human", "computer1", "computer2", "computer3", "computer4"});

        if (!isWordValid(iss, word, cmd, validWords, false))
        {
            cmd.errMessages.push_back("Not a valid player type for white.");
            return cmd;
        }
        cmd.whiteSpec = stringToPlayer(word);

        if (!isWordValid(iss, word, cmd, validWords))
        {
            cmd.errMessages.push_back("Not a valid player type for black.");
            return cmd;
        }
        cmd.blackSpec = stringToPlayer(word);
        cmd.type = CommandType::Game;
        return cmd;
    }

    else if (word == "setup")
    {
        cmd.type = CommandType::Setup;
        return cmd;
    }

    return cmd;
}

Command CommandParser::parseGame(const std::string &command) const
{
    Command cmd{};
    std::istringstream iss(command);
    std::string word;

    if (!isWordValid(iss, word, cmd))
        return cmd;

    if (word == "move" || word == "m")
    {
        if (!isSquareValid(iss, word, cmd.from))
        {
            cmd.type = CommandType::Move;
            return cmd;
        }

        if (!isSquareValid(iss, word, cmd.to))
        {
            cmd.errMessages.push_back("Missing destination square");
            return cmd;
        }

        std::vector<std::string> validWords({"q",
                                             "r",
                                             "b",
                                             "n"});

        bool wordExists = static_cast<bool>(iss >> word);
        if (!wordExists || iss.fail())
        {
            cmd.type = CommandType::Move;
            return cmd;
        }

        if (!isWordValid(iss, word, cmd, validWords, false))
        {
            cmd.errMessages.push_back("Not a valid promotion piece.");
            return cmd;
        }

        cmd.piece = charToPiece(word[0]);
        cmd.type = CommandType::Move;
        return cmd;
    }

    if (word == "resign")
    {
        cmd.type = CommandType::Resign;
        return cmd;
    }

    return cmd;
}

Command CommandParser::parseSetup(const std::string &command) const
{
    Command cmd{};
    std::istringstream iss(command);
    std::string word;

    if (!isWordValid(iss, word, cmd))
        return cmd;

    if (word == "+")
    {
        std::vector<std::string> validWords({"q",
                                             "r",
                                             "b",
                                             "n",
                                             "k",
                                             "p"});

        if (!isWordValid(iss, word, cmd, validWords))
        {
            cmd.errMessages.push_back("Not a valid piece type.");
            return cmd;
        }
        cmd.piece = charToPiece(word[0]);
        cmd.color = toLower(word[0]) == word[0] ? Color::Black : Color::White;

        if (!isSquareValid(iss, word, cmd.square))
        {
            cmd.errMessages.push_back("Not a valid piece square.");
            return cmd;
        }

        cmd.type = CommandType::Place;
        return cmd;
    }

    if (word == "-")
    {
        if (!isSquareValid(iss, word, cmd.square))
        {
            cmd.errMessages.push_back("Not a valid square.");
            return cmd;
        }

        cmd.type = CommandType::Remove;
        return cmd;
    }

    if (word == "=")
    {
        std::vector<std::string> validWords({"white", "black"});

        if (!isWordValid(iss, word, cmd, validWords))
        {
            cmd.errMessages.push_back("Change turn to a valid color.");
            return cmd;
        }
        cmd.color = word == "white" ? Color::White : Color::Black;

        cmd.type = CommandType::SetTurn;
        return cmd;
    }

    if (word == "done")
    {
        cmd.type = CommandType::Done;
        return cmd;
    }

    return cmd;
}

Command CommandParser::parse(const std::string &line, GameState state) const
{

    Command cmd;
    Command menuCommand = parseMenu(line);
    Command gameCommand = parseGame(line);
    Command setupCommand = parseSetup(line);

    // when you in wrong state
    if (state != GameState::Menu && menuCommand.type != CommandType::Invalid && gameCommand.type == CommandType::Invalid && setupCommand.type == CommandType::Invalid)
    {
        cmd.type = CommandType::Invalid;
        cmd.errMessages.push_back("Game/setup mode is currently running. Please end it first.");
        return cmd;
    }
    if (state != GameState::Game && gameCommand.type != CommandType::Invalid && menuCommand.type == CommandType::Invalid && setupCommand.type == CommandType::Invalid)
    {
        cmd.type = CommandType::Invalid;
        cmd.errMessages.push_back("No game is running.");
        return cmd;
    }
    if (state != GameState::Setup && setupCommand.type != CommandType::Invalid && menuCommand.type == CommandType::Invalid && gameCommand.type == CommandType::Invalid)
    {
        cmd.type = CommandType::Invalid;
        cmd.errMessages.push_back("Setup mode has not been started.");
        return cmd;
    }

    switch (state)
    {
    case GameState::Menu:
        cmd = menuCommand;
        break;
    case GameState::Game:
        cmd = gameCommand;
        break;
    case GameState::Setup:
        cmd = setupCommand;
        break;
    }
    if (cmd.errMessages.empty() && cmd.type == CommandType::Invalid)
    {
        cmd.errMessages.push_back("Invalid command.");
    }
    return cmd;
}