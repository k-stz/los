#include "level.hpp"
#include "picojson.h"

#include <iostream>
#include <fstream>
#include <SDL2/SDL_image.h>
#include <cassert>

Level::Level(const std::string &filename, SDL_Renderer *renderer) {
  this->load(filename, renderer);
}

void Level::update(unsigned int delta) {

}

void Level::render(SDL_Renderer *renderer) {
  // First render (optional) background image
  int w, h;
  SDL_GetRendererOutputSize(renderer, &w, &h);
  SDL_Rect bg_rect = {0, 0, w, h};
  SDL_RenderCopy(renderer, bg_texture, NULL, &bg_rect);

  for (unsigned int i = 0; i < width_in_tiles * height_in_tiles; i ++) {
    // empty tile
    if (tiles[i].index == -1)
      continue;

    int y =  i / width_in_tiles;
    int x = i - (y * height_in_tiles);

    // TODO: Just fill them red for now
    SDL_Rect r = {x * TILE_WIDTH, y * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT};
    SDL_RenderFillRect (renderer, &r);
  }


}

void Level::load(const std::string &filename, SDL_Renderer *renderer) {
  std::cout << "Loading " << filename << "..." << std::endl;

  // XXX Slow
  std::ifstream t(filename);
  std::string file_contents((std::istreambuf_iterator<char>(t)),
                             std::istreambuf_iterator<char>());

  picojson::value root;
  std::string err = picojson::parse(root, file_contents);
  if (!err.empty()) {
    std::cerr << err << std::endl;
  }

  auto root_obj = root.get<picojson::object>();
  this->name = root_obj["name"].get<std::string>();
  this->width_in_tiles = static_cast<unsigned int>(root_obj["width_in_tiles"].get<double>());
  this->height_in_tiles = static_cast<unsigned int>(root_obj["height_in_tiles"].get<double>());

  this->tiles = new Tile[width_in_tiles * height_in_tiles];
  for (unsigned int i = 0; i < width_in_tiles * height_in_tiles; i ++)
    tiles[i] = {0, 0, -1};

  // Load background image
  // TODO: Make this optional
  std::string bg_path = "data/" + root_obj["background"].get<std::string>();
  SDL_Surface *bg_surface = IMG_Load(bg_path.c_str());
  this->bg_texture = SDL_CreateTextureFromSurface(renderer, bg_surface);
  SDL_FreeSurface(bg_surface);
  assert(bg_texture != nullptr);


  auto tiles_array = root_obj["tiles"].get<picojson::array>();
  int cur_row = 0;
  for (auto r : tiles_array) {
    auto row = r.get<picojson::array>();

    int cur_col = 0;
    for (auto c : row) {
      int tile_id = static_cast<int>(c.get<double>());
      tiles[cur_row * width_in_tiles + cur_col].index = tile_id;
      cur_col ++;
    }
    cur_row ++;
  }
}


