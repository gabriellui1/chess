#ifndef TYPES_H
#define TYPES_H

#include <string>

enum class CommandType { Move, Resign, Setup, Done, Invalid };
enum class Color { White, Black };

struct Position {
    int row;
    int col;
};

struct Move {
    Position from;
    Position to;
};

struct Command {
    CommandType type;
    std::string rawInput;
    Move move;
};

#endif
