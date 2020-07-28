cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 14)
project(Aurora)

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)
include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/3rdparty/opengex/include")

find_library(OPENDDL_LIBRARY OpenDDL PATHS ${MYGE_EXTERNAL_LIBRARY_PATH})
find_library(OPENGEX_LIBRARY OpenGEX PATHS ${MYGE_EXTERNAL_LIBRARY_PATH})

add_subdirectory(Framework)
add_subdirectory(RHI)
add_subdirectory(Platform)
add_subdirectory(Test)
