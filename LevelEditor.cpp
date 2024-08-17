#include "LevelEditor.hpp"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

LevelEditor::LevelEditor(int gridWidth, int gridHeight)
{
    this->gridWidth = gridWidth;
    this->gridHeight = gridHeight;

    // Initialize grid with empty tiles
    grid.resize(gridHeight, std::vector<TileData>(gridWidth, TileData()));
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
    
    case SDL_MOUSEBUTTONDOWN:
        
        if (event.button.button == SDL_BUTTON_LEFT){

            HandleLefttMouseClick(event.button.x, event.button.y);

        }
        if (event.button.button == SDL_BUTTON_RIGHT){

            // Todo: change to HandleLeftMouseClick?
            RotateTile(event.button.x, event.button.y);
        }

        break;
    default:
        break;
    }

    // Get player input from keyboard
    const Uint8* state = SDL_GetKeyboardState(NULL);


    // Temp save to file
    if (state[SDL_SCANCODE_S]) {
        SaveLevel("level.txt");
    }
}

void LevelEditor::update()
{
    // Todo: Update camera?
}

void LevelEditor::render()
{
    // Clear current render buffer
    SDL_RenderClear(renderer);

    // Add to renderer here
    DrawGrid(renderer);

    // Draw map and tiles
    DrawMap(renderer);

    // Draw available tiles
    DrawAvailableTiles(renderer);
    

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
        grid[y][x] = TileData(tileID, 0); // Set the tile ID at the specified grid position with rotation 0
    }
}

void LevelEditor::RotateTile(int x, int y)
{
    if(isInsideGrid(x, y)){

        // Find which tile is clicked
        int tileX = (x - gridShiftX) / tileSize;
        int tileY = (y - gridShiftY) / tileSize;

        // Get the tile at that position
        TileData& tileData = grid[tileY][tileX];
        if (tileData.id != 0) {
            tileData.rotation = (tileData.rotation + 90) % 360; // Rotate 90 degrees
        }
    }
}


void LevelEditor::SaveLevel(const std::string& filename) const {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Failed to open file for saving: " << filename << std::endl;
        return;
    }

    outFile << gridWidth << " " << gridHeight << std::endl;
    for (int y = 0; y < gridHeight; ++y) {
        for (int x = 0; x < gridWidth; ++x) {
            const TileData& tileData = grid[y][x];
            outFile << tileData.id << "," << tileData.rotation << " ";
        }
        outFile << std::endl;
    }

    outFile.close();
}



void LevelEditor::init(const char *title, int xPos, int yPos, int width, int height, bool fullscreen)
{
    // Set window size
    WinW = width;
    WinH = height;

    // Set  grid shift based on window size
    gridShiftX = WinW * 0.1;
    gridShiftY = WinH * 0.1;

    // Fit a the grid in the window
    tileSize = min((height - 2* gridShiftY) / gridHeight, (width - 2* gridShiftX) / gridWidth);


    if (!initSDL(title, xPos, yPos, width, height, fullscreen))
    {
        isRunning = false;
        return;
    }


    isRunning = true;
    lastFrameTime = SDL_GetTicks();


    // Initialize the set of tiles
    tileSet = new TileSet(renderer);
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

    // Set background color to white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    return true;
}



void LevelEditor::LoadLevel(const std::string& filename) {
    std::ifstream ifs(filename);
    if (!ifs) {
        std::cerr << "Failed to open file for loading: " << filename << std::endl;
        return;
    }

    // Add data: gridWidth, gridHeight
    ifs >> gridWidth >> gridHeight;

    // Set grid size accordingly
    grid.resize(gridHeight, std::vector<TileData>(gridWidth));

    for (int y = 0; y < gridHeight; ++y) {
        for (int x = 0; x < gridWidth; ++x) {
            std::string idRotationPair;
            ifs >> idRotationPair;

            size_t commaPos = idRotationPair.find(',');
            int id = std::stoi(idRotationPair.substr(0, commaPos));
            int rotation = std::stoi(idRotationPair.substr(commaPos + 1));

            grid[y][x] = TileData(id, rotation);
        }
    }

    ifs.close();
}


void LevelEditor::DrawMap(SDL_Renderer* renderer) const {
    // Check each grid position for tile and draw
    for (int y = 0; y < gridHeight; ++y) {
        for (int x = 0; x < gridWidth; ++x) {
            // Get tile data from grid point
            const TileData& tileData = grid[y][x];
            if (tileData.id != 0) {
                Tile* tile = tileSet->GetTile(tileData.id);
                if (tile != nullptr) {
                    tile->SetGridPosition(x, y);
                    tile->SetRotation(tileData.rotation); // Set rotation for rendering
                    tile->Render(renderer, tileSize, gridShiftX, gridShiftY);
                }
            }
        }
    }
}


void LevelEditor::DrawGrid(SDL_Renderer *renderer) const
{
    // Setter fargen for linjen (RGB: 255, 0, 0, full opasitet)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // Background color
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);

    for(int i = 0; i <= gridWidth; ++i){
        SDL_RenderDrawLine(renderer, gridShiftX + i * tileSize, gridShiftY, gridShiftX + i * tileSize, gridShiftY + gridHeight * tileSize);
    }
    for(int i = 0; i <= gridHeight; ++i){
        SDL_RenderDrawLine(renderer, gridShiftX, gridShiftY + i * tileSize, gridShiftX + gridWidth * tileSize, gridShiftY + i * tileSize);
    }
}

Point LevelEditor::GetTopLeftPointFromGridCoords(int x, int y) const
{
    int GridXPos = gridShiftX + x * tileSize;
    int GridYPos = gridShiftY + y * tileSize;

    return Point{GridXPos, GridYPos};
}

void LevelEditor::HandleLefttMouseClick(int x, int y)
{
    // sjekk om innenfor grid
    if(isInsideGrid(x, y)){
        // Inside grid
        // Find which tile is clicked
        int tileX = (x - gridShiftX) / tileSize;
        int tileY = (y - gridShiftY) / tileSize;

        if(selectedTileID != 0){
            PlaceTile(tileX, tileY, selectedTileID);
        }
    }
    

    // Select tile
    else {
        // Sjekk om klikket er innenfor Available Tiles-området
        int i = 0; // Indeks for tile

        for (const auto& pair : tileSet->tiles) {
            SDL_Rect tileRect = GetAvailableTileRect(i); // Bruker GetAvailableTileRect for å finne posisjon

            // Sjekk om klikket er innenfor denne tile
            if (x >= tileRect.x && x < tileRect.x + tileRect.w &&
                y >= tileRect.y && y < tileRect.y + tileRect.h) {
                // Klikket er innenfor denne tile, så oppdater den valgte tile-ID-en
                selectedTileID = pair.first; // ID-en for den valgte tile
                break; // Avslutt loopen når vi har funnet den valgte tile
            }

            i++; // Gå til neste tile
        }
    }
}

void LevelEditor::DrawAvailableTiles(SDL_Renderer *renderer) const
{
    
    int i = 0; // tile id
    for (const auto& pair : tileSet->tiles)
    {
        Tile* tile = pair.second;

        if (tile != nullptr)
        {
            SDL_Rect destRect = GetAvailableTileRect(i);
            SDL_Texture* texture = tile->GetTexture();

            // Draw to screen
            SDL_RenderCopy(renderer, texture, NULL, &destRect);

            i++; // Next tile
        }
    }
}

SDL_Rect LevelEditor::GetAvailableTileRect(int tileIndex) const // Defines the tile selection grid
{
    int xShift = gridShiftX;
    int yShift  = gridShiftY;

    int spaceBetweenTiles = 5;
    int maxRows = (WinH - 2 * yShift) / (tileSize + spaceBetweenTiles);
    if (maxRows <= 0){maxRows = 1;} // prevents crash

    int xPosition = WinW - xShift - (tileSize + spaceBetweenTiles) - (tileSize + spaceBetweenTiles) * (tileIndex / maxRows);
    int yPosition = yShift + (tileIndex % maxRows) * (tileSize + spaceBetweenTiles); 

    SDL_Rect rect;
    rect.x = xPosition;
    rect.y = yPosition;
    rect.w = tileSize;
    rect.h = tileSize;

    return rect;
}

/// @brief Checks if the given mouse coordinates are inside the grid
/// @param x 
/// @param y 
/// @return 
bool LevelEditor::isInsideGrid(int x, int y) const
{
    return  x >= gridShiftX && x < gridShiftX + gridWidth * tileSize && y >= gridShiftY && y < gridShiftY + gridHeight * tileSize;
}
