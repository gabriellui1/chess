#include "Rules.h"
#include "Board.h"
#include "DataManipHelper.h"

std::vector<Move> Rules::legalMoves(Board &board, Color side) const
{
    std::vector<Move> moves;

    // get caslting move
    std::vector<Move> castling_move = castlingMoves(board, side);
    moves.reserve(moves.size() + castling_move.size()); 
    moves.insert(moves.end(), castling_move.begin(), castling_move.end()); 

    //get psuedo move
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            const std::unique_ptr<Piece> &piece = board.at(Position{i, j});
            if (piece != Board::outOfBoundsPtr_ && piece->getColor() == side)
            {
                std::vector<Move> piece_move = piece->pseudoMoves(board, Position{i,j});
                moves.reserve(moves.size() + piece_move.size()); 
                moves.insert(moves.end(), piece_move.begin(), piece_move.end()); 
            }
        }
    }

    std::vector<Move> legal_move;
    for (const Move &move: moves) {
        board.applyTrial(move);
        bool safe = !isInCheck(board, side);
        board.undoTrial();
        if(safe){
            legal_move.push_back(move);
        }
    }
    return legal_move;

}

bool Rules::isInCheck(const Board &board, Color side) const
{
    std::vector<Position> king_pos = board.kingSquares(side);
    if (king_pos.empty())
        return false;
    return isAttacked(board, king_pos.back(), side == Color::White ? Color::White : Color::Black);
}

bool Rules::isAttacked(const Board &board, Position square, Color byColor) const
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            const std::unique_ptr<Piece> &piece = board.at(Position{i, j});
            if (piece != Board::outOfBoundsPtr_ && piece->getColor() == byColor)
            {
                std::vector<Position> pos_attack = piece->attackSquare(board, Position{i, j});
                if (findIndex(pos_attack, square) != -1)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

GameStatus Rules::status(const Board &board, Color toMove, const std::vector<Move> &legal) const
{
    bool inCheck = isInCheck(board, toMove);
    if (inCheck && legal.empty())
    {
        return GameStatus::Checkmate;
    }
    else if (!inCheck && legal.empty())
    {
        return GameStatus::Stalemate;
    }
    else if (inCheck && !legal.empty())
    {
        return GameStatus::Check;
    }
    else
    {
        return GameStatus::Ongoing;
    }
}

bool Rules::givesCheck(Board &board, const Move &move, Color mover) const
{
    board.applyTrial(move);
    bool inCheck = isInCheck(board, mover == Color::White ? Color::Black : Color::White);
    board.undoTrial();
    return inCheck;
}

std::vector<std::string> Rules::validateSetup(const Board &board) const
{
    std::vector<std::string> message;
    std::vector<Position> white_king_square = board.kingSquares(Color::White);
    std::vector<Position> black_king_square = board.kingSquares(Color::Black);
    if (white_king_square.size() != 1)
    {
        std::string white_message = "White needs exactly one king.";
        message.push_back(white_message);
    }
    if (black_king_square.size() != 1)
    {
        std::string black_message = "Black needs exactly one king.";
        message.push_back(black_message);
    }

    for (int column = 0; column < 8; ++column)
    {
        Position first_row{0, column};
        Position last_row{7, column};
        if (board.at(first_row) != Board::outOfBoundsPtr_ && board.at(last_row) != Board::outOfBoundsPtr_
            && (board.at(first_row)->getType() == PieceType::Pawn || board.at(last_row)->getType() == PieceType::Pawn))
        {
            message.push_back("Pawns can't be on the first or last row.");
            break;
        }
    }

    if (isInCheck(board, Color::White))
    {
        message.push_back("White's king is in check.");
    }

    if (isInCheck(board, Color::Black))
    {
        message.push_back("Black's king is in check.");
    }

    return message;
}


bool Rules::castlinghelper(Position rook_square_king, const Board &board, Color opponent, std::vector<Position> empty_squares, std::vector<Position> king_crossed_squares) const{
    if(board.at(rook_square_king) == Board::outOfBoundsPtr_) return false;
    if(board.at(rook_square_king)->getType() != PieceType::Rook 
        || board.at(rook_square_king)->getColor() == opponent 
        || board.at(rook_square_king)->hasMoved()) return false;
    for (const auto& square : empty_squares) {
        if(board.at(square) != Board::outOfBoundsPtr_) return false;
    }
    for (const auto& square : king_crossed_squares) {
        if(isAttacked(board, square, opponent)) return false; 
    }
    return true;
}

std::vector<Move> Rules::castlingMoves(const Board &board, Color color) const
{
    std::vector<Move> moves;
    Color opponent = color == Color::White ? Color::Black : Color::White;
    int row = color == Color::White ? 0 : 7;
    Position king_square = {row, 4};

    const std::unique_ptr<Piece> &king = board.at(king_square);
    if (king == Board::outOfBoundsPtr_ || king->getType() != PieceType::King
        || king->getColor() != color || king->hasMoved() || isInCheck(board, color)) {
        return moves;
    }
    // kingside
    if (castlinghelper({row, 7}, board, opponent,
                       {{row, 5}, {row, 6}}, {{row, 5}, {row, 6}})) {
        moves.push_back(Move{king_square, {row, 6}, MoveKind::Castle, {}});
    }
    // queenside
    if (castlinghelper({row, 0}, board, opponent,
                       {{row, 1}, {row, 2}, {row, 3}}, {{row, 2}, {row, 3}})) {
        moves.push_back(Move{king_square, {row, 2}, MoveKind::Castle, {}});
    }
    return moves;
}
