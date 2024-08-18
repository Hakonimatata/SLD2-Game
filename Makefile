all:
	g++ -I src/include/SDL2 -L src/lib -o main main.cpp Game.cpp Player.cpp Car.cpp Utilities.cpp LevelEditor.cpp Tile.cpp Hitbox.cpp TileSet.cpp Button.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
