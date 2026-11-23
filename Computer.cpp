#include "Computer.h"

bool Computer::isHuman() const
{
    return false;
}

Computer::Computer(const Rules &rules) : rules_(rules)
{
}