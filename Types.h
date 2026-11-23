// we should make enum for type of player Human, Computer1, Computer2, just like PieceType

#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <vector>

enum class Color
{
    White,
    Black
};
enum class MoveKind
{
    Normal,
    DoublePawnStep,
    Castle,
    EnPassant,
    Promotion
};
enum class PieceType
{
    King,
    Queen,
    Rook,
    Bishop,
    Knight,
    Pawn
};
enum class GameState
{
    Menu,
    Setup,
    Game
};
enum class GameStatus
{
    Ongoing,
    Check,
    Checkmate,
    Stalemate
};
enum class CommandType
{
    Invalid,
    Game,
    Resign,
    Move,
    Setup,
    Place,
    Remove,
    SetTurn,
    Done
};

enum class PlayerType
{
    Human,
    Computer1,
    Computer2,
    Computer3,
    Computer4
};

struct Position
{
    int row;
    int col;
    bool operator==(const Position &other) const = default;
};

struct Offset
{
    int drow;
    int dcol;
};

struct Move
{
    Position from;
    Position to;
    MoveKind kind;
    std::vector<PieceType> promoteTo;
};

struct Command
{
    CommandType type;
    Position from;
    Position to;
    PieceType piece;      // empty unless move promotes
    PlayerType whiteSpec; // player type, uses player type enum class
    PlayerType blackSpec;
    Position square; // setup mode
    Color color;     // setup mode
};

#endif
