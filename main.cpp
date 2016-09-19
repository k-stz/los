#include <SDL2/SDL.h>
#include <iostream>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 360;

int main( int argc, char* args[]) {
  printf("Hi, there!\n");

  SDL_Window* window = NULL; // c++ is case-sensitive in its identifiers!
  SDL_Surface* screenSurface = NULL;

  // init SDL 
  // SDL_INIT_VIDEO indicates that we will only use the sdl's video subset
  if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) 
  {
    printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
  } else {
    // Create window
    window = SDL_CreateWindow( "it werks(tm)", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
			       SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    if (window == NULL) {
      printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError());
    } else {
      // get handle to window surface
      screenSurface = SDL_GetWindowSurface( window );
      // fill the surface with some color
      SDL_FillRect( screenSurface, NULL, SDL_MapRGB ( screenSurface->format, 0x1F, 0x00, 0xFF));
      // this call does the actual "framebuffer" update !
      SDL_UpdateWindowSurface( window );     
      
      SDL_Delay( 1000 ); // 2 seconds
    }
  }

  // clean up
  SDL_DestroyWindow( window );
  SDL_Quit();
    
  return 0;

}
