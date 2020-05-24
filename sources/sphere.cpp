#include "sphere.h"

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
  vec3 oc = r.origin() - center;
  double a = r.direction().squaredNorm();
  double half_b = dot(oc, r.direction());
  double c = oc.squaredNorm() - radius * radius;
  double discriminant = half_b * half_b - a * c;

  if (discriminant > 0) {
    double root = sqrt(discriminant);
    double temp = (-half_b - root) / a;
    if (temp < t_max && temp > t_min) {
      rec.t = temp;
      rec.p = r.at(rec.t);
      vec3 outward_normal = (rec.p - center) / radius;
      rec.set_face_normal(r, outward_normal);
      rec.mat_ptr = mat_ptr;
      
      return true;
    }
  }
  return false;
}