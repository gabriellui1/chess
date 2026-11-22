#include "Rules.h"
#include "Board.h"

std::vector<Move> Rules::legalMoves(Board &board, Color side) const
{
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
            if (piece && piece->getColor() == byColor)
            {
                std::vector<Position> pos_attack = piece->attackSquare(board, Position{i, j});
                if (std::find(pos_attack.begin(), pos_attack.end(), square) != pos_attack.end())
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
    bool inCheck = isInCheck(board, mover == Color::White ? Color::White : Color::Black);
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
        if (board.at(first_row)->getType() == PieceType::Pawn || board.at(last_row)->getType() == PieceType::Pawn)
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

std::vector<Move> Rules::castlingMoves(const Board &board, Color color) const
{
}
