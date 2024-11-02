cmake_minimum_required(VERSION 3.14)

project(IPC)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(fifo)
add_subdirectory(msg_queue)
add_subdirectory(shmem)


