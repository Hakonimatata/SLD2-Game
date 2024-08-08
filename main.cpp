#include <iostream>
#include <SDL2/SDL.h>
#include "Game.h"

using namespace std;

const int WIDTH = 800, HEIGHT = 600;


int main(int argc, char* argv[])
{

    // Create instance of Game
    Game* game = nullptr;
    game = new Game();
    
    // Initialize SDL
    game->init("Title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, false);

    while (game->running())
    {
        game->handleEvents();
        game->update();
        game->render();
    }

    return EXIT_SUCCESS;
}