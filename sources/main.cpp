#include <iostream>

#include "color.h"
#include "hittable_list.h"
#include "rtweekend.h"
#include "sphere.h"

color ray_color(const ray& r, const hittable& world) {
  hit_record rec;
  if (world.hit(r, 0, infinity, rec)) {
    return 0.5 * (rec.normal + color(1, 1, 1));
  }
  vec3 unit_direction = unit_vector(r.direction());
  auto t = 0.5 * (unit_direction.y() + 1.0);
  return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {
  const auto aspect_ratio = 16.0 / 9.0;
  const int image_width = 1024;
  const int image_height = static_cast<int>(image_width / aspect_ratio);

  ImageWrapper image("raytrace.png", image_width, image_height);

  point3 lower_left_corner(-2.0, -1.0, -1.0);
  vec3 horizontal(4.0, 0.0, 0.0);
  vec3 vertical(0.0, 2.0, 0.0);
  point3 origin(0.0, 0.0, 0.0);

  hittable_list world;
  world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
  world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

  for (int j = image_height - 1; j >= 0; --j) {
    std::cout << "\rScanlines remaining: " << j << ' ' << std::flush;
    for (int i = 0; i < image_width; ++i) {
      auto u = double(i) / (image_width - 1);
      auto v = double(j) / (image_height - 1);
      
      ray r(origin, lower_left_corner + u * horizontal + v * vertical);
      color pixel_color = ray_color(r, world);

      image.write_color(i, j, pixel_color);
    }
  }

  image.write();

  std::cout << "\nDone.\n";
}