add_executable(fifo ./src/main.c ./../common/src/utils.c)
target_include_directories(fifo PRIVATE
                           ${CMAKE_CURRENT_SOURCE_DIR}/include
                           ${CMAKE_CURRENT_SOURCE_DIR}/../common/include)
