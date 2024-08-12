#ifndef LEVELEDITOR_HPP
#define LEVELEDITOR_HPP

#include <vector>
#include <string>
#include "TileSet.hpp"

class LevelEditor {
public:
    LevelEditor(int gridWidth, int gridHeight, int tileSize, TileSet* tileSet);
    
    void PlaceTile(int x, int y, int tileID);
    void SaveLevel(const std::string& filename) const;
    void LoadLevel(const std::string& filename);

    void Render(SDL_Renderer* renderer) const;

private:
    int gridWidth, gridHeight;
    int tileSize;
    std::vector<std::vector<int>> grid; // Grid storing tile IDs
    TileSet* tileSet; // Reference to the TileSet
};

#endif // LEVELEDITOR_HPP
