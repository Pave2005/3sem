cmake_minimum_required(VERSION 3.14)

project(terminal)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(run ./main.c ./src/terminal_emulator.c)
target_include_directories(run PRIVATE
                           ${CMAKE_CURRENT_SOURCE_DIR}/include)
