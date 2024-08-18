#include "Game.h"
#include <iostream>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

Game::Game() : isRunning(false), window(nullptr), renderer(nullptr), lastFrameTime(0) {}

Game::~Game()
{
    clean();
}

void Game::init(const char* title, int xPos, int yPos, int width, int height, bool fullscreen, bool twoPlayerMode)
{
    // Set current window size
    WinW = width;
    WinH = height;

    // Set 1 or 2 plyer mode
    this->twoPlayerMode = twoPlayerMode;
    
    // Todo: Set these based on start position?
    gridShiftX = 0; 
    gridShiftY = 0;

    // Initialize SDL
    if (!initSDL(title, xPos, yPos, width, height, fullscreen))
    {
        isRunning = false;
        return;
    }
    // Initialize the players
    initPlayers();

    // Set player controlls
    initPlayerControls();

    // Get the tileset with textures for the tiles
    tileSet = new TileSet(renderer);

    // Load the level
    loadLevel("Levels/level.txt");
    

    // Everything initialised correctly. Running.
    isRunning = true;
    lastFrameTime = SDL_GetTicks();

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
    // renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        cout << "Renderer creation failed: " << SDL_GetError() << endl;
        return false;
    }

    // Draw a white background
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    return true;
}

void Game::initPlayers()
{
    players[0] = new Car(renderer, 100.0f, 100.0f, 8.0f, "assets/RaceCar.png", carScaleFactor);

    if (twoPlayerMode)
    {
        players[1] = new Car(renderer, 100.0f, 200.0f, 8.0f, "assets/RaceCar2.png", carScaleFactor);
    }
}

void Game::initPlayerControls()
{
    playerControls[0] = 
    {
        SDL_SCANCODE_UP,      // Accelerate
        SDL_SCANCODE_DOWN,    // Reverse
        SDL_SCANCODE_LEFT,    // Turn left
        SDL_SCANCODE_RIGHT,   // Turn right
        SDL_SCANCODE_RSHIFT,  // Drift
        SDL_SCANCODE_PERIOD,  // Boost
    };

    if (twoPlayerMode)
    {
        playerControls[1] = 
        {
            SDL_SCANCODE_W,       // Accelerate
            SDL_SCANCODE_S,       // Reverse
            SDL_SCANCODE_A,       // Turn left
            SDL_SCANCODE_D,       // Turn right
            SDL_SCANCODE_LCTRL,   // Drift
            SDL_SCANCODE_LSHIFT,  // Boost
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
            // Get new window size
            int newWidth = event.window.data1;
            int newHeight = event.window.data2;

            // Resize elements before setting new windowsize.
            // This is to get relative changes from previous window size.
            resizeElements(newWidth, newHeight);
            
            // Set new window size
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
    // Get current delta time. This is used for smooth movement, not based on frame rate
    float deltaTime = getDeltaTime();

    // Update players
    players[0]->Update(deltaTime);
    if(twoPlayerMode) {players[1]->Update(deltaTime);}

    // Update camera (position of cars andmap)
    updateCamera();
}

void Game::updateCamera()
{
    // Window mid-points
    float centerX = WinW * 0.5;
    float centerY = WinH * 0.5;
    
    // Get the car's current position
    FloatPoint carPos = players[0]->GetPos();
    float distance = 0.0f;
    FloatPoint midPoint = {0.0f, 0.0f};

    float targetShiftX, targetShiftY;

    if(twoPlayerMode) // Keep camera in between cars
    {
        FloatPoint car2Pos = players[1]->GetPos();
        distance = getDistanceBetween(carPos, car2Pos);
        midPoint = getMidPointBetween(carPos, car2Pos);

        targetShiftX = centerX - midPoint.x;
        targetShiftY = centerY - midPoint.y;
    }
    else // Desired grid shift to keep the car centered on the screen
    {
        targetShiftX = centerX - carPos.x;
        targetShiftY = centerY - carPos.y;
    }

    // Lerp factor (controls the smoothness of the camera movement)
    float lerpFactor = 0.1; // Adjust this value to control the speed of the camera adjustment [0, 1]

    // Add the same amount of movement to the map, as applied to the car under
    gridShiftX += targetShiftX * lerpFactor;
    gridShiftY += targetShiftY * lerpFactor;

    // Smoothly adjust the car towards the target shift
    carPos.x += targetShiftX * lerpFactor;
    carPos.y += targetShiftY * lerpFactor;
    players[0]->SetPosition(carPos.x, carPos.y);

    if(twoPlayerMode) // Update car 2 as well
    {
        FloatPoint car2Pos = players[1]->GetPos();
        car2Pos.x += targetShiftX * lerpFactor;
        car2Pos.y += targetShiftY * lerpFactor;
        players[1]->SetPosition(car2Pos.x, car2Pos.y);
    }

    // Keep two players in screen! // Todo: implement
}

void Game::render()
{
    // Clear current render buffer first
    SDL_RenderClear(renderer);

    // ------------Start Adding Render Functions Here------------
    
    // Black background
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, NULL);

    // Render map
    DrawMap(renderer);

    // Render player
    players[0]->Render(renderer);
    if(twoPlayerMode) {players[1]->Render(renderer);}
    
    // ------------End Adding Render Functions Here------------

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

    // Resize map (tilesize)
    tileSize *= scaleFactor;
    gridShiftX *= scaleFactor;
    gridShiftY *= scaleFactor;
    

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

//  Get delta time in seconds
float Game::getDeltaTime()
{
    Uint32 currentTime = SDL_GetTicks();
    float deltaTime = (currentTime - lastFrameTime) / 1000.0f; // in seconds 
    lastFrameTime = currentTime;

    return deltaTime;
}
