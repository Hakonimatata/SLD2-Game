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

    void init(const char* title, int xPos, int yPos, int width, int height, bool fullscreen);
    
    void handleEvents();
    void update();
    void render();
    void clean();

    bool running(){ return isRunning; }

private:

    int WinW;
    int WinH;

    float getDeltaTime();
    
    bool isRunning;
    SDL_Window* window;
    SDL_Renderer* renderer;
   
    Car* player;


    SDL_Rect backgroundSpriteRect; 
    SDL_Texture* backgroundTexture;
    

    Uint32 lastFrameTime;
};