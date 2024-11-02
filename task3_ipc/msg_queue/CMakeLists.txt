add_executable(msg_queue ./src/main.c ./../common/src/utils.c)
target_include_directories(msg_queue PRIVATE
                           ${CMAKE_CURRENT_SOURCE_DIR}/include
                           ${CMAKE_CURRENT_SOURCE_DIR}/../common/include)
