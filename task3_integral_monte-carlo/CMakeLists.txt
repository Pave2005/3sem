cmake_minimum_required(VERSION 3.14)

project(monte_carlo)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(integral ./main.c ./src/functions.c ./src/integral.c)
target_include_directories(integral PRIVATE
                           ${CMAKE_CURRENT_SOURCE_DIR}/include
                           ${CMAKE_CURRENT_SOURCE_DIR}/config)
