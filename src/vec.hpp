#ifndef __VEC_HPP
#define __VEC_HPP

#include <istream>

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


  vec2 operator * (const float f) {
    vec2 other;
    other.x = this->x * f;
    other.y = this->y * f;
    return other;
  }

  friend vec2 operator + (vec2 lhs, const vec2& rhs) {
    lhs += rhs;
    return lhs;
  }

  friend std::ostream &operator<<(std::ostream &os, vec2 const &m) {
    os << "vec2(" << m.x << ", " << m.y << ")";
    return os;
  }

  void min_all(float min) {
    this->x = std::min(x, min);
    this->y = std::min(y, min);
  }
};

#endif
