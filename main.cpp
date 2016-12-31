#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include "src/world.hpp"
#include "src/util.hpp"

const int SCREEN_WIDTH  = 1024;
const int SCREEN_HEIGHT = 768;

const int TARGET_FPS    = 60;

// TODO put somewhere else
void quit(SDL_Window* window) {
  TTF_Quit();
  // clean up
  SDL_DestroyWindow( window );
  SDL_Quit();
}


int main( int argc, char* args[]) {
  SDL_Window* window = NULL;
  World *world;
  bool running = true;

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS) < 0) {
    std::cout << "Couldn't initialize SDL" << std::endl;
    return -1;
  }

  if (TTF_Init() == -1) {
    std::cout << "Couldn't initialize sdl_ttf: " << TTF_GetError() << std::endl;
    return -1;
  }

  window = SDL_CreateWindow("it werks(tm)",
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            SCREEN_WIDTH, SCREEN_HEIGHT,
                            SDL_WINDOW_SHOWN);

  if (window == NULL) {
    std::cout << "Window is NULL" << std::endl;
    return -2;
  }

  // use SDL_RENDERER_SOFTWARE on Laptop !
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // sets clear-color to Red



  world = new World(renderer);
  SDL_Event event;
  unsigned int last_time = SDL_GetTicks();
  unsigned int current_time;
  while (running) {
    while(SDL_PollEvent(&event)) {
      Input input; // first we collect the PollEvent inputs in var "input"
      switch(event.type) {
        case SDL_KEYDOWN:
          input.down = true;
          input.keycode = event.key.keysym.sym;
          break;
        case SDL_KEYUP:
          input.up = true;
          input.keycode = event.key.keysym.sym;
          break;
        case SDL_QUIT:
          running = false;
          break;
      }
      // then we pass the input to the world, which currently in turn passes it to the player
      if (running) {
        world->input(&input);
      }
    }

    current_time = SDL_GetTicks();
    unsigned int frame_time = current_time - last_time;
    world->update(frame_time);

    SDL_RenderClear(renderer);
    world->render(renderer);
    SDL_RenderPresent(renderer);

    SDL_UpdateWindowSurface (window);
    last_time = current_time;

    // Burn CPU until we hit the desired FPS
    unsigned int time_per_frame = 1000 / TARGET_FPS;
    while (SDL_GetTicks() < current_time + time_per_frame) {
      // Idle
    }
  }

  delete world;
  SDL_DestroyRenderer(renderer);

  quit(window);

  return 0;
}

