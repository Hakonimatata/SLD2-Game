#include <iostream>
#include <SDL2/SDL.h>
#include "Game.h"
#include "SDL_image.h"
#include "LevelEditor.hpp"

using namespace std;

const int WIDTH = 800, HEIGHT = 500;


int main(int argc, char* argv[])
{
    // Todo: Add launcher for editor/game

    LevelEditor* levelEditor = new LevelEditor(15, 15);

    levelEditor->init("Level Editor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, false);

    while(levelEditor->running())
    {
        levelEditor->handleEvents();
        levelEditor->update();
        levelEditor->render();
    }

    levelEditor->clean();


    // Create instance of Game
    Game* game = new Game();
    
    // Initialize Game
    bool twoPlayerMode = false;
    game->init("Title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, false, twoPlayerMode);

    while (game->running())
    {
        game->handleEvents();
        game->update();
        game->render();
    }

    game->clean();

    return EXIT_SUCCESS;
}