#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

#include "rtweekend.h"

#ifdef USE_EIGEN
#include <Eigen/Dense>

using vec3 = Eigen::Vector3d;

inline vec3 unit_vector(vec3 v) { return v.normalized(); }
inline double dot(const vec3 &u, const vec3 &v) { return u.dot(v); }
inline vec3 cross(const vec3 &u, const vec3 &v) { return u.cross(v); }
#else
class vec3 {
 public:
  vec3() : e{0, 0, 0} {}
  vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

  double x() const { return e[0]; }
  double y() const { return e[1]; }
  double z() const { return e[2]; }

  vec3 cwiseProduct(const vec3 &v) { return {e[0] * v[0], e[1] * v[1], e[2] * v[2]}; }

  vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
  double operator[](int i) const { return e[i]; }
  double &operator[](int i) { return e[i]; }

  vec3 &operator+=(const vec3 &v) {
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    return *this;
  }

  vec3 &operator*=(const double t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
  }

  vec3 &operator/=(const double t) { return *this *= 1 / t; }

  double norm() const { return std::sqrt(squaredNorm()); }

  double squaredNorm() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }

  public:
  double e[3];
};

inline vec3 operator+(const vec3 &u, const vec3 &v) { return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]); }

inline vec3 operator-(const vec3 &u, const vec3 &v) { return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]); }

inline vec3 operator*(const vec3 &u, const vec3 &v) { return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]); }

inline vec3 operator*(double t, const vec3 &v) { return vec3(t * v.e[0], t * v.e[1], t * v.e[2]); }

inline vec3 operator*(const vec3 &v, double t) { return t * v; }

inline vec3 operator/(vec3 v, double t) { return (1 / t) * v; }

inline double dot(const vec3 &u, const vec3 &v) { return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2]; }

inline vec3 cross(const vec3 &u, const vec3 &v) {
  return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1], u.e[2] * v.e[0] - u.e[0] * v.e[2], u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline std::ostream &operator<<(std::ostream &out, const vec3 &v) { return out << v[0] << ' ' << v[1] << ' ' << v[2]; }

inline vec3 unit_vector(vec3 v) { return v / v.norm(); }
#endif

// Type aliases for vec3
using point3 = vec3;  // 3D point
using color = vec3;   // RGB color

// vec3 Utility Functions
inline vec3 reflect(const vec3 &v, const vec3 &n) { return v - 2 * dot(v, n) * n; }

vec3 refract(const vec3 &uv, const vec3 &n, double etai_over_etat);

inline static vec3 random_vec3() { return vec3(random_double(), random_double(), random_double()); }

inline static vec3 random_vec3(double min, double max) {
  return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
}

extern vec3 random_in_unit_sphere();
extern vec3 random_unit_vector();
extern vec3 random_in_hemisphere(const vec3 &normal);
extern vec3 random_in_unit_disk();

#endif