#ifndef __PLAYER_HPP
#define __PLAYER_HPP

#include "entity.hpp"
#include "level.hpp"
#include "util.hpp"
#include "spriteanimation.hpp"

class Player : public Entity {
public:
  Player(Level *level, SDL_Renderer *renderer, vec2 start_pos);

  virtual ~Player(){}

  void input(const Input *input) override;
  void update(unsigned int delta) override;
  void render(SDL_Renderer *renderer) override;

private:
  Level *level;
  SpriteAnimation *animation;

  // animations
  uint standing_anim;
  uint walkin_right_anim;

  // Input
  bool left_pressed  = false;
  bool right_pressed = false;
  bool space_pressed = false;
  bool up_pressed    = false;
  bool down_pressed  = false;

  // Debugging
  bool hits_left   = false;
  bool hits_bottom = false;
  bool hits_right  = false;
  bool hits_top    = false;
};

#endif
