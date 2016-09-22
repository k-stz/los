#ifndef __PLAYER_HPP
#define __PLAYER_HPP

#include "entity.hpp"
#include "level.hpp"
#include "util.hpp"

class Player : public Entity {
public:
  Player(Level *level);

  void input(Input *input) override;
  void update(unsigned int delta) override;
  void render(SDL_Renderer *renderer) override;

private:
  vec2 current_force;
  Level *level;
};

#endif
