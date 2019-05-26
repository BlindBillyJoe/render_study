cmake_minimum_required(VERSION 2.8)

project(GE)

add_subdirectory(swr)

add_executable(main main.cpp)

target_link_libraries(main swr)