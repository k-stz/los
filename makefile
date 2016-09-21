
# Update these paths to match your installation
# You may also need to update the linker option rpath, which sets where to look for
# the SDL2 libraries at runtime to match your install
SDL_LIB = `pkg-config --libs sdl2 SDL2_ttf SDL2_image`
SDL_INCLUDE = `pkg-config --cflags sdl2 SDL2_ttf SDL2_image`

CFLAGS = -Wall -std=c++11 $(SDL_INCLUDE) -g -DJSON_IS_AMALGAMATION
LDFLAGS = $(SDL_LIB)

FILES = \
	main.cpp \
	src/world.cpp \
	src/level.cpp \
	src/entity.cpp \


build: $(FILES)
	$(CXX) $(CFLAGS) $(LDFLAGS) $^ -o bin/main

run-main:
	./bin/main


