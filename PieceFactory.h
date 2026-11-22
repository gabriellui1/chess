#ifndef PIECEFACTORY_H
#define PIECEFACTORY_H
#include "Types.h"
#include "Piece.h"
#include <memory>
class PieceFactory
{
public:
    static std::unique_ptr<Piece> create(PieceType piecetype, Color color);
    static std::unique_ptr<Piece> fromSymbol(char c);
};

#endif