cmake_minimum_required(VERSION 3.11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

project(AH)

add_executable(cheese mina.cpp test.cpp test.hpp)

target_link_libraries(cheese PRIVATE gpiod)