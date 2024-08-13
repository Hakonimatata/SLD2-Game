#ifndef LEVELEDITOR_HPP
#define LEVELEDITOR_HPP

#include <vector>
#include <string>
#include "TileSet.hpp"
#include "Utilities.h"
#include "SDL2/SDL.h"
#include "SDL_image.h"

using namespace std;

class LevelEditor {
public:
    LevelEditor();
    ~LevelEditor();

    void handleEvents();
    void update();
    void updateCamera();
    void render();
    void clean();

    void init(const char* title, int xPos, int yPos, int width, int height, bool fullscreen);
    bool initSDL(const char* title, int xPos, int yPos, int width, int height, bool fullscreen);


    void PlaceTile(int x, int y, int tileID);
    void SaveLevel(const string& filename) const;
    void LoadLevel(const string& filename);

    void DrawMap(SDL_Renderer* renderer) const;
    void DrawBackgroundGrid(SDL_Renderer* renderer) const;
    Point GetTopLeftPointFromGridCoords(int x, int y) const;
    void HandleMouseClick(int x, int y);
    bool running() { return isRunning; }
private:
    int currentTileID = 0; // 0 is nothing

    bool isRunning;
    SDL_Window* window;
    SDL_Renderer* renderer;

    int WinW, WinH;
    int gridWidth, gridHeight;
    int gridShiftX, gridShiftY;
    int tileSize;
    vector<vector<int>> grid; // Grid storing tile IDs
    TileSet* tileSet; // Reference to the TileSet
    Uint32 lastFrameTime;
};

#endif // LEVELEDITOR_HPP
