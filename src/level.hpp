#ifndef __LEVEL_HPP
#define __LEVEL_HPP

#include <SDL2/SDL.h>
#include <string>

const int TILE_WIDTH = 32;
const int TILE_HEIGHT = 32;

struct Tile {
  int x, y;
  int index;
};

class Level {
public:

  Level(const std::string &filename, SDL_Renderer *renderer);
  void update(unsigned int delta);
  void render(SDL_Renderer *renderer);

  bool is_tile_solid(unsigned int x, unsigned int y);

private:
  unsigned int width_in_tiles = 0;
  unsigned int height_in_tiles = 0;

  std::string name;
  SDL_Texture *bg_texture = nullptr;

  Tile *tiles;

  void load(const std::string &filename, SDL_Renderer *renderer);
};

#endif
