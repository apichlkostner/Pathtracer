#include <algorithm>
#include <iomanip>
#include <iostream>
#include <thread>

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "material.h"
#include "random_world.h"
#include "raytrace.h"
#include "rtweekend.h"
#include "sphere.h"
#include "stop_watch.h"

static void log(double delta_time, size_t image_number, size_t num_rotation_steps) {
  double finished_in = delta_time * (num_rotation_steps - image_number);
  std::cout << "Finished " << image_number << " of " << num_rotation_steps << " -> calc time " << std::fixed << std::setprecision(3)
            << std::setfill('0') << delta_time << "s, sequence will be finished in " << std::setprecision(1) << finished_in
            << "s == " << finished_in / 60. << "m == " << finished_in / 3600. << "h\r" << std::flush;
}

int main() {
  constexpr double aspect_ratio = 16.0 / 9.0;
  constexpr size_t image_width = 320;
  constexpr size_t image_height = static_cast<int>(image_width / aspect_ratio);
  constexpr size_t samples_per_pixel = 50;
  constexpr uint8_t max_depth = 50;
  // const uint32_t num_threads = std::thread::hardware_concurrency();

  const point3 lower_left_corner(-2.0, -1.0, -1.0);
  const vec3 horizontal(4.0, 0.0, 0.0);
  const vec3 vertical(0.0, 2.0, 0.0);
  const point3 origin(0.0, 0.0, 0.0);

  const hittable_list world = randomWorld::generate_random_scene();

  const point3 lookfrom(13., 2., 3.);
  const point3 lookat(0, 0, 0);
  const vec3 vup(0, 1, 0);
  constexpr double dist_to_focus = 10.0;
  constexpr double aperture = 0.1;

  raytrace raytracer(world, image_width, image_height, samples_per_pixel, max_depth);

  constexpr double rotation_angle_delta = 0.01;
#ifdef USE_EIGEN
  size_t num_rotation_steps = size_t(M_PI / rotation_angle_delta);
#else
  size_t num_rotation_steps = 1;
#endif

  for (size_t image_number = 0; image_number < num_rotation_steps; image_number++) {
    stopWatch stop_watch;

    stop_watch.start();

    Eigen::AngleAxis<double> rotation(image_number * rotation_angle_delta, vec3(0, 1., 0));
    // std::cout << rotation * lookfrom << std::endl;
    camera cam(rotation * lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);
    std::string image_filename = "raytrace" + std::to_string(image_number);

    ImageWrapper image = raytracer.calcImage(cam, image_filename, false);

    image.write();

    double delta_time = stop_watch.stop();
    log(delta_time, image_number, num_rotation_steps);
  }

  std::cout << "\nDone.\nYou can make a video with ffmpeg -r 60 -i raytrace%d.png -vcodec libx264 -crf 15 -pix_fmt "
               "yuv420p raytrace.mp4 if you like."
            << std::endl;
}