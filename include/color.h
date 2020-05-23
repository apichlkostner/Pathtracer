#ifndef COLOR_H
#define COLOR_H

#include <array>
#include <iostream>
#include <memory>

#include "vec3.h"

#ifdef USE_PNG
#include <png++/png.hpp>
typedef png::rgb_pixel image_color;
typedef png::image<image_color> image_type;
const std::string image_filename_postfix = ".png";
#else
#include "p3.h"
typedef color image_color;
typedef P3<image_color> image_type;
const std::string image_filename_postfix = ".p3";
#endif

class ImageWrapper {
 public:
  ImageWrapper(std::string filename, uint32_t width, uint32_t height)
      : image_(std::make_unique<image_type>(width, height)), width_(width), height_(height), filename_(filename) {}

  void write_color(uint32_t x, uint32_t y, color pixel_color, int samples_per_pixel) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Divide the color total by the number of samples.
    auto scale = 1.0 / samples_per_pixel;
    r = std::sqrt(scale * r);
    g = std::sqrt(scale * g);
    b = std::sqrt(scale * b);

    // Write the translated [0,255] value of each color component.
    auto pixel = image_color(static_cast<int>(256 * clamp(r, 0.0, 0.999)), static_cast<int>(256 * clamp(g, 0.0, 0.999)),
                             static_cast<int>(256 * clamp(b, 0.0, 0.999)));
    image_->set_pixel(x, height_ - 1 - y, pixel);
  }

  void write() { image_->write(filename_ + image_filename_postfix); }

 private:
  std::unique_ptr<image_type> image_;
  uint32_t width_;
  uint32_t height_;
  std::string filename_;
};

#endif