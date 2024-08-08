#include <iostream>
#include <SDL2/SDL.h>

using namespace std;

const int WIDTH = 800, HEIGHT = 600;

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* window = 
        SDL_CreateWindow("Title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
            WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);

    if (window == nullptr)
    {
        cout << "Could not create window" << SDL_GetError() << endl; return EXIT_FAILURE;
    }

    SDL_Event windowEvent;

    while (true)
    {
        if (SDL_PollEvent(&windowEvent))
        {
            if (windowEvent.type == SDL_QUIT) break;
        }
    }

    // Cleanup
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}