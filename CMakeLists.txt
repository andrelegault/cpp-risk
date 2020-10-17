cmake_minimum_required(VERSION 3.17.3)
set (CMAKE_CXX_STANDARD 20)

project(WarZone
    VERSION 0.0.1
    DESCRIPTION "COMP345 Project"
    LANGUAGES CXX
)

add_subdirectory(src)

add_subdirectory(drivers)

file(COPY maps DESTINATION drivers)