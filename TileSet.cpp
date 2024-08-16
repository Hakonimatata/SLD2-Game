#include "TileSet.hpp"
#include <iostream>


TileSet::TileSet(SDL_Renderer* renderer) : renderer(renderer) 
{
    // Load tiles
    AddTile(1, "assets/Tiles/Flat.png", /* hitboxes. Tomt nå */ {});
    AddTile(2, "assets/Tiles/FlatAngled.png", /* hitboxes. Tomt nå */ {});
    AddTile(3, "assets/Tiles/FlatAngledInv.png", /* hitboxes. Tomt på højre */ {});
    AddTile(4, "assets/Tiles/Grass.png", /* hitboxes. Tomt på venstre */ {});
    AddTile(5, "assets/Tiles/InnerTurn.png", /* hitboxes. Tomt på højre og venstre */ {});
    AddTile(6, "assets/Tiles/OuterTurn.png", /* hitboxes. Tomt på venstre og højre */ {});
    AddTile(7, "assets/Tiles/Road.png", /* hitboxes. Tomt på højre og venstre */ {});
    AddTile(8, "assets/Tiles/SlowInnerTurn.png", /* hitboxes. Tomt på højre og venstre */ {});
    AddTile(9, "assets/Tiles/SlowInnerTurnInv.png", /* hitboxes. Tomt på højre og venstre */ {});
    AddTile(10, "assets/Tiles/SlowOuterTurn.png", /* hitboxes. Tomt på højre og venstre */ {});
    AddTile(11, "assets/Tiles/SlowOuterTurnInv.png", /* hitboxes. Tomt på højre og venstre */ {});
}

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