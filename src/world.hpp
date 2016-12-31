#ifndef __WORLD_HPP
#define __WORLD_HPP

#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "level.hpp"
#include "player.hpp"
#include "util.hpp"
#include "fps_counter.hpp"

class World {
public:
  World(SDL_Renderer *renderer);
  ~World();

  void input(Input *input);
  void update(unsigned int delta);
  void render(SDL_Renderer *renderer);

private:
  TTF_Font *default_font;

  // Current game state
  FPSCounter fps_counter;
  Level  *level;
  Player *player;
};

#endif
