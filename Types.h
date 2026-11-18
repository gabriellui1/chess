#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <vector>

enum class Color { White, Black };
enum class MoveKind {Normal, DoublePawnStep, Castle, EnPassant, Promotion};
enum class PieceType {King, Queen, Rook, Bishop, Knight, Pawn};
enum class GameState {Menu, Idle, Setup, Playing};
enum class GameStatus {Ongoing, Check, Checkmate, Stalemate};
enum class CommandType { Move, Resign, Setup, Done, Invalid };


struct Position {
    int row;
    int col;
};

struct Offset{
    int drow;
    int dcol;
};

struct Move {
    Position from;
    Position to;
    MoveKind kind;
    std::vector<PieceType> promoteTo;
};


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

#endif
