#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>
#include <functional>
#include <random>

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) { return degrees * pi / 180; }

inline double random_double() {
    // thread_local to speed up, else the threads wait most of the time
    static thread_local std::mt19937 generator;
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    return distribution(generator);
}

inline double random_double(double min, double max) {
  // Returns a random real in [min,max).
  return min + (max - min) * random_double();
}

inline double clamp(double x, double min, double max) {
  if (x < min) return min;
  if (x > max) return max;
  return x;
}

#endif