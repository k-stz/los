#ifndef __LEVEL_HPP
#define __LEVEL_HPP

#include <SDL2/SDL.h>
#include <string>

class Level {
public:

  Level(const std::string &filename, SDL_Renderer *renderer);
  void update(unsigned int delta);
  void render(SDL_Renderer *renderer);

private:
  unsigned int width_in_tiles = 0;
  unsigned int height_in_tiles = 0;

  std::string name;
  SDL_Texture *bg_texture = nullptr;

  void load(const std::string &filename, SDL_Renderer *renderer);
};

#endif
