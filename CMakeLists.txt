cmake_minimum_required(VERSION 3.21)
project(snake C)

set(CMAKE_C_STANDARD 99)

# Find the required curses package
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(snake main.c)

# Link the executable with the curses library
target_link_libraries(snake ${CURSES_LIBRARIES})
