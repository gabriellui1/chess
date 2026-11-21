#include "Piece.h"
#include "Board.h"

std::vector<Position> Piece::slide(const Board& board, const std::vector<Offset>& directions, Position from, int maxSteps) const{
    std::vector<Position> squares; 
    for (const Offset& dir : directions){ // read only
        Position cur = from;

        for(int i = 0; i < maxSteps; ++i){
            cur = Position{cur.row + dir.drow, cur.col + dir.dcol};
            if(!isOnBoard(cur)){
                break; // can't step in that direction anymore
            }

            squares.push_back(cur); // include the square that possibly can have a piece there.
            if(board.at(cur) != nullptr){
                break; // there is a piece so break
            }

        }
    return squares;
    }
}


std::vector<Position> Piece::jump(const Board& board, Position from, const std::vector<Offset>& offsets) const{
    std::vector<Position> squares; 
    for (const Offset& offset : offsets){ // read only
        Position cur = Position{from.row + offset.drow, from.col + offset.dcol};
        if (isOnBoard(cur)) {
            squares.push_back(cur);
        }
    }
    return squares;
}
