cmake_minimum_required(VERSION 3.16)
project(Tangram)

set(CMAKE_CXX_STANDARD 14)

include_directories("../../Libraries/glew-2.1.0/include")
include_directories("../../Libraries/glfw-3.3.8/include")
include_directories("../../Libraries/glm")

add_executable(Tangram hello-2d-world/hello-2d-world.cpp)
