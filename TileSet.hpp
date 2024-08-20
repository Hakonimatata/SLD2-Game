#ifndef TILESET_HPP
#define TILESET_HPP


#include <map>
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include "SDL_image.h"
#include "Tile.hpp"

class TileSet {
public:
    TileSet(SDL_Renderer* renderer);
    ~TileSet();

    // Legg til en tile i settet
    void AddTile(int id, const std::string& imagePath, const std::vector<Hitbox>& hitboxes, int width, int height);
    
    // Hent en tile basert på ID
    Tile* GetTile(int id) const;

    std::map<int, Tile*> tiles;

private:
    SDL_Renderer* renderer;
};


#endif // TILESET_HPP