#include "LevelEditor.hpp"
#include <fstream>
#include <iostream>

LevelEditor::LevelEditor(int gridWidth, int gridHeight, int tileSize, TileSet* tileSet)
    : gridWidth(gridWidth), gridHeight(gridHeight), tileSize(tileSize), tileSet(tileSet) 
{
    // Initialize grid with empty tiles (e.g., ID 0 represents no tile)
    grid.resize(gridHeight, std::vector<int>(gridWidth, 0));
}

void LevelEditor::PlaceTile(int x, int y, int tileID) {
    if (x >= 0 && x < gridWidth && y >= 0 && y < gridHeight) {
        grid[y][x] = tileID; // Set the tile ID at the specified grid position
    }
}

void LevelEditor::SaveLevel(const std::string& filename) const {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Failed to open file for saving: " << filename << std::endl;
        return;
    }

    outFile << gridWidth << " " << gridHeight << std::endl;
    for (const auto& row : grid) {
        for (int id : row) {
            outFile << id << " ";
        }
        outFile << std::endl;
    }

    outFile.close();
}

void LevelEditor::LoadLevel(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cerr << "Failed to open file for loading: " << filename << std::endl;
        return;
    }

    inFile >> gridWidth >> gridHeight;
    grid.resize(gridHeight, std::vector<int>(gridWidth, 0));

    for (auto& row : grid) {
        for (int& id : row) {
            inFile >> id;
        }
    }

    inFile.close();
}

void LevelEditor::Render(SDL_Renderer* renderer) const {
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
