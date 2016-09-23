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
  if (input->keycode == SDLK_RIGHT) {
    right_pressed = input->down;
  } else if (input->keycode == SDLK_LEFT) {
    left_pressed = input->down;
  } else if (input->keycode == SDLK_SPACE) {
    space_pressed = true;
  }
}

void Player::update(unsigned int delta) {
  if (left_pressed)
    apply_force(vec2(-(delta / 300.0f), 0));
  else if (right_pressed)
    apply_force(vec2(delta / 300.0f, 0));

  if (space_pressed) {
    apply_force(vec2(0, -(current_force.y + (delta / 10.0f))));
    space_pressed = false;
  }


  // Will update the position according to the currently applied force
  // position attribute is inherited from entity(.hpp)
  Entity::update(delta);


  // collision detection

  int left_x   = position.x / 32;
  int right_x  = (position.x + width) / 32;
  int top_y    = position.y / 32;
  int bottom_y = (position.y + height) / 32;

  // bottom
  bool touches_ground = false;
  if (level->is_tile_solid(left_x, bottom_y) ||
      level->is_tile_solid(right_x, bottom_y)) {
    // move up, reset y force
    this->position.y = (bottom_y - 1) * 32;
    touches_ground = true;
  }

  bool touches_side = false;
  if ((touches_ground && level->is_tile_solid(right_x, top_y)) ||
      (!touches_ground && level->is_tile_solid(right_x, bottom_y))) {
    this->position.x = (right_x - 1) * 32;
    touches_side = true;
  }

  if ((touches_ground && level->is_tile_solid(left_x, top_y)) ||
      (!touches_ground && level->is_tile_solid(left_x, bottom_y))) {
    this->position.x = (left_x + 1) * 32;
    touches_side = true;
  }

  if (touches_ground) {
    this->current_force.y = 0;
    apply_force(vec2(- current_force.x / 100, 0)); // Friction!
  } else {
    current_force.x *= 0.98; // In air
  }

  if (touches_side) {
    this->current_force.x = 0;
    apply_force(vec2(0, - current_force.y / 100)); // Friction!
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


