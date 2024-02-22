cmake_minimum_required(VERSION 3.0)
project(RayTracer)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(RayTracer
        include/color.h
        include/ray.h
        include/vec3.h
	include/hittable.h
	include/sphere.h
        src/main.cpp
		include/interval.h
		include/camera.h)
