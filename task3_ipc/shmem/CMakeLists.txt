add_executable(shmem ./src/main.c ./../common/src/utils.c)
target_include_directories(shmem PRIVATE
                           ${CMAKE_CURRENT_SOURCE_DIR}/include
                           ${CMAKE_CURRENT_SOURCE_DIR}/../common/include)
