#include "world.hpp"

#include <iostream>
#include <cmath>
#include "debug.hpp"

static const vec2 GRAVITY = vec2(0, 0.00981);

World::World(SDL_Renderer *renderer) {
  this->default_font = TTF_OpenFont("../DejaVuSans.ttf", 16);
  if (default_font == NULL) {
    std::cout << "Couldn't load font: " << TTF_GetError() << std::endl;
  }

  this->level = new Level("level2", renderer);
  this->player = new Player(level, renderer, level->get_player_start_pos());
}

World::~World() {
  delete level;
  delete player;
  TTF_CloseFont(default_font);
}

void World::input(Input *input) {
  player->input(input);
}

// delta in milliseconds - time that has passed since the last call to update()
void World::update(unsigned int delta) {
  fps_counter.update(delta);

  player->apply_force(GRAVITY, delta);

  level->update(delta);
  player->update(delta);
}

void World::render(SDL_Renderer *renderer) {
  level->render(renderer);
  player->render(renderer);

  if (DRAW_FORCES) {
    if (std::abs(player->current_force.x * 100) >= 1.0 ||
        std::abs(player->current_force.y * 100) >= 1.0) {
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      SDL_RenderDrawLine(renderer,
                         player->position.x + (player->width / 2) + level->offset_x,
                         player->position.y + (player->height / 2) + level->offset_y,
                         player->position.x + (player->width / 2) + player->current_force.x * 100 + level->offset_x,
                         player->position.y + (player->height / 2) + player->current_force.y * 100 + level->offset_y);
    }
  }


  // TODO: Update all this only once a second
  char buffer[20];
  sprintf(buffer, "FPS: %d", fps_counter.current_fps);
  SDL_Color color = {0, 255, 0, 255};
  SDL_Surface *fps_surface = TTF_RenderText_Blended(default_font, buffer, color);
  SDL_Texture *fps_texture = SDL_CreateTextureFromSurface(renderer, fps_surface);
  SDL_FreeSurface(fps_surface);

  SDL_Rect fps_pos = {10, 10};
  SDL_QueryTexture(fps_texture, NULL, NULL, &fps_pos.w, &fps_pos.h);
  SDL_RenderCopy(renderer, fps_texture, NULL, &fps_pos);
  SDL_DestroyTexture(fps_texture);
}
