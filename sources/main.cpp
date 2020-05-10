#include <iostream>

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "rtweekend.h"
#include "sphere.h"

color ray_color(const ray& r, const hittable& world, uint8_t depth) {
  hit_record rec;

  if (world.hit(r, 0.001, infinity, rec)) {
    point3 target = rec.p + rec.normal + random_unit_vector();
    return 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth - 1);
  }

  vec3 unit_direction = unit_vector(r.direction());
  auto t = 0.5 * (unit_direction.y() + 1.0);
  return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {
  constexpr auto aspect_ratio = 16.0 / 9.0;
  constexpr int image_width = 640;
  constexpr int image_height = static_cast<int>(image_width / aspect_ratio);
  constexpr int samples_per_pixel = 100;
  constexpr uint8_t max_depth = 50;
  constexpr uint32_t num_threads = 16;

  ImageWrapper image("raytrace.png", image_width, image_height);

  point3 lower_left_corner(-2.0, -1.0, -1.0);
  vec3 horizontal(4.0, 0.0, 0.0);
  vec3 vertical(0.0, 2.0, 0.0);
  point3 origin(0.0, 0.0, 0.0);

  hittable_list world;
  world.add(std::make_shared<sphere>(point3(0, 0, -1), 0.5));
  world.add(std::make_shared<sphere>(point3(0, -100.5, -1), 100));
  camera cam;

  for (int j = image_height - 1; j >= 0; --j) {
    std::cout << "\rScanlines remaining: " << j << ' ' << std::flush;
    for (int i = 0; i < image_width; ++i) {
      color pixel_color(0, 0, 0);

      for (int s = 0; s < samples_per_pixel; ++s) {
        auto u = (i + random_double()) / (image_width - 1);
        auto v = (j + random_double()) / (image_height - 1);
        ray r = cam.get_ray(u, v);
        pixel_color += ray_color(r, world, max_depth);
      }

      image.write_color(i, j, pixel_color, samples_per_pixel);
    }
  }

  image.write();

  std::cout << "\nDone.\n";
}