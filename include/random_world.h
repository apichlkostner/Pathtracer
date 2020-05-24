#ifndef RANDOM_WORLD_H
#define RANDOM_WORLD_H

#include "hittable_list.h"

class randomWorld {
 public:
  static hittable_list generate_random_scene();
};

#endif