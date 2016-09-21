#include "world.hpp"

#include <iostream>

World::World(SDL_Renderer *renderer) {
  this->default_font = TTF_OpenFont("DejaVuSans.ttf", 20);
  if (default_font == NULL) {
    std::cout << "Couldn't load font: " << TTF_GetError() << std::endl;
  }

  this->level = new Level("data/level1.json", renderer);
}

// delta in milliseconds
void World::update(unsigned int delta) {
  this->fps_counter += delta;
  this->frame_counter ++;

  // One second passed
  if (fps_counter >= 1000) {
    this->current_fps = this->frame_counter;
    this->frame_counter = 0;
    this->fps_counter = 0;
  }


  level->update(delta);
}

void World::render(SDL_Renderer *renderer) {
  level->render(renderer);


  // TODO: Update all this only once a second
  char buffer[20];
  sprintf(buffer, "FPS: %d", this->current_fps);
  SDL_Color color = {0, 255, 0, 255};
  SDL_Surface *fps_surface = TTF_RenderText_Solid(default_font, buffer, color);
  SDL_Texture *fps_texture = SDL_CreateTextureFromSurface(renderer, fps_surface);
  SDL_FreeSurface(fps_surface);

  SDL_Rect fps_pos = {0, 0};
  SDL_QueryTexture(fps_texture, NULL, NULL, &fps_pos.w, &fps_pos.h);
  SDL_RenderCopy(renderer, fps_texture, NULL, &fps_pos);
}
