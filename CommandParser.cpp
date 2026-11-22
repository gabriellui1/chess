// Status: TBD

#include "CommandParser.h"
#include <sstream>
#include <algorithm>

bool isWordValid(std::istringstream &iss, std::string &word, Command &cmd, std::vector<std::string> validWords = {})
{
    iss >> word;
    if (iss.eof() || iss.fail() || (validWords.size() != 0 && std::find(validWords.begin(), validWords.end(), word) == validWords.end()))
    {
        cmd.type = CommandType::Invalid;
        return false;
    }
    return true;
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
        cmd.whiteSpec = word;

        if (!isWordValid(iss, word, cmd, validWords))
            return cmd;
        cmd.blackSpec = word;

        cmd.type = CommandType::Game;
        return cmd;
    }

    if (word == "move")
    {

        iss >> word;
        if (iss.eof())
            return cmd;

        Position pos;
        std::string files = "abcdefgh";
        int index = files.find(word[0]);
        if (index == std::string::npos)
        {
            cmd.type = CommandType::Invalid;
            return cmd;
        }
        pos.col = index;
        if (word[1] - '1' < 0 || word[1] - '1' > 7)
        {
            cmd.type = CommandType::Invalid;
            return cmd;
        }

        pos.row = word[1] - '1';
        // not done
        cmd.type = CommandType::Move;
        return cmd;
    }
    else if (word == "resign")
    {
        cmd.type = CommandType::Resign;
    }
    else if (word == "setup")
    {
        cmd.type = CommandType::Setup;
    }
    else if (word == "done")
    {
        cmd.type = CommandType::Done;
    }

    cmd.type = CommandType::Invalid;
    return cmd;
}
