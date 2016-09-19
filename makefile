
# Update these paths to match your installation
# You may also need to update the linker option rpath, which sets where to look for
# the SDL2 libraries at runtime to match your install
SDL_LIB = -L/usr/lib -lSDL2 -Wl,-rpath=/usr/lib
SDL_INCLUDE = -I/usr/include

FLAGS = -Wall -c -std=c++11 $(SDL_INCLUDE)
LDFLAGS = $(SDL_LIB)

build: 
	g++ $< $(LDFLAGS) main.cpp -o bin/main

run-main:
	./bin/main


