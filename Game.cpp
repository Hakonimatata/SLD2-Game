#include "Game.h"
#include <iostream>

using namespace std;

Game::Game() : isRunning(false), window(nullptr), renderer(nullptr), player(nullptr), lastFrameTime(0) {}

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

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (!renderer)
        {
            cout << "Renderer creation failed: " << SDL_GetError() << endl;
            isRunning = false;
        }
        
        // Draw a white background
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); 

        // Initialize player
        player = new Player(100.0f, 100.0f, 200.0f); // Startposisjon (100, 100) og hastighet 200 piksler/sekund

        isRunning = true;
        lastFrameTime = SDL_GetTicks();
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
    // Get current delta time. This is used for smooth movement
    float deltaTime = getDeltaTime();

    // Oppdater player
    player->Update(deltaTime);

}



void Game::render()
{
    // Clear current render buffer
    SDL_RenderClear(renderer);

    // Add to renderer here
    
    // Render player
    player->Render(renderer);

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






// Helper function to calculate delta time
float Game::getDeltaTime()
{
    Uint32 currentTime = SDL_GetTicks();
    float deltaTime = (currentTime - lastFrameTime) / 1000.0f; // Konverter fra millisekunder til sekunder
    lastFrameTime = currentTime;

    return deltaTime;
}
