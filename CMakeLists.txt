cmake_minimum_required(VERSION 3.10.0)
project(ring_buffer)

set(CMAKE_CXX_STANDARD 17)
aux_source_directory(ring_buffer RING_BUFFER_SRC)
add_executable(ring_buffer main.cpp ${RING_BUFFER_SRC})