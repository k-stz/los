#ifndef __WORLD_HPP
#define __WORLD_HPP

#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class World {
public:
  World();

  void update(unsigned int delta);
  void render(SDL_Renderer *renderer);

private:
  unsigned int fps_counter   = 0;
  unsigned int frame_counter = 0;
  unsigned int current_fps   = 0;

  TTF_Font *default_font;
};

#endif