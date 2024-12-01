cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(day01 VERSION 1.0)
add_compile_options(-Wall -Wextra -pedantic -Werror -O3)
add_executable(part1 part1.cpp)
add_executable(part2 part2.cpp)
