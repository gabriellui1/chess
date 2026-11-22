#include "GameController.h"
#include "TextView.h"

int main() {
    TextView view;
    GameController game(view);
    game.run();
    return 0;
}
