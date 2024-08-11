#include "SDL2/SDL.h"
#include <stdio.h>
#include "SDL_image.h"
#include "Player.hpp"
#include "Car.hpp"


using namespace std;

class Game
{

public:
    Game();
    ~Game();

    void init(const char* title, int xPos, int yPos, int width, int height, bool fullscreen, bool twoPlayerMode);
    
    void handleEvents();
    void update();
    void render();
    void clean();

    void HandleInputForPlayer2(const Uint8 *state);

    bool running(){ return isRunning; }

private:

    int WinW;
    int WinH;

    float getDeltaTime();
    
    bool isRunning;
    SDL_Window* window;
    SDL_Renderer* renderer;
    
    PlayerControls playerControls[2];
    Car* players[2];
    bool twoPlayerMode;


    SDL_Rect backgroundSpriteRect; 
    SDL_Texture* backgroundTexture;
    

    Uint32 lastFrameTime;
};

