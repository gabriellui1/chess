#include "Pieces.h"
#include "Board.h"
#include <vector>


King::King(Color colour): Piece{colour}{}

PieceType King::getType() const{
    return PieceType::King;
}

std::vector<Move> King::pseudoMoves(const Board& board, Position from) const{
    static const std::vector<Offset> direction = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    int maxSteps = 1;
    return toMoves(board, from, slide(board, direction, from, maxSteps));
}


Queen::Queen(Color colour): Piece{colour}{}

PieceType Queen::getType() const{
    return PieceType::Queen;
}

std::vector<Move> Queen::pseudoMoves(const Board& board, Position from) const{
    static const std::vector<Offset> direction = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    static const int maxSteps = 7;
    return toMoves(board, from, slide(board, direction, from, maxSteps));
}



Rook::Rook(Color colour): Piece{colour}{}

PieceType Rook::getType() const{
    return PieceType::Rook;
}

std::vector<Move> Rook::pseudoMoves(const Board& board, Position from) const{
    static const std::vector<Offset> direction = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    static const int maxSteps = 7;
    return toMoves(board, from, slide(board, direction, from, maxSteps));
}



Bishop::Bishop(Color colour): Piece{colour}{}

PieceType Bishop::getType() const{
    return PieceType::Bishop;
}

std::vector<Move> Bishop::pseudoMoves(const Board& board, Position from) const{
    static const std::vector<Offset> direction = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    static const int maxSteps = 7;
    return toMoves(board, from, slide(board, direction, from, maxSteps));
}



Knight::Knight(Color colour): Piece{colour}{}

PieceType Knight::getType() const{
    return PieceType::Knight;
}

std::vector<Move> Knight::pseudoMoves(const Board& board, Position from) const{
    static const std::vector<Offset> direction = {{1, 2}, {2, 1}, {-1, 2}, {-2, 1}, {1, -2}, {2, -1}, {-1, -2}, {-2, -1}};
    return toMoves(board, from, jump(board, from, direction));
}


char King::getChar() const{
    return getColour() == Color::White ? 'K' : 'k';
}

char Queen::getChar() const{
    return getColour() == Color::White ? 'Q' : 'q';
}

char Bishop::getChar() const{
    return getColour() == Color::White ? 'B' : 'b';
}

char Knight::getChar() const{
    return getColour() == Color::White ? 'N' : 'n';
}

char Rook::getChar() const{
    return getColour() == Color::White ? 'R' : 'r';
}
