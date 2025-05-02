cmake_minimum_required(VERSION 3.10)

project(list VERSION 1.0.0)

add_executable(list main/main.cpp)

set_target_properties(
    list PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
)

include_directories(list PRIVATE ${CMAKE_SOURCE_DIR}/Header)