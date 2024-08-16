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
        void loadLevel(const std::string& filename);

    private:
        // Ajustable parameters
        const float carScaleFactor = 0.4f;  // Currently used to set the scale of everything for the car (into the construcotr)
        float tileSize = 64;                // Tilesize <=> map size

        // Functions
        void resizeElements(int newWidth, int newHeight);
        bool initSDL(const char* title, int xPos, int yPos, int width, int height, bool fullscreen);
        bool loadBackgroundTexture(const std::string& filepath);
        void initPlayers();
        void initPlayerControls();
        void DrawMap(SDL_Renderer* renderer) const;
        float getDeltaTime();

        // Original width and height
        int WinW, WinH;

        // Map Grid
        int gridWidth, gridHeight;
        float gridShiftX, gridShiftY;
        vector<vector<TileData>> grid; // Grid storing tile IDs

        // Tile data
        TileSet* tileSet;

        
        bool isRunning;
        SDL_Window* window;
        SDL_Renderer* renderer;
        PlayerControls playerControls[2];
        Car* players[2];
        bool twoPlayerMode;
        Uint32 lastFrameTime;

        SDL_Rect backgroundSpriteRect; 
        SDL_Texture* backgroundTexture;
        
};

