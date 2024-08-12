#include "TileSet.hpp"

void TileSet::AddTile(int id, SDL_Texture* texture, const std::vector<Hitbox>& hitboxes) {
    if (tiles.find(id) == tiles.end()) {
        tiles[id] = new Tile(id, texture, hitboxes, 0, 0);
    }
}

Tile* TileSet::GetTile(int id) const {
    auto it = tiles.find(id);
    if (it != tiles.end()) {
        return it->second;
    }
    return nullptr;
}
