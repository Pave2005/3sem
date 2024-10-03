cmake_minimum_required(VERSION 3.14)

project(duplex_pipe)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(pipe ./main.c ./src/data_processing.c ./src/dup_pipe.c)
target_include_directories(pipe PRIVATE
                           ${CMAKE_CURRENT_SOURCE_DIR}/include)


