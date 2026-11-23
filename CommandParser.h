#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <string>
#include "Types.h"

class CommandParser
{
private:
public:
    Command parse(const std::string &line, std::ostream &out) const; // added extra argument for debugging pls remove later.
};

#endif
