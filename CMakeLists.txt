cmake_minimum_required(VERSION 3.25)
cmake_minimum_required(VERSION 3.25)
project(Proyecto)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/Dependencies/glfw/lib/cmake/glfw3")

add_executable(Proyecto main.cpp)

include_directories(Proyecto/glfw/include/GLFW/)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 CONFIG REQUIRED)

target_link_libraries(Proyecto opengl32 glfw)
