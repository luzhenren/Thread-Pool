cmake_minimum_required(VERSION 3.27)
project(ThreadPool)

set(CMAKE_CXX_STANDARD 17)

add_executable(ThreadPool main.cpp
        src/threadpool.cpp
        include/threadpool.h)
