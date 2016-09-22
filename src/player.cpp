#include "player.hpp"

Player::Player(Level *level) {
  this->level = level;
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

}

void Player::render(SDL_Renderer *renderer) {
  SDL_Rect r = { static_cast<int>(position.x),
                 static_cast<int>(position.y),
                 20, 20};

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderFillRect (renderer, &r);
}


