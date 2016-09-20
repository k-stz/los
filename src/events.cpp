#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>


bool handleEvents(SDL_Event event, SDL_Window* window) {
  while (SDL_PollEvent(&event) != 0) {
    printf(" event type progress: %d \n", event.type);
    if (event.type == SDL_QUIT) {
      return false; // running = false
    }
  }
  return true;
  
}
