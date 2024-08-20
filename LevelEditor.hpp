#ifndef LEVELEDITOR_HPP
#define LEVELEDITOR_HPP

#pragma once

#include <vector>
#include <string>
#include "TileSet.hpp"
#include "Utilities.h"
#include "SDL2/SDL.h"
#include "SDL_image.h"
#include "Button.hpp"

using namespace std;

class LevelEditor {
public:
    LevelEditor(int gridWidth, int gridHeight);
    ~LevelEditor();

    void handleEvents();
    void update();
    void updateCamera(); // Todo: implement to move map around
    void render();
    void clean();

    void init(const char* title, int xPos, int yPos, int width, int height, bool fullscreen);
    bool initSDL(const char* title, int xPos, int yPos, int width, int height, bool fullscreen);
    void initButtons();

    void PlaceTile(int x, int y, int tileID);
    void RotateTile(int x, int y);
    void SaveLevel(const string& filename) const;
    void LoadLevel(const string& filename);

    void DrawMap(SDL_Renderer* renderer) const;
    void DrawGrid(SDL_Renderer* renderer) const;
    void HandleLefttMouseClick(int x, int y);
    bool running() { return isRunning; }

    void DrawAvailableTiles(SDL_Renderer* renderer) const;
    void MarkSelectedRect(SDL_Rect selectedRect, int thickness, SDL_Renderer *renderer, int tileId) const;
    void MarkSelectedRect(SDL_Rect selectedRect, int thickness, SDL_Renderer *renderer) const;
    SDL_Rect GetAvailableTileRect(int tileIndex) const;

    bool isInsideGrid(int x, int y) const;

private:

    int selectedTileID = 0; // 0 = nothing selected

    bool isRunning;
    SDL_Window* window;
    SDL_Renderer* renderer;

    int WinW, WinH;
    int gridWidth, gridHeight;
    int gridShiftX, gridShiftY;
    int tileSize;
    vector<vector<TileData>> grid; // Grid storing tile IDs
    TileSet* tileSet; // Reference to the TileSet
    Uint32 lastFrameTime;

    vector<Button> buttons;

};

#endif // LEVELEDITOR_HPP
