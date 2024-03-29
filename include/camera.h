#pragma once

#include "hittable.h"
#include "color.h"
#include <iostream>

class camera {
public:
    double aspect_ratio = 1.0;
    int image_width = 100;
    int samples_per_pixel;

    void render(const hittable &world) {
        initialize();

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int i = 0; i < image_height; i++) {
            std::clog << "\rScanlines remaining: " << (image_height - i) << ' ' << std::flush;
            for (int j = 0; j < image_width; j++) {
                color pixel_color(0,0,0);
                for(int sample=0;sample<samples_per_pixel;sample++) {
                    ray r = get_ray(j,i);
                    pixel_color+=ray_color(r,world);
                }
                write_color(std::cout, pixel_color,samples_per_pixel);
            }
        }

        std::clog << "\rDone.                 \n";
    }

    ray get_ray(int i, int j) const {
        auto pixel_center = pixel100_loc+(i*pixel_delta_u)+(j*pixel_delta_v);
        auto pixel_sample = pixel_center + pixel_sample_square();

        auto ray_origin=center;
        auto ray_direction=pixel_sample-ray_origin;

        return ray(ray_origin, ray_direction);
    }

    vec3 pixel_sample_square() const {
        auto px = -0.5+random_double();
        auto py = -0.5+random_double();
        return (px*pixel_delta_u)+(py*pixel_delta_v);
    }

private:
    int image_height;
    point3 center;
    point3 pixel100_loc;
    vec3 pixel_delta_u;
    vec3 pixel_delta_v;

    void initialize() {
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        center = point3(0, 0, 0);

        //viewport dimensions
        auto focal_length = 1.0;
        auto viewport_height = 2.0;
        auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);

        //u and v vector
        auto viewport_u = vec3(viewport_width, 0, 0);
        auto viewport_v = vec3(0, -viewport_height, 0);

        //pixel to pixel delta u, delta v
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        //upper left pixel position
        auto viewport_upper_left = center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
        pixel100_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    }

    color ray_color(const ray &r, const hittable &world) {
        hit_record rec;

        if (world.hit(r, interval(0, infinity), rec)) {
            return 0.5 * (rec.normal + color(1, 1, 1));
        }

        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
    }
};