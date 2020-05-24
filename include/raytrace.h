#ifndef RAYTRACE_H
#define RAYTRACE_H

#include <algorithm>
#include <chrono>
#include <iostream>
#include <memory>

#include "color.h"

class raytrace {
 public:
  raytrace(hittable_list world, double aspect_ratio, size_t image_width, size_t image_height, size_t samples_per_pixel,
           uint8_t max_depth, uint32_t num_threads)
      : world_(world),
        aspect_ratio_(aspect_ratio),
        image_width_(image_width),
        image_height_(image_height),
        samples_per_pixel_(samples_per_pixel),
        max_depth_(max_depth),
        num_threads_(num_threads) {}

  ImageWrapper calcImage(const camera& cam, size_t samples_per_pixel, std::string image_filename, bool do_log) {
    ImageWrapper image(image_filename, image_width_, image_height_);

    #pragma omp parallel for
    for (size_t j = 0; j < image_height_; j++) {
      if (do_log) {
        std::cout << "Line " << j << " / " << image_height_ << "\r" << std::flush;
      }
      for (size_t i = 0; i < image_width_; ++i) {
        color pixel_color = calcPixel(cam, i, j, samples_per_pixel);

        image.write_color(i, j, pixel_color, samples_per_pixel);
      };
    };

    return image;
  }

  color calcPixel(const camera& cam, size_t i, size_t j, size_t samples_per_pixel) {
    color pixel_color(0, 0, 0);

    for (size_t s = 0; s < samples_per_pixel; s++) {
      double u = (i + random_double()) / (image_width_ - 1);
      double v = (j + random_double()) / (image_height_ - 1);
      ray r = cam.get_ray(u, v);
      pixel_color += ray_color(r, world_, max_depth_);
    }

    return pixel_color;
  }

  color ray_color(const ray& r, const hittable& world, uint8_t depth) {
    hit_record rec;

    if (depth <= 0) return color(0, 0, 0);

    if (world.hit(r, 0.001, infinity, rec)) {
      color attenuation;
      ray scattered;
      if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
        return attenuation.cwiseProduct(ray_color(scattered, world, depth - 1));
      } else {
        return color(0, 0, 0);
      }
    }

    vec3 unit_direction = unit_vector(r.direction());
    double t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
  }

 private:
  hittable_list world_;
  double aspect_ratio_;
  size_t image_width_;
  size_t image_height_;
  size_t samples_per_pixel_;
  uint8_t max_depth_;
  uint32_t num_threads_;
};

#endif