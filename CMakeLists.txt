
cmake_minimum_required(VERSION 3.20)
project(physics2d LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find SDL2 (installed via Homebrew)
find_package(SDL2 REQUIRED)

add_executable(physics2d src/main.cpp)

target_include_directories(physics2d PRIVATE ${SDL2_INCLUDE_DIRS} src)
target_link_libraries(physics2d PRIVATE ${SDL2_LIBRARIES})