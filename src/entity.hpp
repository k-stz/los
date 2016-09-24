#ifndef __ENTITY_HPP
#define __ENTITY_HPP

#include <SDL2/SDL.h>
#include "vec.hpp"
#include "util.hpp"

class Entity {
public:
  vec2 position;
  vec2 current_force;
  int width;
  int height;
  bool is_static = true;

  virtual ~Entity(){}

  virtual void input(Input *input);
  virtual void update(unsigned int delta);
  virtual void render(SDL_Renderer *renderer) = 0;

  void apply_force(vec2 force);

private:
};

#endif
