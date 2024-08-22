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

    int mouseX = 0; // Declare outside the switch-case
    int mouseY = 0;

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

    case SDL_MOUSEMOTION: // Catch mouse movement
        mouseX = event.motion.x;
        mouseY = event.motion.y;

        // Check if the mouse is hovering over the grid
        if (isInsideGrid(mouseX, mouseY)) {
            hoverTileX = (mouseX - gridShiftX) / tileSize;
            hoverTileY = (mouseY - gridShiftY) / tileSize;
        } else {
            hoverTileX = -1;
            hoverTileY = -1;
        }
        break;
    
    case SDL_MOUSEBUTTONDOWN:
        mouseX = event.button.x;
        mouseY = event.button.y;
        
        if (event.button.button == SDL_BUTTON_LEFT){
            HandleLefttMouseClick(mouseX, mouseY);
        }
        if (event.button.button == SDL_BUTTON_RIGHT){
            RotateTile(mouseX, mouseY);
        }
        break;

    default:
        break;
    }

    // Get player input from keyboard
    const Uint8* state = SDL_GetKeyboardState(NULL);

    MooveCamera(state);

    // Todo: implement buttons
    // Temp save to file
    if (state[SDL_SCANCODE_S]) {
        SaveLevel("Level/level.txt");
    }

}

void LevelEditor::MooveCamera(const Uint8* state)
{
    // Move camera
    if(state[SDL_SCANCODE_LEFT]) 
    {
        gridShiftX += 10;
    }
    else if(state[SDL_SCANCODE_RIGHT])
    {
        gridShiftX -= 10;
    }
    if(state[SDL_SCANCODE_UP])
    {
        gridShiftY += 10;
    }
    else if(state[SDL_SCANCODE_DOWN])
    {
        gridShiftY -= 10;
    }
}


void LevelEditor::update()
{
    // Todo: Update camera?
    updateButtonPositions(); 
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

    // Draw buttons in buttons vector
    for (Button& button : buttons) {renderButton(renderer, button);}    

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

void LevelEditor::PlaceTile(int gridX, int gridY) {

    // Inputs are grid coordinates
    
    // Get tile 
    Tile* tile = tileSet->GetTile(selectedTileData.id);

    if (tile != nullptr) 
    { 
        int tileWidth = tile->GetWidth();
        int tileHeight = tile->GetHeight();

        
        // Check if in bounds (prevent tile to render outside)
        if (gridX + tileWidth < gridWidth || gridY + tileHeight < gridHeight) 
        {
            // The whole tile is within bounds of grid!
            grid[gridY][gridX] = TileData(selectedTileData.id, selectedTileData.rotation);
            
            // Remove tiles surrounding
            // todo: implement
        }   
    }
}



void LevelEditor::RotateTile(int x, int y)
{
    if (selectedTileData.id != 0)
    {
        selectedTileData.rotation = (selectedTileData.rotation + 90) % 360;
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
    
    tileSize = 50; // Sets the size of the tiles in the editor

    if (!initSDL(title, xPos, yPos, width, height, fullscreen))
    {
        isRunning = false;
        return;
    }

    // Initialize the set of tiles
    tileSet = new TileSet(renderer);

    // Initialize buttons
    initButtons();

    // ----------Level editor running successfully---------------
    isRunning = true;
    lastFrameTime = SDL_GetTicks();
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

void LevelEditor::initButtons()
{
    Button cycleTilesButton;
    cycleTilesButton.rect = { WinW - 140, 20, 100, 50 }; // xPos, yPos, width, height
    addCallback(cycleTilesButton, this, &LevelEditor::CycleAvalableTileSize);
    buttons.push_back(cycleTilesButton);
    // ---------------------------------------------------------

    // Add more buttons here

}

void LevelEditor::updateButtonPositions()
{

    if (!buttons.empty()) {
        // Update the position of cycleTilesButton
        SDL_Rect newRect = { WinW - 140, 20, 100, 50 };
        buttons[0].updateRect(newRect);


        // Add updates for more buttons here
    }
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
    for (int y = 0; y < gridHeight; ++y) {
        for (int x = 0; x < gridWidth; ++x) {
            const TileData& tileData = grid[y][x];


            // Tegn flisene som allerede er plassert
            if (tileData.id != 0) {
                Tile* tile = tileSet->GetTile(tileData.id);
                if (tile != nullptr) {

                    int tileWidth = tile->GetWidth();
                    int tileHeight = tile->GetHeight();

                    // Time to draw the tiles!
                    tile->SetGridPosition(x, y);
                    tile->SetRotation(tileData.rotation);
                    tile->Render(renderer, tileSize, gridShiftX, gridShiftY);
                }
            }

            // Tegn den valgte flisen under musen med opasitet
            if (x == hoverTileX && y == hoverTileY && selectedTileData.id != 0) {
                if (selectedTileData.id != 0) {
                    DrawTileWithOpacity(renderer, x, y, 70); // 150 er opasiteten (kan justeres)
                }
            }
        }
    }
}




void LevelEditor::DrawGrid(SDL_Renderer *renderer) const
{
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

void LevelEditor::HandleLefttMouseClick(int x, int y)
{
    // ------ Placing tiles ------

    if (isInsideGrid(x, y)) {
        // Get grid coordinates
        int gridX = (x - gridShiftX) / tileSize;
        int gridY = (y - gridShiftY) / tileSize;

        if (selectedTileData.id != 0) {
            PlaceTile(gridX, gridY);
        }
    }
    else
    {
        // ------ Select tiles ------

        int i = 0;
        for (const auto& pair : tileSet->tiles) {

            Tile* tile = pair.second;

            if (tile == nullptr || !ShouldDrawTile(tile)) continue;

            SDL_Rect tileRect = GetAvailableTileRect(i, tile);

            if (x >= tileRect.x && x < tileRect.x + tileRect.w && y >= tileRect.y && y < tileRect.y + tileRect.h) {
                selectedTileData = TileData(pair.first);
                break;
            }

            i++;
        }

        // ------ Handle button click ------

        handleButtonClick(x, y, buttons);
    }
}


/**
 * Draws all available tiles in the tile set to the given renderer.
 *
 * @param renderer The SDL renderer to draw the tiles to.
 *
 * @return None
 *
 * @throws None
 */
void LevelEditor::DrawAvailableTiles(SDL_Renderer* renderer) const
{
    int i = 0;
    for (const auto& pair : tileSet->tiles)
    {
        Tile* tile = pair.second;

        if (tile == nullptr || !ShouldDrawTile(tile)) continue;

        SDL_Rect rect = GetAvailableTileRect(i, tile);

        SDL_Texture* texture = tile->GetTexture();

        SDL_RenderCopy(renderer, texture, NULL, &rect);

        i++;
    }
}

/**
 * Determines whether the given tile should be drawn based on the current setting of the showTileDimention.
 *
 * @param tile A pointer to the tile object.
 * @return A boolean indicating whether the tile should be drawn.
 */
bool LevelEditor::ShouldDrawTile(const Tile* tile) const
{
    switch (showTileDimention) {
        case TileDimentions::OneByOne:
            return (tile->GetWidth() == 1 && tile->GetHeight() == 1);
        case TileDimentions::OneByTwo:
            return (tile->GetWidth() == 1 && tile->GetHeight() == 2);
        default:
            return false;
    }
}

/**
 * Calculates the position and dimensions of a tile in the available tile list.
 *
 * @param tileIndex The index of the tile in the available tile list.
 * @param tile A pointer to the tile object.
 *
 * @return An SDL_Rect representing the position and dimensions of the tile.
 */
SDL_Rect LevelEditor::GetAvailableTileRect(int tileIndex, const Tile* tile) const
{
    int tileWidth = tile->GetWidth();
    int tileHeight = tile->GetHeight();

    int xShift = 40;
    int yShift = 100;
    int spaceBetweenTiles = 5;

    int maxRows = (WinH - 2 * yShift) / (tileSize * tileHeight + spaceBetweenTiles);
    if (maxRows <= 0) { maxRows = 1; } // prevents crash

    int xPosition = WinW - xShift - (tileSize * tileWidth + spaceBetweenTiles) - (tileSize * tileWidth + spaceBetweenTiles) * (tileIndex / maxRows);
    int yPosition = yShift + (tileIndex % maxRows) * (tileSize * tileHeight + spaceBetweenTiles);

    SDL_Rect rect;
    rect.x = xPosition;
    rect.y = yPosition;
    rect.w = tileSize * tileWidth;
    rect.h = tileSize * tileHeight;

    return rect;
}

void LevelEditor::DrawTileWithOpacity(SDL_Renderer* renderer, int gridX, int gridY, int opacity) const {
    
    Tile* tile = tileSet->GetTile(selectedTileData.id);
    SDL_Texture* texture = tile->GetTexture(); 

    // Sett alpha (gjennomsiktighet)
    SDL_SetTextureAlphaMod(texture, opacity);

    SDL_Rect destRect = GetTileRectFromGrid(tileSize, gridShiftX, gridShiftY, gridX, gridY, selectedTileData.rotation, tile->GetWidth(), tile->GetHeight());
    
    SDL_RenderCopyEx(renderer, texture, nullptr, &destRect, selectedTileData.rotation, nullptr, SDL_FLIP_NONE);

    // Sett alpha tilbake til fullt synlig for å unngå at andre fliser også blir påvirket
    SDL_SetTextureAlphaMod(texture, 255);
}


bool LevelEditor::isInsideGrid(int x, int y) const {
    return x >= gridShiftX && x < gridShiftX + gridWidth * tileSize && 
           y >= gridShiftY && y < gridShiftY + gridHeight * tileSize;
}

void LevelEditor::CycleAvalableTileSize()
{
    switch (showTileDimention)
    {
        case TileDimentions::OneByOne:
            showTileDimention = TileDimentions::OneByTwo;
            break;

        case TileDimentions::OneByTwo:
            showTileDimention = TileDimentions::OneByOne;
            break;

        case TileDimentions::None:
        default:
            showTileDimention = TileDimentions::OneByOne;
            break;
    }
}
