#include "level.hpp"
#include "picojson.h"
#include "common.hpp"

#include <iostream>
#include <fstream>
#include <SDL2/SDL_image.h>
#include <cassert>

Level::Level(const std::string &filename, SDL_Renderer *renderer) {
  this->load(BASEPATH + filename + ".json", renderer);
}

Level::~Level() {
  SDL_DestroyTexture(bg_texture);
  delete[] tiles;
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

    TileDefinition *tile_def = tileset->get_tiledef(this->tiles[i].index);
    SDL_Rect texture_coords = {tile_def->x, tile_def->y, tile_def->w, tile_def->h};
    SDL_Rect screen_coords  = {offset_x + x * TILE_SIZE, offset_y + y * TILE_SIZE, TILE_SIZE, TILE_SIZE};

    SDL_RenderCopy(renderer, this->tileset->texture,
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

  auto player_pos_array = root_obj["player_start_pos"].get<picojson::array>();
  this->player_start_pos.x = player_pos_array[0].get<double>();
  this->player_start_pos.y = player_pos_array[1].get<double>();

  this->tiles = new Tile[width_in_tiles * height_in_tiles];
  for (unsigned int i = 0; i < width_in_tiles * height_in_tiles; i ++)
    tiles[i] = {0, 0, -1};

  // Load background image
  // TODO: Make this optional
  std::string bg_path = "../data/" + root_obj["background"].get<std::string>();
  SDL_Surface *bg_surface = IMG_Load(bg_path.c_str());
  this->bg_texture = SDL_CreateTextureFromSurface(renderer, bg_surface);
  SDL_FreeSurface(bg_surface);
  assert(bg_texture != nullptr);

  // Parse tileset
  // TODO: We might want to just save tilesets in their own file
  auto tileset_name = root_obj["tileset"].get<std::string>();
  this->tileset = new Tileset(tileset_name);
  this->tileset->load(renderer);

  auto tiles_array = root_obj["tiles"].get<picojson::array>();
  int cur_row = 0;
  for (auto r : tiles_array) {
    auto row = r.get<picojson::array>();

    assert(row.size() <= width_in_tiles);

    int cur_col = 0;
    for (auto c : row) {
      int tile_id = static_cast<int>(c.get<double>());
      tiles[cur_row * width_in_tiles + cur_col].index = tile_id;
      cur_col ++;
    }
    cur_row ++;
  }

  // TODO: Hardcoded screen size
  // Set initial level offset
  // TODO: Depending on the player position, this might be wrong (player should always be visible).
  this->offset_x = std::max(0, (1024 - static_cast<int>((width_in_tiles  * TILE_SIZE))) / 2);
  this->offset_y = std::max(0, (768  - static_cast<int>((height_in_tiles * TILE_SIZE))) / 2);
}

bool Level::is_tile_solid(unsigned int x, unsigned int y) {
  assert(x >= 0);
  assert(x < width_in_tiles);
  assert(y >= 0);
  assert(y < height_in_tiles);

  // TODO: Use 'solid' flag from TileDefinition
  return tiles[y * width_in_tiles + x].index != -1;
}


