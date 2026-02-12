cmake_minimum_required(VERSION 3.28)
project(cpp_programming_course LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Global warnings
if(MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_subdirectory(assignment0)
add_subdirectory(assignment1)
add_subdirectory(assignment2)
add_subdirectory(assignment3)
add_subdirectory(assignment4)
