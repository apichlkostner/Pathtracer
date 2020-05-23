#include "vec3.h"

vec3 random_in_unit_sphere() {
  vec3 p;

  do {
    p = random_vec3(-1, 1);
  } while (p.squaredNorm() >= 1);

  return p;
}

vec3 random_unit_vector() {
  auto a = random_double(0, 2 * pi);
  auto z = random_double(-1, 1);
  auto r = sqrt(1 - z * z);
  return vec3(r * cos(a), r * sin(a), z);
}

vec3 random_in_hemisphere(const vec3& normal) {
  vec3 in_unit_sphere = random_in_unit_sphere();
  if (dot(in_unit_sphere, normal) > 0.0)  // In the same hemisphere as the normal
    return in_unit_sphere;
  else
    return -in_unit_sphere;
}

vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
  auto cos_theta = dot(-uv, n);
  vec3 r_out_parallel = etai_over_etat * (uv + cos_theta * n);
  vec3 r_out_perp = -sqrt(1.0 - r_out_parallel.squaredNorm()) * n;

  return r_out_parallel + r_out_perp;
}

vec3 random_in_unit_disk() {
  vec3 p;

  do {
    p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
  } while (p.squaredNorm() > 1);

  return p;
}