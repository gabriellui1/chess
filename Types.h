// we should make enum for type of player Human, Computer1, Computer2, just like PieceType

#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <vector>
#include <unordered_map>

enum class Color
{
    White,
    Black
};

constexpr Color operator!(Color c)
{
    if (c == Color::White)
    {
        return Color::Black;
    }
    return Color::White;
}

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

const std::unordered_map<PieceType, int> PIECE_VALUES = {
    { PieceType::Pawn,   1 },
    { PieceType::Knight, 3 },
    { PieceType::Bishop, 3 },
    { PieceType::Rook,   5 },
    { PieceType::Queen,  9 },
    { PieceType::King,   -1 }
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
    Done,
    Help
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
    int row = -1;
    int col = -1;
    bool operator==(const Position &other) const = default;
};

struct Offset
{
    int drow = 0;
    int dcol = 0;
};

struct Move
{
    Position from = Position{-1, -1};
    Position to = Position{-1, -1};
    MoveKind kind = MoveKind::Normal;
    std::vector<PieceType> promoteTo = {};
};

struct Command
{
    CommandType type = CommandType::Invalid;
    std::vector<std::string> errMessages = {};
    Position from = Position{-1, -1};
    Position to = Position{-1, -1};
    PieceType piece = PieceType::Pawn;        // empty unless move promotes
    PlayerType whiteSpec = PlayerType::Human; // player type, uses player type enum class
    PlayerType blackSpec = PlayerType::Human;
    Position square = Position{-1, -1}; // setup mode
    Color color = Color::White;         // setup mode
};

#endif
