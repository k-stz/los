#include "spriteanimation.hpp"

#include <SDL2/SDL_image.h>
#include <iostream>
#include <cassert>

SpriteAnimation::SpriteAnimation(SDL_Renderer *renderer,
                                 const std::string &filename,
                                 uint frame_ms,
                                 int frame_width,
                                 int frame_height) {
  this->frame_width = frame_width;
  this->frame_height = frame_height;
  this->frame_ms = frame_ms;

  auto surface = IMG_Load((BASEPATH + filename).c_str());
  assert(surface);
  this->texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);
}

uint SpriteAnimation::add_subanimation(uint frame_from, uint frame_to) {
  ranges.emplace_back(FrameRange{frame_from, frame_to});

  if (ranges.size() == 1) {
    // First range added
    this->current_frame_index = frame_from;
  }
  return ranges.size() - 1;
}

void SpriteAnimation::set_active_subanimation(uint index) {
  assert(index >= 0);
  assert(index < ranges.size());

  this->current_range = index;
}

void SpriteAnimation::update(uint delta) {
  assert(frame_ms > 0);
  current_frame_ms += delta;

  if (current_frame_ms >= frame_ms) {
    FrameRange range = this->ranges.at(current_range);
    current_frame_index ++;
    current_frame_ms = 0;
    if (current_frame_index > range.to)
      current_frame_index = range.from;
  }
}

void SpriteAnimation::get_frame_coords(SDL_Rect *rect) {
  int texture_width, texture_height;

  SDL_QueryTexture(this->texture, nullptr, nullptr, &texture_width, &texture_height);

  uint n_frames_x = texture_width / frame_width;
  uint n_frames_y = texture_height / frame_height;

  rect->x = 0;
  rect->y = 0;
  rect->w = frame_width;
  rect->h = frame_height;

  rect->y = current_frame_index / n_frames_x;
  rect->x = current_frame_index - (rect->y * n_frames_x);

  rect->x *= frame_width;
  rect->y *= frame_height;
}
