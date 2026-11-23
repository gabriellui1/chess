#include "GameController.h"
#include "TextView.h"

int main()
{
    std::shared_ptr<TextView> view = std::make_shared<TextView>();
    GameController game(view);
    game.run();
    return 0;
}
