cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 14)
project(Aurora)

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)
include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/3rdparty/glm/include")
include_directories("${PROJECT_SOURCE_DIR}/3rdparty/opengex/include")
include_directories("${PROJECT_SOURCE_DIR}/3rdparty/stb/")

IF(${UNIX})
	
ELSEIF(${WIN32})
	add_definitions(-DGUID_WINDOWS)
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES Orbis)
ENDIF(${UNIX})

set(OPENGEX_LIB_PATH ${PROJECT_SOURCE_DIR}/3rdparty/opengex/lib)

find_library(OPENDDL_LIBRARY OpenDDL PATHS ${OPENGEX_LIB_PATH})
find_library(OPENGEX_LIBRARY OpenGEX PATHS ${OPENGEX_LIB_PATH})

add_subdirectory(Framework)
add_subdirectory(RHI)
add_subdirectory(Platform)
add_subdirectory(Test)
