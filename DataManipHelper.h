
#ifndef DATA_MANIP_HELPER_H
#define DATA_MANIP_HELPER_H

#include "Types.h"
#include <algorithm>
#include <cctype>
#include <string>

class Piece;

// turns every char in string to lowercase
std::string toLower(const std::string str);

// turns every char in string to lowercase
char toLower(const char c);

// convert square to string
std::string squareToString(const Position square);

bool isSquareValid(const Position square);

// finds first index of char in string, -1 if not found
int findCharIndex(const std::string &str, char target);

//
char symbolFor(const Piece &piece);

// finds index of element in vector, -1 if not found
template <typename T>
int findIndex(const std::vector<T> &vec, const T &word)
{
    auto it = std::find(vec.begin(), vec.end(), word);

    if (it != vec.end())
    {
        return std::distance(vec.begin(), it);
    }

    return -1;
}
// assumes you have already validated
PieceType charToPiece(const char c);

std::string pieceToString(const PieceType piece);

// assumes you have already validated
PlayerType stringToPlayer(const std::string str);

#endif