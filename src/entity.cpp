#include "entity.hpp"
#include <iostream>

// Empty default implementation
void Entity::input(const Input *input) {
}

void Entity::update(unsigned int delta) {
  //this->position += (this->current_force * delta);
}

void Entity::apply_force(vec2 force, unsigned int delta) {
  this->current_force += force * (delta / 10.0f);
}
