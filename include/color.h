#pragma once

#include "interval.h"
#include "vec3.h"

#include <iostream>

using color = vec3;

inline double linear_to_gamma(double linear_component) {
  if (linear_component > 0)
    return sqrt(linear_component);

  return 0;
}

inline void write_color(std::ostream &out, color pixel_color) {
  auto r = pixel_color.x();
  auto g = pixel_color.y();
  auto b = pixel_color.z();

  static const interval intensity(0.000, 0.999);

  r = linear_to_gamma(r);
  g = linear_to_gamma(g);
  b = linear_to_gamma(b);

  const int rbyte = static_cast<int>(256 * intensity.clamp(r));
  const int gbyte = static_cast<int>(256 * intensity.clamp(g));
  const int bbyte = static_cast<int>(256 * intensity.clamp(b));

  out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}
