#include "SDL2/SDL.h"
#include <stdio.h>
#include "SDL_image.h"
#include "Player.hpp"
#include "Car.hpp"
#include <string>


using namespace std;

class Game
{

public:
    Game();
    ~Game();

    void init(const char* title, int xPos, int yPos, int width, int height, bool fullscreen, bool twoPlayerMode);
    
    void handleEvents();
    void update();
    void updateCamera();
    void render();
    void clean();

    bool running(){ return isRunning; }

    void resizeElements(int newWidth, int newHeight);

private:

    bool initSDL(const char* title, int xPos, int yPos, int width, int height, bool fullscreen);
    bool loadBackgroundTexture(const std::string& filepath);
    void initPlayers();
    void initPlayerControls();

    // Original width and height
    int WinW;
    int WinH;

    const float carScaleFactor = 0.6f;

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

