#include <sstream>
#include <limits>
#include "Helper.h"
#include "Piece.h"

// turns every char in string to lowercase
std::string toLower(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c)
                   { return std::tolower(c); });
    return str;
}

// turns every char in string to lowercase
char toLower(const char c)
{
    return static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
}

bool isSquareValid(const Position square)
{
    return square.col != -1 && square.row != -1;
}
// convert square to string
std::string squareToString(const Position square)
{
    std::string files = "abcdefgh";
    return {files[square.col], ('1' + square.row)};
}

// finds first index of char in string, -1 if not found
int findCharIndex(const std::string &str, char c)
{
    size_t pos = str.find(c);

    if (pos == std::string::npos || pos > static_cast<size_t>(std::numeric_limits<int>::max()))
    {
        return -1;
    }

    return static_cast<int>(pos);
}

char symbolFor(const Piece &piece)
{
    char c = '?';
    switch (piece.getType())
    {
    case PieceType::King:
        c = 'K';
        break;
    case PieceType::Queen:
        c = 'Q';
        break;
    case PieceType::Bishop:
        c = 'B';
        break;
    case PieceType::Pawn:
        c = 'P';
        break;
    case PieceType::Knight:
        c = 'N';
        break;
    case PieceType::Rook:
        c = 'R';
        break;
    default:
        c = '?';
    }
    return piece.getColor() == Color::White ? c : toLower(c);
}

std::string chessSymbolFor(const Piece &piece)
{
    switch (piece.getType())
    {
    case PieceType::King:
        return piece.getColor() == Color::White ? "♔" : "♚";
        break;
    case PieceType::Queen:
        return piece.getColor() == Color::White ? "♕" : "♛";
        break;
    case PieceType::Rook:
        return piece.getColor() == Color::White ? "♖" : "♜";
        break;
    case PieceType::Bishop:
        return piece.getColor() == Color::White ? "♗" : "♝";
        break;
    case PieceType::Knight:
        return piece.getColor() == Color::White ? "♘" : "♞";
        break;
    case PieceType::Pawn:
        return piece.getColor() == Color::White ? "♙" : "♟";
        break;
    }
    return "?";
}

std::string pieceToString(const PieceType piece)
{
    std::vector<std::string> pieceNames = {"Queen", "Rook", "Bishop", "Knight", "Pawn", "King"};
    std::vector<PieceType> pieces({PieceType::Queen, PieceType::Rook, PieceType::Bishop, PieceType::Knight, PieceType::Pawn, PieceType::King});
    int index = findIndex(pieces, piece);
    if (index == -1)
        return "";
    return pieceNames[index];
}

std::string colorToString(const Color color)
{
    return color == Color::White ? "White" : "Black";
}

// assumes you have already validated
PieceType charToPiece(const char c)
{
    std::vector<PieceType> pieces({PieceType::Queen, PieceType::Rook, PieceType::Bishop, PieceType::Knight, PieceType::Pawn, PieceType::King});
    int index = findCharIndex("qrbnpk", toLower(c));
    return pieces[index];
}

// assumes you have already validated
PlayerType stringToPlayer(const std::string str)
{
    std::vector<PlayerType> players({PlayerType::Human, PlayerType::Computer1, PlayerType::Computer2, PlayerType::Computer3, PlayerType::Computer4});
    std::vector<std::string> playerStrings({"human", "computer1", "computer2", "computer3", "computer4"});
    int index = findIndex(playerStrings, toLower(str));
    return players[index];
}
