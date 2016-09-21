#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>


bool handleEvents(SDL_Event event, SDL_Window* window) {
  while (SDL_PollEvent(&event) != 0) {
    if (event.type == SDL_QUIT) {
      return false; // running = false
    }
  }
  return true;

}
