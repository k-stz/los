#include "player.hpp"

#include <iostream>


const float MAX_FORCE = 10.0f;

// TODO: replace with actual size
int width = 32;
int height = 32;

Player::Player(Level *level) {
  this->level = level;
  this->position.x = 120;
  this->position.y = 80;
}

void Player::input(Input *input) {
  if (input->down) {
    if (input->keycode == SDLK_RIGHT) {
      apply_force(vec2(0.1, 0));
    } else if (input->keycode == SDLK_LEFT) {
      apply_force(vec2(-0.1, 0));
    } else if (input->keycode == SDLK_SPACE) {
      apply_force(vec2(0, -0.2));
    }
  }
}

void Player::update(unsigned int delta) {
  // Will update the position according to the currently applied force
  Entity::update(delta);


  // collision detection

  int left_x  = position.x / 32;
  int right_x = (position.x + width) / 32;
  // bottom
  bool touches_ground = false;
  int bottom_y = (position.y + height) / 32;

  if (level->is_tile_solid(left_x, bottom_y)) {
    // move up, reset y force
    this->position.y = (bottom_y - 1) * 32;
    touches_ground = true;
  }

  if (level->is_tile_solid(right_x, bottom_y)) {
    this->position.y = (bottom_y - 1) * 32;
    touches_ground = true;
  }

  if (touches_ground) {
  this->current_force.y = 0;
    apply_force(vec2(- current_force.x / 100, 0)); // Friction!
  }


  this->current_force.min_all(MAX_FORCE);
}

void Player::render(SDL_Renderer *renderer) {
  SDL_Rect r = { static_cast<int>(position.x),
                 static_cast<int>(position.y),
                 width, height};

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderFillRect (renderer, &r);
}


