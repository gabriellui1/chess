#include "CommandParser.h"
#include <sstream>

Command CommandParser::parse(const std::string& command) const {
    Command cmd{};
    cmd.type = CommandType::Invalid;

    std::istringstream iss(command);
    std::string word;
    iss >> word;

    if (word == "game") {
        cmd.type = CommandType::Game;
    } else if (word == "move") {
        cmd.type = CommandType::Move;
    } else if (word == "resign") {
        cmd.type = CommandType::Resign;
    } else if (word == "setup") {
        cmd.type = CommandType::Setup;
    } else if (word == "done") {
        cmd.type = CommandType::Done;
    }

    return cmd;
}
