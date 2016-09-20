#ifndef __ENTITY_HPP
#define __ENTITY_HPP

#include <SDL2/SDL.h>
#include "vec.hpp"

class Entity {
private:


public:
  vec2 position;

  virtual void update(unsigned int delta);
  virtual void render(SDL_Renderer *renderer);
};

#endif
