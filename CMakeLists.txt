cmake_minimum_required(VERSION 3.17.0)
set (CMAKE_CXX_STANDARD 20)
set (CMAKE_BUILD_TYPE Debug)

project(WarZone
    VERSION 0.0.1
    DESCRIPTION "COMP345 Project"
    LANGUAGES CXX
)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

add_subdirectory(src)

add_subdirectory(drivers)

file(COPY maps DESTINATION bin)