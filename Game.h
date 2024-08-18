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
        void render();
        void clean();

        bool running(){ return isRunning; }
        void loadLevel(const std::string& filename);

    private:
        // Ajustable parameters
        const float carScaleFactor = 0.4f;  // Currently used to set the scale of everything for the car (into the construcotr)
        float tileSize = 64;                // Tilesize dictates the map size

        // Functions
        void resizeElements(int newWidth, int newHeight);
        bool initSDL(const char* title, int xPos, int yPos, int width, int height, bool fullscreen);
        bool loadBackgroundTexture(const std::string& filepath);
        void initPlayers();
        void initPlayerControls();
        void DrawMap(SDL_Renderer* renderer) const;
        void updateCamera(float deltaTime);
        float getDeltaTime();

       
        int WinW, WinH;                   // Original width and height
        int gridWidth, gridHeight;        // Map grid size
        float gridShiftX, gridShiftY;
        vector<vector<TileData>> grid;    // Grid storing tile IDs
        TileSet* tileSet;                 // Tile data
        bool isRunning;                   // Is game running
        SDL_Window* window;               // SDL window
        SDL_Renderer* renderer;           // SDL renderer
        PlayerControls playerControls[2]; // Player controlls
        Car* players[2];                  // Players
        bool twoPlayerMode;               // Two player mode
        Uint32 lastFrameTime;             // Last frame time
        
        SDL_Rect backgroundSpriteRect;    // For background if needed
        SDL_Texture* backgroundTexture;   // Background texture
        
};

