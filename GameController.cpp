#include "GameController.h"
#include <iostream>
#include <string>

using namespace std;

GameController::GameController() {
}

void GameController::run() {
    cout << "hello world" << endl;

    while (1) {
        string input;
        cout << "enter command" << endl;
        cin >> input;
        Command command = parsers.parse(input);
    }
}
