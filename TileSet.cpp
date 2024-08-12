#include "TileSet.hpp"
#include <iostream>
#

TileSet::TileSet(SDL_Renderer* renderer) : renderer(renderer) {}

TileSet::~TileSet() {
    for (auto& pair : tiles) {
        delete pair.second;
    }
}

void TileSet::AddTile(int id, const std::string& imagePath, const std::vector<Hitbox>& hitboxes) {
    SDL_Surface* tempSurface = IMG_Load(imagePath.c_str());
    if (!tempSurface) {
        std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    if (!texture) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
        return;
    }

    Tile* tile = new Tile(id, texture, hitboxes, 0, 0);
    tiles[id] = tile;
}

Tile* TileSet::GetTile(int id) const {
    auto it = tiles.find(id);
    if (it != tiles.end()) {
        return it->second;
    }
    return nullptr;
}