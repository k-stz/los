#ifndef __VEC_HPP
#define __VEC_HPP


class vec2 {
public:
  float x = 0;
  float y = 0;

  vec2(){}
  vec2(float x, float y) {
    this->x = x;
    this->y = y;
  }

  vec2& operator += (const vec2& other) {
    this->x += other.x;
    this->y += other.y;
    return *this;
  }

  vec2& operator * (const float other) {
    this->x *= other;
    this->y *= other;
    return *this;
  }

  friend vec2 operator + (vec2 lhs, const vec2& rhs) {
    lhs += rhs;
    return lhs;
  }
};

#endif
