#pragma once

#include "./utility.h"
#include "interval.h"
#include "vec3.h"

class aabb {
public:
  interval x, y, z;

  aabb() {}
  aabb(const interval &x, const interval &y, const interval &z)
      : x(x), y(y), z(z) {}
  aabb(const point3 &a, const point3 &b) {
    x = (a[0] <= b[0]) ? interval(a[0], b[0]) : interval(b[0], a[0]);
    y = (a[1] <= b[1]) ? interval(a[1], b[1]) : interval(b[1], a[1]);
    z = (a[2] <= b[2]) ? interval(a[2], b[2]) : interval(b[2], a[2]);
  }
};
