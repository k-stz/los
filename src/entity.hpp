#ifndef __ENTITY_HPP
#define __ENTITY_HPP

#include <SDL2/SDL.h>
#include "vec.hpp"
#include "util.hpp"

class Entity {
public:
  vec2 position;

  virtual void input(Input *input);
  virtual void update(unsigned int delta)     = 0;
  virtual void render(SDL_Renderer *renderer) = 0;

private:
};

#endif
