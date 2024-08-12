#include "LevelEditor.hpp"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

LevelEditor::LevelEditor()
{
    gridWidth = 10;
    gridHeight = 10;
    tileSize = 64;

    // Initialize grid with empty tiles (e.g., ID 0 represents no tile)
    grid.resize(gridHeight, std::vector<int>(gridWidth, 0));
}
LevelEditor::~LevelEditor()
{
    clean();
}


void LevelEditor::handleEvents()
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

            // Todo: add scaling of elements here
            
            WinW = newWidth;
            WinH = newHeight;
        }
        break;
    default:
        break;
    }

    // Get player input from keyboard
    const Uint8* state = SDL_GetKeyboardState(NULL);
}

void LevelEditor::update()
{
    // Todo: Update camera
}

void LevelEditor::render()
{
    // Clear current render buffer
    SDL_RenderClear(renderer);

    // Add to renderer here
    

    // Draw map and tiles
    DrawMap(renderer);
    

    // Render to screen
    SDL_RenderPresent(renderer);
}

void LevelEditor::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    cout << "Level editor cleaned" << endl;
}

void LevelEditor::PlaceTile(int x, int y, int tileID) {
    if (x >= 0 && x < gridWidth && y >= 0 && y < gridHeight) {
        grid[y][x] = tileID; // Set the tile ID at the specified grid position
    }
}

void LevelEditor::SaveLevel(const string& filename) const {
    ofstream outFile(filename);
    if (!outFile) {
        cerr << "Failed to open file for saving: " << filename << endl;
        return;
    }

    outFile << gridWidth << " " << gridHeight << endl;
    for (const auto& row : grid) {
        for (int id : row) {
            outFile << id << " ";
        }
        outFile << endl;
    }

    outFile.close();
}

void LevelEditor::init(const char *title, int xPos, int yPos, int width, int height, bool fullscreen)
{
    WinW = width;
    WinH = height;

    if (!initSDL(title, xPos, yPos, width, height, fullscreen))
    {
        isRunning = false;
        return;
    }


    isRunning = true;
    lastFrameTime = SDL_GetTicks();


    // Midlertidig: Opprett en testtile
    // ------------------------------------------------------------
    
    tileSet = new TileSet(renderer);

    tileSet->AddTile(1, "assets/TestTile.png", /* hitboxes. Tomt nå */ {});

    PlaceTile(0, 0, 1);
}

bool LevelEditor::initSDL(const char* title, int xPos, int yPos, int width, int height, bool fullscreen)
{
    int flags = fullscreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_RESIZABLE;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        cout << "SDL init failed: " << SDL_GetError() << endl;
        return false;
    }

    cout << "SDL init success" << endl;

    window = SDL_CreateWindow(title, xPos, yPos, width, height, flags);
    if (!window)
    {
        cout << "Window creation failed: " << SDL_GetError() << endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
    {
        cout << "Renderer creation failed: " << SDL_GetError() << endl;
        return false;
    }

    // Draw a white background
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    return true;
}



void LevelEditor::LoadLevel(const string& filename) {
    ifstream inFile(filename);
    if (!inFile) {
        cerr << "Failed to open file for loading: " << filename << endl;
        return;
    }

    inFile >> gridWidth >> gridHeight;
    grid.resize(gridHeight, vector<int>(gridWidth, 0));

    for (auto& row : grid) {
        for (int& id : row) {
            inFile >> id;
        }
    }

    inFile.close();
}

void LevelEditor::DrawMap(SDL_Renderer* renderer) const {
    for (int y = 0; y < gridHeight; ++y) {
        for (int x = 0; x < gridWidth; ++x) {
            int tileID = grid[y][x];
            Tile* tile = tileSet->GetTile(tileID);
            if (tile != nullptr) {
                tile->SetGridPosition(x, y);
                tile->Render(renderer, tileSize);
            }
        }
    }
}
