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

  void write_color(uint32_t x, uint32_t y, color pixel_color) {
    // Write the translated [0,255] value of each color component.
    (*image_)[height_ - 1 - y][x] =
        png::rgb_pixel(static_cast<int>(255.999 * pixel_color.x()), static_cast<int>(255.999 * pixel_color.y()),
                       static_cast<int>(255.999 * pixel_color.z()));
  }

  void write() { image_->write(filename_); }

 private:
  std::unique_ptr<png::image<png::rgb_pixel>> image_;
  uint32_t width_;
  uint32_t height_;
  std::string filename_;
};

#endif