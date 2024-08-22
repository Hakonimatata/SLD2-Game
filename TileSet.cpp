#include "TileSet.hpp"
#include <iostream>


TileSet::TileSet(SDL_Renderer* renderer) : renderer(renderer) 
{
    // Load tiles

    // ------- 1 x 1 tiles -------
    AddTile(1, "assets/Tiles/Flat.png", /* hitboxes. Tomt nå */ {}, 1, 1);
    AddTile(2, "assets/Tiles/FlatAngled.png", /* hitboxes. Tomt nå */ {}, 1, 1);
    AddTile(3, "assets/Tiles/FlatAngledInv.png", /* hitboxes. Tomt på højre */ {}, 1, 1);
    AddTile(4, "assets/Tiles/Grass.png", /* hitboxes. Tomt på venstre */ {}, 1, 1);
    AddTile(5, "assets/Tiles/InnerTurn.png", /* hitboxes. Tomt på højre og venstre */ {}, 1, 1);
    AddTile(6, "assets/Tiles/OuterTurn.png", /* hitboxes. Tomt på venstre og højre */ {}, 1, 1);
    AddTile(7, "assets/Tiles/Road.png", /* hitboxes. Tomt på højre og venstre */ {}, 1, 1);
    AddTile(8, "assets/Tiles/SlowInnerTurn.png", /* hitboxes. Tomt på højre og venstre */ {}, 1, 1);
    AddTile(9, "assets/Tiles/SlowInnerTurnInv.png", /* hitboxes. Tomt på højre og venstre */ {}, 1, 1);
    AddTile(10, "assets/Tiles/SlowOuterTurn.png", /* hitboxes. Tomt på højre og venstre */ {}, 1, 1);
    AddTile(11, "assets/Tiles/SlowOuterTurnInv.png", /* hitboxes. Tomt på højre og venstre */ {}, 1, 1);

    // ------- 1 x 2 tiles -------
    AddTile(12, "assets/Tiles/Flat.png", /* hitboxes. Tomt nå */ {}, 1, 2); // Height = 2

    // ------- 2 x 2 tiles -------
    AddTile(13, "assets/Tiles/OuterTurn.png", /* hitboxes. Tomt på venstre og højre */ {}, 2, 2);


}

TileSet::~TileSet() {
    for (auto& pair : tiles) {
        delete pair.second;
    }
}

Tile* TileSet::GetTile(int id) const {
    auto it = tiles.find(id);
    if (it != tiles.end()) {
        return it->second;
    }
    return nullptr;
}



void TileSet::AddTile(int id, const std::string& imagePath, const std::vector<Hitbox>& hitboxes, int width, int height) {
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

    Tile* tile = new Tile(id, texture, hitboxes, 0, 0, width, height);
    tiles[id] = tile;
}
