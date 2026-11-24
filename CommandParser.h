#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <string>
#include "Types.h"

class CommandParser
{
private:
public:
    Command parseMenu(const std::string &line) const;
    Command parseGame(const std::string &line) const; // added extra argument for debugging pls remove later.
    Command parseSetup(const std::string &line) const;
    Command parse(const std::string &line, GameState state) const;
};

#endif
