cmake_minimum_required(VERSION 3.14)

project(signal)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(signal ./main.c ./src/funcs.c)
target_include_directories(signal PRIVATE
                           ${CMAKE_CURRENT_SOURCE_DIR}/include)
