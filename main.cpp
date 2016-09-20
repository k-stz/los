#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include "src/world.hpp"
#include "src/events.cpp"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 360;

// TODO put somewhere else
void quit(SDL_Window* window) {
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

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // sets clear-color to Red



  world = new World();
  // TODO: We should do event handling somewhere else
  SDL_Event event;
  unsigned int last_time = SDL_GetTicks();
  unsigned int current_time;
  while (running) {

    running = handleEvents(event, window);

    current_time = SDL_GetTicks();
    unsigned int frame_time = current_time - last_time;
    world->update(frame_time);

    SDL_RenderClear(renderer);
    world->render(renderer);
    SDL_RenderPresent(renderer);

    SDL_UpdateWindowSurface (window);
    last_time = current_time;

  }

  quit(window);

  return 0;

}

