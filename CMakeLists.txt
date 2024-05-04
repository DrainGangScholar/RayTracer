cmake_minimum_required(VERSION 3.5)
project(RayTracer)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenMP)

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
if(OpenMP_CXX_FOUND)
    target_link_libraries(RayTracer PUBLIC OpenMP::OpenMP_CXX)
endif()

