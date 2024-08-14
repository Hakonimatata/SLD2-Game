#include "Game.h"
#include <iostream>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

Game::Game() : isRunning(false), window(nullptr), renderer(nullptr), lastFrameTime(0) 
{
       
}

Game::~Game()
{
    clean();
}

void Game::init(const char* title, int xPos, int yPos, int width, int height, bool fullscreen, bool twoPlayerMode)
{
    WinW = width;
    WinH = height;

    gridShiftX = 0;
    gridShiftY = 0;

    // backgroundSpriteRect.h = height;
    // backgroundSpriteRect.w = width;
    // backgroundSpriteRect.x = 0;
    // backgroundSpriteRect.y = 0;
    // if (!loadBackgroundTexture("assets/RaceTrack.png"))
    // {
    //     isRunning = false;
    //     return;
    // }

    this->twoPlayerMode = twoPlayerMode;

    if (!initSDL(title, xPos, yPos, width, height, fullscreen))
    {
        isRunning = false;
        return;
    }

    initPlayers();
    initPlayerControls();

    isRunning = true;
    lastFrameTime = SDL_GetTicks();

    // initialize the set of tiles
    tileSet = new TileSet(renderer);

    // Load the level
    loadLevel("level.txt");

}

bool Game::initSDL(const char* title, int xPos, int yPos, int width, int height, bool fullscreen)
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

bool Game::loadBackgroundTexture(const std::string& filepath)
{
    SDL_Surface* tempSurface = IMG_Load(filepath.c_str());
    if (!tempSurface)
    {
        cout << "Failed to load surface: " << SDL_GetError() << endl;
        return false;
    }

    backgroundTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    if (!backgroundTexture)
    {
        cout << "Failed to load texture: " << SDL_GetError() << endl;
        return false;
    }

    return true;
}

void Game::initPlayers()
{
    players[0] = new Car(renderer, 100.0f, 100.0f, 7.0f, "assets/RaceCar.png", carScaleFactor);

    if (twoPlayerMode)
    {
        players[1] = new Car(renderer, 100.0f, 200.0f, 7.0f, "assets/RaceCar2.png", carScaleFactor);
    }
}

void Game::initPlayerControls()
{
    playerControls[0] = 
    {
        SDL_SCANCODE_UP,     // Accelerate
        SDL_SCANCODE_DOWN,   // Reverse
        SDL_SCANCODE_LEFT,   // Turn left
        SDL_SCANCODE_RIGHT,  // Turn right
        SDL_SCANCODE_RCTRL,  // Drift
        SDL_SCANCODE_PERIOD  // Boost
    };

    if (twoPlayerMode)
    {
        playerControls[1] = 
        {
            SDL_SCANCODE_W,      // Accelerate
            SDL_SCANCODE_S,      // Reverse
            SDL_SCANCODE_A,      // Turn left
            SDL_SCANCODE_D,      // Turn right
            SDL_SCANCODE_LCTRL,  // Drift
            SDL_SCANCODE_LSHIFT  // Boost
        };
    }
}

void Game::DrawMap(SDL_Renderer *renderer) const
{
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

void Game::handleEvents()
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

            resizeElements(newWidth, newHeight);
            
            WinW = newWidth;
            WinH = newHeight;
        }
        break;
    default:
        break;
    }

    // Get player input from keyboard
    const Uint8* state = SDL_GetKeyboardState(NULL);

    // Handle input for all players
    for(int i = 0; i < (twoPlayerMode ? 2 : 1); ++i) {
        players[i]->HandleInput(state, playerControls[i]);
    }
}

void Game::update()
{
    // Get current delta time. This is used for smooth movement
    float deltaTime = getDeltaTime();

    // Update player
    players[0]->Update(deltaTime);
    if(twoPlayerMode) {players[1]->Update(deltaTime);}

    updateCamera();
}

// Todo: implement update camera
void Game::updateCamera()
{
    // Update camera
}



void Game::render()
{
    // Clear current render buffer
    SDL_RenderClear(renderer);

    // Add to renderer here
    
    // Render background
    // SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundSpriteRect);


    // Render map
    DrawMap(renderer);

    // Render player
    players[0]->Render(renderer);
    if(twoPlayerMode) {players[1]->Render(renderer);}
    

    // Render to screen
    SDL_RenderPresent(renderer);

}

void Game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    cout << "Game cleaned" << endl;
}

void Game::resizeElements(int newWidth, int newHeight)
{
    // Beregn skaleringsfaktoren basert på den nye bredden
    float scaleFactor = static_cast<float>(newWidth) / static_cast<float>(WinW);

    // Resize background
    backgroundSpriteRect.w = newWidth;
    backgroundSpriteRect.h = static_cast<int>(WinH * scaleFactor);  // Juster høyden basert på skaleringsfaktoren

    // Skaler alle spillere
    for(int i = 0; i < (twoPlayerMode ? 2 : 1); ++i) {
        players[i]->ScaleEverything(scaleFactor);
    }
}

void Game::loadLevel(const std::string &filename)
{
    std::ifstream ifs(filename);
    if (!ifs) {
        std::cerr << "Failed to open file for loading: " << filename << std::endl;
        return;
    }

    // Add data: gridWidth, gridHeight
    ifs >> gridWidth >> gridHeight;

    // Recalculate the tile size based on the loaded grid dimensions
    tileSize = min((WinH - 2* gridShiftY) / gridHeight, (WinW - 2* gridShiftX) / gridWidth);
    tileSize*= 3; // temporary

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

// Helper function to calculate delta time
float Game::getDeltaTime()
{
    Uint32 currentTime = SDL_GetTicks();
    float deltaTime = (currentTime - lastFrameTime) / 1000.0f; // Konverter fra millisekunder til sekunder
    lastFrameTime = currentTime;

    return deltaTime;
}
