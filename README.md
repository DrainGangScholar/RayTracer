
# Ray Tracing in One Weekend

This repository contains my implementation of the ray tracing techniques outlined in the book "Ray Tracing in One Weekend" by Peter Shirley. I followed the tutorial in the book to create a simple ray tracer from scratch.

## Overview

Ray tracing is a rendering technique used to generate realistic images by tracing the path of light as pixels in an image plane and simulating the way it interacts with the objects in a scene.

## Results

Below is the result of following the tutorial:

![Ray Tracing Result](assets/result.jpeg)

## How to Run

To run the code:

1. Clone the repository.
2. ``cd RayTracer``
3. ``mkdir build``
4. ``cd build``
5. ``cmake ..``
6. ``cmake --build .``
7. ``./RayTracer > image.ppm``

Please note that this ray tracer runs on the CPU, which can be slow for complex scenes. In the future, parallelization techniques or a GPU implementation could be explored to improve performance.

