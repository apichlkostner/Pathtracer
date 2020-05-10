#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#define USE_CPP_RANDOM 1

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>
#if USE_CPP_RANDOM
#include <functional>
#include <random>
#endif

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) { return degrees * pi / 180; }

#if USE_CPP_RANDOM
inline double random_double() {
  static std::uniform_real_distribution<double> distribution(0.0, 1.0);
  static std::mt19937 generator;
  static std::function<double()> rand_generator = std::bind(distribution, generator);

  return rand_generator();
}
#else
inline double random_double() {
  // Returns a random real in [0,1).
  return rand() / (RAND_MAX + 1.0);
}
#endif

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