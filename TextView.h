#ifndef TEXTVIEW_H
#define TEXTVIEW_H

#include "BoardObserver.h"
#include "Types.h"
#include <iostream>
#include <string>


class TextView: public BoardObserver{
private:
    std::ostream& out_;
    void drawBoard();
public:
    void onBoardChanged(const Board&);
    void showMessage(const std::string& message);
    void showError(const std::string& error);
    void showStartMenu();
    void showLevelPrompt(Color side);
};



#endif