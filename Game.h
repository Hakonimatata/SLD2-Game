#pragma once

#include "SDL2/SDL.h"
#include <stdio.h>
#include "SDL_image.h"
#include "Player.hpp"
#include "Car.hpp"
#include <string>
#include "TileSet.hpp"


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
        void loadLevel(const std::string& filename);

    private:

        bool initSDL(const char* title, int xPos, int yPos, int width, int height, bool fullscreen);
        bool loadBackgroundTexture(const std::string& filepath);
        void initPlayers();
        void initPlayerControls();
        void DrawMap(SDL_Renderer* renderer) const;

        // Original width and height
        int WinW, WinH;

        // Grid
        int gridWidth, gridHeight;
        float gridShiftX, gridShiftY;
        vector<vector<TileData>> grid; // Grid storing tile IDs


        TileSet* tileSet;
        float tileSize;



        const float carScaleFactor = 0.4f;

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

