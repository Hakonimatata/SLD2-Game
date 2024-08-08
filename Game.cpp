#include "Game.h"
#include <iostream>

using namespace std;


Game::Game()
{ 

}

Game::~Game()
{
    clean();
}

void Game::init(const char* title, int xPos, int yPos, int width, int height, bool fullscreen)
{

    int flags = 0;
    if(fullscreen)
    {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    // Initialize SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        cout << "SDL init success" << endl;

        window = SDL_CreateWindow(title, xPos, yPos, width, height, flags);

        if (!window)
        {
            cout << "Window creation failed: " << SDL_GetError() << endl;
            isRunning = false;
        }

        renderer = SDL_CreateRenderer(window, -1, 0);
        
        if (!renderer)
        {
            cout << "Renderer creation failed: " << SDL_GetError() << endl;
            isRunning = false;
        }
        
        // Draw a white canvas
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); 

        isRunning = true;
    }
    else
    {
        isRunning = false;
    }
}

void Game::handleEvents()
{
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type)
    {
    case SDL_QUIT:
        isRunning = false;
        break;
    default:
        break;
    }
}

void Game::update()
{

}


void Game::render()
{
    // Clear current render buffer
    SDL_RenderClear(renderer);

    // Add to renderer here

    // Render to screen
    SDL_RenderPresent(renderer);

}

void Game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    cout << "Game cleaned" << endl;
}