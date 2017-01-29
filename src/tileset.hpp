#ifndef TILESET_HPP
#define TILESET_HPP

#include <string>
#include <vector>
#include <SDL2/SDL_image.h>


struct TileDefinition {
  int x, y, w, h;
  bool solid;
};


class Tileset {
public:
  SDL_Texture *texture;

  Tileset (const std::string &name);
  virtual ~Tileset();

  void load(SDL_Renderer *renderer);
  TileDefinition *get_tiledef(int index);

private:
  std::string name;

  std::vector<TileDefinition> definitions;
};

#endif
