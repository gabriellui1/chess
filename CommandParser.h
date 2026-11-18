#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <string>;
#include "Types.h";

enum class CommandType { Move, Resign, Setup, Done, Invalid };

struct Command {
    CommandType type;
    std::vector<Position> from;
    std::vector<Position> to;
    std::vector<PieceType> promotion; //empty unless move promotes
    std::string whiteSpec; //player name
    std::string blackSpec;
    std::vector<Position> square; // setup mode
    char Symbol; //setup mode
    std::vector<Color> turnColor; //setup mode

};

class COMMAND_PARSER {
private:
public:
    Command parse(const std::string &command) const;
};

#endif
