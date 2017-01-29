#include "player.hpp"

#include <iostream>
#include <cstdlib>
#include <SDL2/SDL_image.h>
#include <cassert>

const float MAX_FORCE = 6.0f;

Player::Player(Level *level, SDL_Renderer *renderer, vec2 start_pos) {
  this->level = level;
  this->position = start_pos;
  this->width  = 32;
  this->height = 32;

  auto surface = IMG_Load("../data/kirby.png");
  assert(surface);
  this->texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);
}

void Player::input(const Input *input) {
  if (input->keycode == SDLK_RIGHT) {
    right_pressed = input->down;
  } else if (input->keycode == SDLK_LEFT) {
    left_pressed = input->down;
  } else if (input->keycode == SDLK_SPACE) {
    space_pressed = true;
  } else if (input->keycode == SDLK_UP) {
    up_pressed = input->down;
  } else if (input->keycode == SDLK_DOWN) {
    down_pressed = input->down;
  }
}

void Player::update(unsigned int delta) {
  if (left_pressed)
    apply_force(vec2(-0.06, 0), delta);
  else if (right_pressed)
    apply_force(vec2(+0.06, 0), delta);

  if (up_pressed)
    apply_force(vec2(0, -0.003), delta);
  else if (down_pressed)
    apply_force(vec2(0, +0.003), delta);

  if (space_pressed) {
    apply_force(vec2(0, -0.13), delta);
    space_pressed = false;
  }

  this->hits_bottom = false;
  this->hits_left   = false;
  this->hits_right  = false;
  this->hits_top    = false;


  vec2 new_pos = this->position + (this->current_force * static_cast<float>(delta));

  // Use macros here so we don't have to reevaluate the variables
  // every time we modify new_pos
#define top_tile    (new_pos.y / 32)
#define bottom_tile ((new_pos.y + height) / 32)
#define left_tile   (new_pos.x / 32)
#define right_tile  ((new_pos.x + width) / 32)

  bool touches_bottom = false;
  bool touches_top    = false;
  bool touches_left   = false;
  bool touches_right  = false;

  // TODO: This is now only looking at the 4 corners,
  //       but what about tiles between e.g. bottom left and bottom right
  //       (for larger player sizes)? Care or not?

  // bottom left corner
  if (level->is_tile_solid(left_tile, bottom_tile)) {
    // We should either be moving up or right
    // Push the player into the direction that would
    // cause the least movement.
    int lt = left_tile;
    int bt = bottom_tile;
    vec2 new_up_pos    = vec2(new_pos.x,     (bt * 32) - height);
    vec2 new_right_pos = vec2((lt + 1) * 32, new_pos.y);

    vec2 up_diff    = new_up_pos    - new_pos;
    vec2 right_diff = new_right_pos - new_pos;

    if (up_diff.length() < right_diff.length()) {
      // Move up if that tile is not solid
      if (!level->is_tile_solid(left_tile, bottom_tile - 1)) {
        new_pos.y = (bt * 32) - height;
        current_force.y = 0;
        touches_bottom = true;
      }
    } else {
      // Move right if that tile is not solid
      if (!level->is_tile_solid(left_tile + 1, bottom_tile)) {
        new_pos.x = (lt + 1) * 32;
        current_force.x = 0;
        touches_left = true;
      }
    }
  }

  // bottom right corner
  if (level->is_tile_solid(right_tile, bottom_tile)) {
    int rt = right_tile;
    int bt = bottom_tile;
    vec2 new_up_pos    = vec2(new_pos.x, (bt * 32) - height);
    vec2 new_left_pos  = vec2(rt * 32- width, new_pos.y);

    vec2 up_diff   = new_up_pos - new_pos;
    vec2 left_diff = new_left_pos - new_pos;

    if (up_diff.length() < left_diff.length()) {
      // Move up if that tile is not solid
      if (!level->is_tile_solid(right_tile, bottom_tile - 1)) {
        new_pos.y = (bt * 32) - height;
        current_force.y = 0;
        touches_bottom = true;
      }
    } else {
      // Move left if that tile is not solid
      if (!level->is_tile_solid(right_tile - 1, bottom_tile)) {
        new_pos.x = rt * 32 - width;
        current_force.x = 0;
        touches_right = true;
      }
    }
  }

  // Upper left corner
  if (level->is_tile_solid(left_tile, top_tile)) {
    int tt = top_tile;
    int lt = left_tile;

    vec2 new_down_pos  = vec2(new_pos.x, (tt + 1) * 32);
    vec2 new_right_pos = vec2((lt + 1) * 32, new_pos.y);

    vec2 down_diff  = new_down_pos - new_pos;
    vec2 right_diff = new_right_pos - new_pos;

    if (down_diff.length() < right_diff.length()) {
      // Move down if that tile is not solid
      if (!level->is_tile_solid(left_tile, top_tile + 1)) {
        new_pos.y = (tt + 1) * 32;
        current_force.y = 0;
        touches_top = true;
      }
    } else {
      // Move right if that tile is not solid
      if (!level->is_tile_solid(left_tile + 1, top_tile)) {
        new_pos.x = (lt + 1) * 32;
        current_force.x = 0;
        touches_left = true;
      }
    }
  }

  // Upper right corner
  if (level->is_tile_solid(right_tile, top_tile)) {
    int tt = top_tile;
    int rt = right_tile;

    vec2 new_down_pos  = vec2(new_pos.x, (tt + 1) * 32);
    vec2 new_left_pos  = vec2(rt * 32- width, new_pos.y);

    vec2 down_diff  = new_down_pos - new_pos;
    vec2 left_diff = new_left_pos - new_pos;

    if (down_diff.length() < left_diff.length()) {
      // Move down if that tile is not solid
      if (!level->is_tile_solid(right_tile, top_tile + 1)) {
        new_pos.y = (tt + 1) * 32;
        current_force.y = 0;
        touches_top = true;
      }
    } else {
      // Move left
      if (!level->is_tile_solid(right_tile - 1, top_tile)) {
        new_pos.x = rt * 32 - width;
        current_force.x = 0;
        touches_right = true;
      }
    }
  }

  this->hits_bottom = touches_bottom;
  this->hits_left   = touches_left;
  this->hits_right  = touches_right;
  this->hits_top    = touches_top;

  if (touches_top || touches_bottom) {
    apply_force(vec2(- current_force.x / 7.0f, 0), delta); // Friction
  } else {
    // In air
    current_force.x *= 1.0 - (delta * 0.02); // In air
  }

  if (touches_left || touches_right) {
    apply_force(vec2(0, - current_force.y / 100), delta); // Friction
  }

  this->current_force.min_all(MAX_FORCE);

  this->position = new_pos;
}

void Player::render(SDL_Renderer *renderer) {
  SDL_Rect r = { static_cast<int>(position.x) + level->offset_x,
                 static_cast<int>(position.y) + level->offset_y,
                 width, height};
  SDL_Rect r2 = { 0, 0, width, height};

  SDL_RenderCopy(renderer, this->texture, &r2, &r);

#if 0
  if (hits_bottom) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    SDL_Rect r = {static_cast<int>(position.x),
                  static_cast<int>(position.y) + height - 5,
                  width, 5};
    SDL_RenderFillRect(renderer, &r);
  }

  if (hits_top) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    SDL_Rect r = {static_cast<int>(position.x),
                  static_cast<int>(position.y),
                  width, 5};
    SDL_RenderFillRect(renderer, &r);
  }

  if (hits_left) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    SDL_Rect r = {static_cast<int>(position.x),
                  static_cast<int>(position.y),
                  5, height};
    SDL_RenderFillRect(renderer, &r);
  }

  if (hits_right) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    SDL_Rect r = {static_cast<int>(position.x + width - 5),
                  static_cast<int>(position.y),
                  5, height};
    SDL_RenderFillRect(renderer, &r);

  }
#endif
}


