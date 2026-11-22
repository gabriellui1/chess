#include "Types.h"
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>

#ifndef DATA_MANIP_HELPER
#define DATA_MANIP_HELPER

// finds index of element in vector, -1 if not found
template <typename T>
int findIndex(const std::vector<T> &vec, const T &target)
{
    auto it = std::find(vec.begin(), vec.end(), target);

    if (it != vec.end())
    {
        return std::distance(vec.begin(), it);
    }

    return -1;
}

// assumes you have already validated
PieceType charToPiece(const char c)
{
    std::string files = "qrbnpk";
    std::vector<PieceType> pieces({PieceType::Queen, PieceType::Rook, PieceType::Bishop, PieceType::Knight, PieceType::Pawn, PieceType::King});
    int index = files.find(toLower(c));
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

// turns every char in string to lowercase
std::string toLower(const std::string str)
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

#endif