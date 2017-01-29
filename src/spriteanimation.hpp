#ifndef TILE_ANIM_HPP
#define TILE_ANIM_HPP

#include <string>
#include <SDL2/SDL.h>
#include <vector>

#include "common.hpp"

struct FrameRange {
  uint from;
  uint to;
};

class SpriteAnimation {
public:
  SDL_Texture *texture;
  // TODO: The SpriteAnimation actually doesn't need the
  //       texture at all, just for its width/height,
  //       so move it somewhere else?
  SpriteAnimation(SDL_Renderer *renderer,
                  const std::string &filename,
                  uint frame_ms,
                  int frame_width,
                  int frame_height);

  // A single sprite sheet might contain multiple sub animations,
  // like a player walking, but also jumping or swimming, etc.
  uint add_subanimation(uint frame_from, uint frame_to);
  void set_active_subanimation(uint index);

  void update(uint delta);

  void get_frame_coords(SDL_Rect *rect);

private:
  std::vector<FrameRange> ranges;
  int frame_width;
  int frame_height;
  uint frame_ms;

  uint current_range = 0;
  uint current_frame_ms = 0;
  uint current_frame_index = 0;
  uint n_frames = 0;
};

#endif
