#ifndef COMPUTER_H
#define COMPUTER_H

#include "Player.h"
#include <vector>


class Computer: public Player{ //different levels implement this class


public:
    virtual ~Computer() = default;
    virtual std::vector<Move> choose(const Command&, const Board&, const std::vector<Move>& legal) =0;    
    // more as needed 
};








#endif