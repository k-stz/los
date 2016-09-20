
# Update these paths to match your installation
# You may also need to update the linker option rpath, which sets where to look for
# the SDL2 libraries at runtime to match your install
SDL_LIB = `pkg-config --libs sdl2`
SDL_INCLUDE = `pkg-config --cflags sdl2`

FLAGS = -Wall -c -std=c++11 $(SDL_INCLUDE)
LDFLAGS = $(SDL_LIB)

FILES = \
	main.cpp

build: $(FILES)
	$(CXX) $(LDFLAGS) $^ -o bin/main

run-main:
	./bin/main


