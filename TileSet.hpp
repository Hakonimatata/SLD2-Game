#ifndef TILESET_HPP
#define TILESET_HPP

#include <unordered_map>
#include "Tile.hpp"

class TileSet {
public:
    // Load tile into the tileset
    void AddTile(int id, SDL_Texture* texture, const std::vector<Hitbox>& hitboxes);

    // Get a tile by its ID
    Tile* GetTile(int id) const;

private:
    std::unordered_map<int, Tile*> tiles; // Mapping from ID to Tile
};

#endif // TILESET_HPP
