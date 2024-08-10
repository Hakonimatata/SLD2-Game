#include "Game.h"
#include <iostream>

using namespace std;

Game::Game() : isRunning(false), window(nullptr), renderer(nullptr), player(nullptr), lastFrameTime(0) 
{

    backgroundSpriteRect.h = 600;
    backgroundSpriteRect.w = 800;
    backgroundSpriteRect.x = 0;
    backgroundSpriteRect.y = 0;

    
}

Game::~Game()
{
    clean();
}

void Game::init(const char* title, int xPos, int yPos, int width, int height, bool fullscreen)
{
    int flags = SDL_WINDOW_RESIZABLE;
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

        // Init background texture
        SDL_Surface* tempSurface = IMG_Load("assets/RaceTrack.png");
        if(tempSurface == NULL) {cout << "Failed to load surface: " << SDL_GetError() << endl;}
        backgroundTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_FreeSurface(tempSurface);
        if(backgroundTexture == NULL) {cout << "Failed to load texture: " << SDL_GetError() << endl;}



        // Initialize player
        // player = new Player(renderer, 100.0f, 100.0f, 10.0f); // Startposisjon (100, 100) og hastighet 200 piksler/sekund
        player = new Car(renderer, 100.0f, 100.0f, 10.0f);

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

    case SDL_WINDOWEVENT:
        if (event.window.event == SDL_WINDOWEVENT_RESIZED)
        {
            int newWidth = event.window.data1;
            int newHeight = event.window.data2;

            // update variables
            WinW = newWidth;
            WinH = newHeight;
            backgroundSpriteRect.h = newHeight;
            backgroundSpriteRect.w = newWidth;
        }
        break;
    default:
        break;
    }

    // Get player input from keyboard
    const Uint8* state = SDL_GetKeyboardState(NULL);
    player->HandleInput(state);
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
    
    // Render background
    SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundSpriteRect);

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
