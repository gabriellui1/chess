#include "PieceFactory.h"
#include "Pieces.h"
#include "Pawn.h"
#include <cctype>

std::unique_ptr<Piece> PieceFactory::create(PieceType piecetype, Color colour){
    switch(piecetype){
        case PieceType::King:
            return std::make_unique<King>(colour);
        case PieceType::Queen:
            return std::make_unique<Queen>(colour);
        case PieceType::Bishop:
            return std::make_unique<Bishop>(colour);
        case PieceType::Pawn:
            return std::make_unique<Pawn>(colour);
        case PieceType::Knight:
            return std::make_unique<Knight>(colour);
        case PieceType::Rook:
            return std::make_unique<Rook>(colour);
    }
    return nullptr;
}

std::unique_ptr<Piece> PieceFactory::fromSymbol(char c){
    Color color = std::isupper(static_cast<unsigned char>(c)) ? Color::White : Color::Black;
    char upper = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    switch (upper) {
        case 'K': return create(PieceType::King, color);
        case 'Q': return create(PieceType::Queen, color);
        case 'R': return create(PieceType::Rook, color);
        case 'B': return create(PieceType::Bishop, color);
        case 'N': return create(PieceType::Knight, color);
        case 'P': return create(PieceType::Pawn, color);
    }
    return nullptr; 
}