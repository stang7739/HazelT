cmake_minimum_required(VERSION 3.29)
project(HaZelT)

set(CMAKE_CXX_STANDARD 20)

#add_compile_options(/utf8)

add_compile_definitions(_UTF8)
#Load the cmake of the word directory
add_subdirectory(Hazel)
add_subdirectory(SandBox)