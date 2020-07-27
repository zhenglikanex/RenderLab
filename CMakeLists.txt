cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 14)
project(Aurora)
include_directories("${PROJECT_SOURCE_DIR}")

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(Framework)
add_subdirectory(RHI)
add_subdirectory(Platform)
add_subdirectory(Test)
