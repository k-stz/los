#ifndef __UTIL__HPP
#define __UTIL__HPP

#include "vec.hpp"

struct Input {
  unsigned int keycode;
  bool up = false;
  bool down = false;
  vec2 mouse;
};


#endif
