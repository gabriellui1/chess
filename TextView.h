//
#ifndef TEXTVIEW_H
#define TEXTVIEW_H

#include <iostream>
#include <string>
#include "BoardObserver.h"
#include "Types.h"

class TextView : public BoardObserver
{
private:
    std::ostream &out_;
    std::vector<std::string> messages_;
    void drawBoard(const Board &board);

public:
    explicit TextView(std::ostream &out = std::cout);
    void onBoardChanged(const Board &board);
    void show(const std::string &message);
    void show(const std::vector<std::string> &messages);
    void clearMessages();
    void showMessages();
    void pushMessages(const std::string message);
    void showStartMenu();
    void showSetupHelp();
    void showGameHelp();
    // void showLevelPrompt(Color side); this functionality is already implemented in commandParser.cpp
};

#endif
