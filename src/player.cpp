#include "player.hpp"

#include <iostream>
#include <cstdlib>


const float MAX_FORCE = 6.0f;

Player::Player(Level *level) {
  this->level = level;
  this->position.x = 0;
  this->position.y = 0;
  this->width  = 32;
  this->height = 32;
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
    apply_force(vec2(0, -(current_force.y + (delta / 2.0f))));
    space_pressed = false;
  }


  // Will update the position according to the currently applied force
  // position attribute is inherited from entity(.hpp)
  vec2 new_pos = this->position + (this->current_force * static_cast<float>(delta));

  // collision detection
  int left_x   =  new_pos.x / 32.0f;
  int right_x  = (new_pos.x - 1.0f + width) / 32.0f;
  int top_y    =  new_pos.y / 32.0f;
  int bottom_y = (new_pos.y - 1 + height) / 32.0f;

  bool touches_left   = false;
  bool touches_right  = false;
  bool touches_bottom = false;



  // Simple case first: The center of each side
  // bottom
  // printf("level->is_tile_solid(%d, %d) ==> %d\n",
  // 	 (new_pos.x + (width / 2)) / 32, 
  // 	 bottom_y,
  // 	 level->is_tile_solid((new_pos.x + (width / 2)) / 32, bottom_y));
  if (level->is_tile_solid((new_pos.x + (width / 2)) / 32, bottom_y)) {
    // printf("yes, this is never reached, %d %d", (new_pos.x + (width / 2)),
    // 	   bottom_y);
    current_force.y = 0;
    new_pos.y = top_y * 32;
    top_y    =  new_pos.y / 32.0f;
    bottom_y = (new_pos.y - 1 + height) / 32.0f;
    touches_bottom = true;
  }

  // left
  if (level->is_tile_solid(left_x, (new_pos.y + (height / 2)) / 32)) {
    current_force.x = 0;
    new_pos.x = (left_x + 1) * 32;
    left_x   =  new_pos.x / 32.0f;
    right_x  = (new_pos.x - 1.0f + width) / 32.0f;
    touches_left = true;
  }

  //right
  if (level->is_tile_solid(right_x, (new_pos.y + (height / 2)) / 32)) {
    current_force.x = 0;
    new_pos.x = left_x * 32;
    left_x   =  new_pos.x / 32.0f;
    right_x  = (new_pos.x - 1.0f + width) / 32.0f;
    touches_right = true;
  }


  // Now check the same thing again but with both edges on each side

  const bool _touches_right  = level->is_tile_solid(right_x, top_y) ||
                              level->is_tile_solid(right_x, bottom_y);
  if (_touches_right) {
    current_force.x = 0;
    new_pos.x = left_x * 32;
    top_y    =  new_pos.y / 32.0f;
    bottom_y = (new_pos.y - 1 + height) / 32.0f;
    touches_right = true;
  }

  const bool _touches_left = level->is_tile_solid(left_x, top_y) ||
                            level->is_tile_solid(left_x, bottom_y);
  if (_touches_left) {
    current_force.x = 0;
    new_pos.x = left_x * 32;
    left_x   =  new_pos.x / 32.0f;
    right_x  = (new_pos.x - 1.0f + width) / 32.0f;
    touches_left = true;
  }


  bool _touches_bottom = level->is_tile_solid(left_x, bottom_y) ||
                        level->is_tile_solid(right_x, bottom_y);
  if (_touches_bottom) {
    current_force.y = 0;
    new_pos.y = top_y * 32;
    left_x   =  new_pos.x / 32.0f;
    right_x  = (new_pos.x - 1.0f + width) / 32.0f;
    touches_bottom = true;
  }

  if (touches_bottom) {
    this->current_force.y = 0;
    apply_force(vec2(- current_force.x / 100, 0)); // Friction!
  } else {
    // TODO needs delta factor
    current_force.x *= 0.98; // In air
  }

  if (touches_left || touches_right) {
    this->current_force.x = 0;
    apply_force(vec2(0, - current_force.y / 100)); // Friction!
  }

  this->current_force.min_all(MAX_FORCE);
  this->position = new_pos;
}

void Player::render(SDL_Renderer *renderer) {
  SDL_Rect r = { static_cast<int>(position.x),
                 static_cast<int>(position.y),
                 width, height};

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderFillRect (renderer, &r);
}


