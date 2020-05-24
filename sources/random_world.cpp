#include "random_world.h"

#include "material.h"
#include "sphere.h"

hittable_list randomWorld::generate_random_scene() {
  hittable_list world;

  world.add(std::make_shared<sphere>(point3(0, -1000, 0), 1000, std::make_shared<lambertian>(color(0.5, 0.5, 0.5))));

  world.add(std::make_shared<sphere>(point3(0, 1, 0), 1.0, std::make_shared<dielectric>(1.5)));
  world.add(std::make_shared<sphere>(point3(-4, 1, 0), 1.0, std::make_shared<lambertian>(color(.4, .2, .1))));
  world.add(std::make_shared<sphere>(point3(4, 1, 0), 1.0, std::make_shared<metal>(color(.7, .6, .5), 0.0)));

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      double choose_mat = random_double();
      point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());
      if ((center - vec3(4, 0.2, 0)).norm() > 0.9) {
        if (choose_mat < 0.8) {
          // diffuse
          vec3 albedo = random_vec3().cwiseProduct(random_vec3());
          world.add(std::make_shared<sphere>(center, 0.2, std::make_shared<lambertian>(albedo)));
        } else if (choose_mat < 0.95) {
          // metal
          vec3 albedo = random_vec3(.5, 1);
          double fuzz = random_double(0, .5);
          world.add(std::make_shared<sphere>(center, 0.2, std::make_shared<metal>(albedo, fuzz)));
        } else {
          // glass
          world.add(std::make_shared<sphere>(center, 0.2, std::make_shared<dielectric>(1.5)));
        }
      }
    }
  }
  return world;
}