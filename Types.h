#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <vector>

enum class Color { White, Black };
enum class MoveKind {Normal, DoublePawnStep, Castle, EnPassant, Promotion};
enum class PieceType {King, Queen, Rook, Bishop, Knight, Pawn};
enum class GameState {Menu, Idle, Setup, Playing};
enum class GameStatus {Ongoing, Check, Checkmate, Stalemate};

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

#endif
