#pragma once

#include "color.h"
#include "hittable.h"
#include "material.h"
#include "utility.h"
#include "vec3.h"
#include <iostream>
#include <omp.h>
#include <sstream>

class camera {
public:
  double aspect_ratio = 1.0;
  int image_width = 100;
  int samples_per_pixel = 10;
  int max_depth = 10;
  double vfov = 90;
  point3 lookfrom = point3(0, 0, 0);
  point3 lookat = point3(0, 0, -1);
  vec3 vup = vec3(0, 1, 0);

  double defocus_angle = 0;
  double focus_dist = 10;

  void render(const hittable &world) {
    initialize();

    std::ostringstream oss;

    oss << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    const int num_threads = 12;
    std::vector<std::ostringstream> thread_outputs(num_threads);
#pragma omp parallel for num_threads(num_threads)
    for (int i = 0; i < image_height; i++) {
      for (int j = 0; j < image_width; j++) {
        const auto thread_id = omp_get_thread_num();
        color pixel_color(0, 0, 0);
        for (int sample = 0; sample < samples_per_pixel; sample++) {
          ray r = get_ray(j, i);
          pixel_color += ray_color(r, max_depth, world);
        }
#pragma omp critical
        write_color(thread_outputs[thread_id],
                    pixel_samples_scale * pixel_color);
      }
    }
    for (const auto &thread_output : thread_outputs) {
      oss << thread_output.str();
    }

    std::cout << oss.str();

    std::clog << "\rDone.                 \n";
  }

  ray get_ray(int i, int j) const {
    auto pixel_center =
        pixel100_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
    auto pixel_sample = pixel_center + pixel_sample_square();

    auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
    auto ray_direction = pixel_sample - ray_origin;

    return ray(ray_origin, ray_direction);
  }

  vec3 pixel_sample_square() const {
    auto px = -0.5 + random_double();
    auto py = -0.5 + random_double();
    return (px * pixel_delta_u) + (py * pixel_delta_v);
  }
  point3 defocus_disk_sample() const {
    auto p = random_in_unit_disk();
    return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
  }

private:
  int image_height;
  point3 center;
  point3 pixel100_loc;
  vec3 pixel_delta_u;
  vec3 pixel_delta_v;
  double pixel_samples_scale;
  vec3 u, v, w;
  vec3 defocus_disk_u;
  vec3 defocus_disk_v;

  void initialize() {
    image_height = static_cast<int>(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    pixel_samples_scale = 1.0 / samples_per_pixel;

    center = lookfrom;

    // viewport dimensions
    auto theta = degrees_to_radians(vfov);
    auto h = tan(theta / 2);
    auto viewport_height = 2.0 * h * focus_dist;
    auto viewport_width =
        viewport_height * (static_cast<double>(image_width) / image_height);

    w = unit_vector(lookfrom - lookat);
    u = unit_vector(cross(vup, w));
    v = cross(w, u);

    // u and v vector
    auto viewport_u = viewport_width * u;
    auto viewport_v = viewport_height * -v;

    // pixel to pixel delta u, delta v
    pixel_delta_u = viewport_u / image_width;
    pixel_delta_v = viewport_v / image_height;

    // upper left pixel position
    auto viewport_upper_left =
        center - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;
    pixel100_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    auto defocus_radius =
        focus_dist * tan(degrees_to_radians(defocus_angle / 2));
    defocus_disk_u = u * defocus_radius;
    defocus_disk_v = v * defocus_radius;
  }

  color ray_color(const ray &r, int depth, const hittable &world) {
    if (depth <= 0) {
      return color(0, 0, 0);
    }
    hit_record rec;

    if (world.hit(r, interval(0.001, infinity), rec)) {
      ray scattered;
      color attenuation;
      if (rec.mat->scatter(r, rec, attenuation, scattered)) {
        return attenuation * ray_color(scattered, depth - 1, world);
      }
      return color(0, 0, 0);
    }
    // TODO:render background instead of this
    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
  }
};
