#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <string>;
#include "Types.h";


class CommandParser {
private:
public:
    Command parse(const std::string &command) const;
};

#endif
