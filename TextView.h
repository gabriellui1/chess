#ifndef TEXTVIEW_H
#define TEXTVIEW_H

#include <iostream>
#include <string>
#include "BoardObserver.h"
#include "Types.h"

class TextView: public BoardObserver{
private:
    std::ostream& out_;
    void drawBoard();
public:
    explicit TextView(std::ostream& out_ = std::cout);
    void onBoardChanged(const Board&);
    void showMessage(const std::string& message);
    void showError(const std::string& error);
    void showStartMenu();
    void showLevelPrompt(Color side);
};

#endif
