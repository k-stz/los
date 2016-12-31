#include "world.hpp"

#include <iostream>

static const vec2 GRAVITY = vec2(0, 0.00981);
const bool draw_forces = true;

World::World(SDL_Renderer *renderer) {
  this->default_font = TTF_OpenFont("DejaVuSans.ttf", 20);
  if (default_font == NULL) {
    std::cout << "Couldn't load font: " << TTF_GetError() << std::endl;
  }

  this->level = new Level("data/level1.json", renderer);
  this->player = new Player(level);
}

World::~World() {
  delete level;
  delete player;
  TTF_CloseFont(default_font);
}

// just for debugging
void tests(Input *input, Player *player, Level *level) {
  // width_in_tiles = 32
  // height_in_tiles = 24
  int x, y;
  // testing the is_tile_solid() method
  if (input->down == true) {
    if (input->keycode == SDLK_s) {
      while(1) {
        std::cin >> x;
        std::cin >> y;
        printf("level->is_tile_solid(%d,%d): %d\n",
               x, y, level->is_tile_solid(x,y));

      }
    }
    if (input->keycode == SDLK_e) {
      printf("executing experiments----------!\n");
      printf("player x: %f, y: %f\n", player->position.x, player->position.y);
      // for (int i = 0; i < 32; i++) {
      // 	for (int j = 0; j < 24; j ++) {
      // 	  printf("level->is_tile_solid(%d,%d): %d\n",
      // 		 i, j, level->is_tile_solid(i,j));
      // 	}
      // }
    }
  }
  //  printf("%d\n", input->keycode);
}

void World::input(Input *input) {
  // TODO add UI->input (leveleditor, entering menu and debug mode)
  tests(input, player, level);
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

  if (draw_forces) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawLine(renderer,
                       player->position.x + (player->width / 2),
                       player->position.y + (player->height / 2),
                       player->position.x + (player->width / 2) + player->current_force.x * 100,
                       player->position.y + (player->height / 2) + player->current_force.y * 100);
  }


  // TODO: Update all this only once a second
  char buffer[20];
  sprintf(buffer, "FPS: %d", fps_counter.current_fps);
  SDL_Color color = {0, 255, 0, 255};
  SDL_Surface *fps_surface = TTF_RenderText_Solid(default_font, buffer, color);
  SDL_Texture *fps_texture = SDL_CreateTextureFromSurface(renderer, fps_surface);
  SDL_FreeSurface(fps_surface);

  SDL_Rect fps_pos = {0, 0};
  SDL_QueryTexture(fps_texture, NULL, NULL, &fps_pos.w, &fps_pos.h);
  SDL_RenderCopy(renderer, fps_texture, NULL, &fps_pos);
  SDL_DestroyTexture(fps_texture);
}
