#include "level.hpp"
#include "picojson.h"

#include <iostream>
#include <fstream>
#include <SDL2/SDL_image.h>
#include <cassert>

Level::Level(const std::string &filename, SDL_Renderer *renderer) {
  this->load(filename, renderer);
}

Level::~Level() {
  SDL_DestroyTexture(tileset_texture);
  SDL_DestroyTexture(bg_texture);
  delete[] tiles;
  delete[] tile_definitions;
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
    int x = i - (y * width_in_tiles);

    TileDefinition *tile_def = &this->tile_definitions[this->tiles[i].index];
    SDL_Rect texture_coords = {tile_def->x, tile_def->y, tile_def->w, tile_def->h};
    SDL_Rect screen_coords  = {x * TILE_WIDTH, y * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT};

    SDL_RenderCopy(renderer, this->tileset_texture,
                   &texture_coords, &screen_coords);
  }



  // Debug: Draw tile borders
#if 0
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  for (unsigned int x = 1; x < width_in_tiles; x ++) {
    SDL_RenderDrawLine(renderer, x * 32, 0, x * 32, height_in_tiles * 32);
  }

  for (unsigned int y = 1; y < height_in_tiles; y ++) {
    SDL_RenderDrawLine(renderer, 0, y * 32, width_in_tiles * 32, y * 32);
  }
#endif
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

  // Parse tileset
  // TODO: We might want to just save tilesets in their own file
  auto tileset_obj = root_obj["tileset"].get<picojson::object>();
  std::string tileset_image = "data/" + tileset_obj["image"].get<std::string>();
  SDL_Surface *tiledef_surface = IMG_Load(tileset_image.c_str());
  this->tileset_texture = SDL_CreateTextureFromSurface(renderer, tiledef_surface);
  SDL_FreeSurface(tiledef_surface);
  assert(tileset_texture != nullptr);

  auto tiledef_array = tileset_obj["tiles"].get<picojson::array>();
  this->tileset_size = tiledef_array.size();
  this->tile_definitions = new TileDefinition[tileset_size];

  unsigned int tiledef_id = 0;
  for (auto t : tiledef_array) {
    auto tile_arr = t.get<picojson::array>();
    this->tile_definitions[tiledef_id].x = static_cast<int>(tile_arr[0].get<double>());
    this->tile_definitions[tiledef_id].y = static_cast<int>(tile_arr[1].get<double>());
    this->tile_definitions[tiledef_id].w = static_cast<int>(tile_arr[2].get<double>());
    this->tile_definitions[tiledef_id].h = static_cast<int>(tile_arr[3].get<double>());
    tiledef_id ++;
  }

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

bool Level::is_tile_solid(unsigned int x, unsigned int y) {
  // width_in_tiles and height_... are read from the data/.json file!
  assert(x >= 0);
  assert(x < width_in_tiles);
  assert(y >= 0);
  assert(y < height_in_tiles);

  return tiles[y * width_in_tiles + x].index != -1;
}


