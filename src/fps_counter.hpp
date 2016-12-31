#ifndef FPS_COUNTER_HPP
#define FPS_COUNTER_HPP

struct FPSCounter {
  int current_fps   = 0;
  int frame_counter = 0;
  int fps_counter   = 0;

  void update(unsigned int delta) {
    fps_counter += delta;
    frame_counter ++;

    if (fps_counter >= 1000) {
      current_fps   = frame_counter;
      frame_counter = 0;
      fps_counter   = 0;
    }
  }
};

//class FPSCounter {

//};

#endif
