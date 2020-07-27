add_library(OpenGLRHI
        OpenGLGraphicsManager.hpp
		OpenGLGraphicsManager.cpp
        ${PROJECT_SOURCE_DIR}/3rdparty/glad/src/glad.c
)
target_include_directories(OpenGLRHI PUBLIC "${PROJECT_SOURCE_DIR}/3rdparty/glad/include")

SET_PROPERTY(TARGET OpenGLRHI PROPERTY FOLDER "RHI")