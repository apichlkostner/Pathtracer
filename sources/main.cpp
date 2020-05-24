#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <thread>

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "material.h"
#include "rtweekend.h"
#include "sphere.h"

int g_seed;

color ray_color(const ray& r, const hittable& world, uint8_t depth) {
  hit_record rec;

  if (depth <= 0) return color(0, 0, 0);

  if (world.hit(r, 0.001, infinity, rec)) {
    // point3 target = rec.p + rec.normal + random_unit_vector();
    color attenuation;
    ray scattered;
    if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
      return attenuation.cwiseProduct(ray_color(scattered, world, depth - 1));
    } else {
      return color(0, 0, 0);
    }
  }

  vec3 unit_direction = unit_vector(r.direction());
  auto t = 0.5 * (unit_direction.y() + 1.0);
  return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

hittable_list random_scene() {
  hittable_list world;

  world.add(std::make_shared<sphere>(point3(0, -1000, 0), 1000, std::make_shared<lambertian>(color(0.5, 0.5, 0.5))));

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      auto choose_mat = random_double();
      point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());
      if ((center - vec3(4, 0.2, 0)).norm() > 0.9) {
        if (choose_mat < 0.8) {
          // diffuse
          vec3 albedo = random_vec3().cwiseProduct(random_vec3());
          world.add(std::make_shared<sphere>(center, 0.2, std::make_shared<lambertian>(albedo)));
        } else if (choose_mat < 0.95) {
          // metal
          auto albedo = random_vec3(.5, 1);
          auto fuzz = random_double(0, .5);
          world.add(std::make_shared<sphere>(center, 0.2, std::make_shared<metal>(albedo, fuzz)));
        } else {
          // glass
          world.add(std::make_shared<sphere>(center, 0.2, std::make_shared<dielectric>(1.5)));
        }
      }
    }
  }

  world.add(std::make_shared<sphere>(point3(0, 1, 0), 1.0, std::make_shared<dielectric>(1.5)));
  world.add(std::make_shared<sphere>(point3(-4, 1, 0), 1.0, std::make_shared<lambertian>(color(.4, .2, .1))));
  world.add(std::make_shared<sphere>(point3(4, 1, 0), 1.0, std::make_shared<metal>(color(.7, .6, .5), 0.0)));

  return world;
}

int main() {
  constexpr auto aspect_ratio = 16.0 / 9.0;
  constexpr size_t image_width = 1280;
  constexpr size_t image_height = static_cast<int>(image_width / aspect_ratio);
  constexpr size_t samples_per_pixel = 100;
  constexpr uint8_t max_depth = 50;
  const uint32_t num_threads = std::thread::hardware_concurrency();

  point3 lower_left_corner(-2.0, -1.0, -1.0);
  vec3 horizontal(4.0, 0.0, 0.0);
  vec3 vertical(0.0, 2.0, 0.0);
  point3 origin(0.0, 0.0, 0.0);

  auto world = random_scene();

  point3 lookfrom(13, 2, 3);
  point3 lookat(0, 0, 0);
  vec3 vup(0, 1, 0);
  auto dist_to_focus = 10.0;
  auto aperture = 0.1;

  constexpr double rotation_angle_delta = 0.01;
#ifdef USE_EIGEN
  size_t rotation_number = size_t(M_PI / rotation_angle_delta);
#else
  size_t rotation_number = 1;
#endif

  size_t image_number = 0;
  while (image_number < rotation_number) {
    std::vector<std::thread> calcthreads;

    for (size_t thread_nr = 0; (thread_nr < num_threads) && (image_number < rotation_number); thread_nr++) {
      image_number++;
      calcthreads.push_back(std::thread(
          [image_number, &lookfrom, &lookat, &vup, &world, aperture, rotation_number, dist_to_focus,
           num_threads](size_t thread_nr) mutable {
            auto start_time = std::chrono::high_resolution_clock::now();
#ifdef USE_EIGEN
            Eigen::AngleAxis<double> rotation(image_number * rotation_angle_delta, vec3(0, 1., 0));
            camera cam(rotation * lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);
#else
            camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);
#endif

            std::string image_filename = "raytrace" + std::to_string(image_number);
            ImageWrapper image(image_filename, image_width, image_height);

            for (size_t j = 0; j < image_height; j++) {
              if (thread_nr == 0) {
                std::cout << "Line " << j << " / " << image_height << "\r" << std::flush;
              }
              for (size_t i = 0; i < image_width; ++i) {
                color pixel_color(0, 0, 0);

                for (size_t s = 0; s < samples_per_pixel; ++s) {
                  auto u = (i + random_double()) / (image_width - 1);
                  auto v = (j + random_double()) / (image_height - 1);
                  ray r = cam.get_ray(u, v);
                  pixel_color += ray_color(r, world, max_depth);
                }

                image.write_color(i, j, pixel_color, samples_per_pixel);
              };
            };

            image.write();

            auto stop_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> calc_time = stop_time - start_time;
            auto finished_in = calc_time.count() * (rotation_number - image_number) / num_threads;
            std::cout << "Finished " << image_number << " of " << rotation_number << " -> " << std::fixed
                      << std::setprecision(3) << std::setfill('0') << calc_time.count() << "s, finished in "
                      << std::setprecision(1) << finished_in << "s == " << finished_in / 60.
                      << "m == " << finished_in / 3600. << "h" << std::endl;
          },
          thread_nr));
    }
    for (auto& t : calcthreads) t.join();
    calcthreads.clear();
  }
  std::cout << "\nDone.\nYou can make a video with ffmpeg -r 60 -i raytrace%d.png -vcodec libx264 -crf 15 -pix_fmt "
               "yuv420p raytrace.mp4 if you like."
            << std::endl;
}