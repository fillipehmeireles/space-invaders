#include "raylib.h"
#include "spaceinvaders.h"

int main(void)
{

    GameScreen game_screen = (GameScreen){.width=800,.height=450};

    start(game_screen);
    return 0;
}
