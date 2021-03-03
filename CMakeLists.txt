cmake_minimum_required(VERSION 3.17)
project(TC1SOA C)

set(CMAKE_C_STANDARD 99)

add_executable(TC1SOA main.c)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(TC1SOA Threads::Threads)
target_link_libraries(TC1SOA m)