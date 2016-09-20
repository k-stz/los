#include <SDL2/SDL.h>
#include <iostream>
#include "src/world.hpp"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 360;

int main( int argc, char* args[]) {
  SDL_Window* window = NULL;
  SDL_Surface* screenSurface = NULL;
  World *world = new World();
  bool running = true;

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS) < 0) {
    std::cout << "Couldn't initialize SDL" << std::endl;
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

  screenSurface = SDL_GetWindowSurface( window );

  // TODO: We should do event handling somewhere else
  SDL_Event e;
  while (running) {
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) {
        running = false;
      }
    }

    SDL_FillRect( screenSurface, NULL, SDL_MapRGB ( screenSurface->format, 0x1F, 0x00, 0xFF));
    SDL_UpdateWindowSurface( window );
  }

  // clean up
  SDL_DestroyWindow( window );
  SDL_Quit();

  return 0;

}
