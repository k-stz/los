#include "entity.hpp"
#include <iostream>

// Empty default implementation
void Entity::input(Input *input) {
}

void Entity::update(unsigned int delta) {
  this->position += (this->current_force * (delta));
}

void Entity::apply_force(vec2 force) {
  this->current_force += force;
}
