#ifndef P3_H
#define P3_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

template <class COLOR>
class P3 {
 public:
  P3(size_t width, size_t height) : width_(width), height_(height) { image_.resize(width * height); };
  void write(std::string filename) {
    std::ofstream image;
    image.open(filename);
    
    image << "P3\n" << width_ << ' ' << height_ << "\n255\n";
    
    for (auto pixel : image_) {
      image << pixel << " ";
    }

    image.close();
  }

  void set_pixel(size_t x, size_t y, COLOR color_in) { image_[x + width_ * y] = color_in; }
  COLOR get_pixel(size_t x, size_t y) const { return image_[x + width_ * y]; }

 private:
  std::vector<COLOR> image_;
  size_t width_;
  size_t height_;
};

#endif