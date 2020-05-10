#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#include <memory>
#include <png++/png.hpp>

#include "vec3.h"

class ImageWrapper {
 public:
  ImageWrapper(std::string filename, uint32_t width, uint32_t height)
      : image_(std::make_unique<png::image<png::rgb_pixel>>(width, height)),
        filename_(filename),
        width_(width),
        height_(height) {}

  void write_color(uint32_t x, uint32_t y, color pixel_color, int samples_per_pixel) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Divide the color total by the number of samples.
    auto scale = 1.0 / samples_per_pixel;
    r *= scale;
    g *= scale;
    b *= scale;

    // Write the translated [0,255] value of each color component.
    (*image_)[height_ - 1 - y][x] =
        png::rgb_pixel(static_cast<int>(256 * clamp(r, 0.0, 0.999)), static_cast<int>(256 * clamp(g, 0.0, 0.999)),
                       static_cast<int>(256 * clamp(b, 0.0, 0.999)));
  }

  void write() { image_->write(filename_); }

 private:
  std::unique_ptr<png::image<png::rgb_pixel>> image_;
  uint32_t width_;
  uint32_t height_;
  std::string filename_;
};

#endif