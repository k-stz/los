#include "tileset.hpp"
#include "picojson.h"
#include "common.hpp"

#include <iostream>
#include <fstream>
#include <cassert>

Tileset::Tileset(const std::string &name) {
  this->name = name;
}

Tileset::~Tileset() {

}

TileDefinition *Tileset::get_tiledef(int index) {
  return &definitions.at(index);
}

void Tileset::load(SDL_Renderer *renderer) {
  auto filename = BASEPATH + name + ".json";
  std::cout << "Loading tileset " << filename << std::endl;

  std::ifstream t(filename);
  std::string file_contents((std::istreambuf_iterator<char>(t)),
                             std::istreambuf_iterator<char>());

  picojson::value root;
  std::string err = picojson::parse(root, file_contents);
  if (!err.empty()) {
    std::cerr << err << std::endl;
  }

  auto root_obj = root.get<picojson::object>();

  // Load texture
  std::string tileset_image = BASEPATH + root_obj["texture"].get<std::string>();
  SDL_Surface *surface = IMG_Load(tileset_image.c_str());
  this->texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);
  assert(this->texture != nullptr);


  // create tile definitions
  auto tiledef_array = root_obj["tiles"].get<picojson::array>();
  for (auto t : tiledef_array) {
    auto tile = t.get<picojson::object>();
    auto coord_arr = tile["coords"].get<picojson::array>();
    auto solid = tile["solid"].get<bool>();
    this->definitions.emplace_back(TileDefinition{
        static_cast<int>(coord_arr[0].get<double>()),
        static_cast<int>(coord_arr[1].get<double>()),
        static_cast<int>(coord_arr[2].get<double>()),
        static_cast<int>(coord_arr[3].get<double>()),
        solid});
  }
}
