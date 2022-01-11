cmake_minimum_required(VERSION 3.16)
project(model)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_executable(model
        include/data.h
        include/device.h
        include/model.h
        src/model.cpp
        src/main.cpp)
