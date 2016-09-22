#include "player.hpp"

#include <iostream>

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
      this->position.x += 5;
    } else if (input->keycode == SDLK_LEFT) {
      this->position.x -= 5;
    } else if (input->keycode == SDLK_UP) {
      this->position.y -= 5;
    } else if (input->keycode == SDLK_DOWN) {
      this->position.y += 5;
    }
  }
}

void Player::update(unsigned int delta) {
  // Will update the position according to the currently applied force
  Entity::update(delta);


  // collision detection

  int center_x = (position.x + width/2) / 32;
  // bottom
  int bottom_y = (position.y + height) / 32;
  std::cout << bottom_y << std::endl;
  if (level->is_tile_solid(center_x, bottom_y)) {
      // move up, reset y force
      this->current_force.y = 0;
      this->position.y = (bottom_y - 1) * 32;
  }
}

void Player::render(SDL_Renderer *renderer) {
  SDL_Rect r = { static_cast<int>(position.x),
                 static_cast<int>(position.y),
                 width, height};

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderFillRect (renderer, &r);
}


